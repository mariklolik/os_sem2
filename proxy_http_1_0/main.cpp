#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <netdb.h>

#define RED   "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE  "\033[1;34m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"
#define PURPLE "\033[1;35m"
#define RESET "\033[0m"

#define CLIENTS_LIMIT 5
#define BUFFER_SIZE 8192
#define DEFAULT_METHOD_SIZE 10
#define DEFAULT_URI_SIZE 50
#define DEFAULT_VERSION_SIZE 50
#define DEFAULT_HOST_SIZE 50


typedef struct {
    int client_socket;
    struct sockaddr_in client_address;
} client_handler_ctx;

int host_from_url(char *request, char *resolved_host) {
    char *host_start = strstr(request, "http://");
    if (host_start == NULL) {
        return -1;
    }
    host_start += strlen("http://");

    const char *host_end = strchr(host_start, '/');
    if (host_end == NULL) {
        return -1;
    }

    size_t host_length = host_end - host_start;
    if (host_length <= 0) {
        return -1;
    }
    strncpy(resolved_host, host_start, host_length);
    resolved_host[host_length] = '\0';

    return 0;
}

int host_from_request(char *request, char *resolved_host) {
    char *host_start = strstr(request, "Host:");
    if (host_start == NULL) {
        return -1;
    }
    host_start += strlen("Host:");

    const char *host_end = strpbrk(host_start, " \r\n:");
    if (host_end == NULL) {
        return -1;
    }

    size_t host_length = host_end - host_start;
    if (host_length <= 0) {
        return -1;
    }
    strncpy(resolved_host, host_start, host_length);
    resolved_host[host_length] = '\0';

    return 0;
}


void *client_handler(void *arg) {
    const client_handler_ctx ctx = *(client_handler_ctx *) arg;
    const int client_socket = ctx.client_socket;
    struct sockaddr_in client_address = ctx.client_address;
    printf(GREEN "\taccepted client from %s:%d\n" RESET, inet_ntoa(client_address.sin_addr),
           ntohs(client_address.sin_port));


    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(client_socket, buffer, BUFFER_SIZE);
    if (bytes_read < 0) {
        perror("error occurred while read");
        close(client_socket);
        return NULL;
    }
    if (bytes_read == 0) {
        perror("connection closed by peer");
        close(client_socket);
        return NULL;
    }
    char* end_of_request = strstr(buffer, "\r\n\r\n");
    if (end_of_request == NULL) {
        strcat(buffer, "\r\n");
        bytes_read += 2;
    }
    printf("read %ld bytes\n", bytes_read);
    buffer[bytes_read] = '\0';

    unsigned char host[DEFAULT_HOST_SIZE];
    if (host_from_request((char *) buffer, (char *) host) != 0) {
        if (host_from_url((char *) buffer, (char *) host) != 0) {
            printf("TID: %d | Error parsing host from request.\n", gettid());
            close(client_socket);
            return NULL;
        }
    }

    struct addrinfo hints, *res0;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // резолвлю имя
    if (-1 == getaddrinfo((char *) host, "http", &hints, &res0)) {
        perror("error occured while resolving domain name");
        close(client_socket);
        return NULL;
    }
    int dest_socket = 0, connected = 0;
    const char *cause = NULL;

    // создаю сокет и подключаюсь
    for (const struct addrinfo *res = res0; res != NULL; res = res->ai_next) {
        if (-1 == (dest_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol))) {
            cause = "socket";
            continue;
        }
        if (-1 == connect(dest_socket, res->ai_addr, res->ai_addrlen)) {
            cause = "connect";
            close(dest_socket);
            continue;
        }
        connected = 1;
        break;
    }
    if (connected == 0) {
        if (0 == strcmp(cause, "socket")) {
            perror("error occured while creating dest socket");
            close(client_socket);
            freeaddrinfo(res0);
            return NULL;
        }
        if (0 == strcmp(cause, "connect")) {
            perror("error occured while connecting to destination host\n");
            close(client_socket);
            freeaddrinfo(res0);
            return NULL;
        }
    }
    printf(YELLOW "\t\tconnected to destination\n" RESET);

    int bytes_sent;
    // пересылаю запрос от клиента
    if (-1 == (bytes_sent = write(dest_socket, buffer, bytes_read))) {
        perror("error occured while sending packet to destination");
        close(client_socket);
        close(dest_socket);
        freeaddrinfo(res0);
        return NULL;
    }
    printf(CYAN "\t\tsent %d bytes to destination\n" RESET, bytes_sent);

    // пересылаю ответ
    while ((bytes_read = read(dest_socket, buffer, BUFFER_SIZE)) > 0) {
        printf(YELLOW "\t\tread %ld bytes from destination\n" RESET, bytes_read);
        if ((bytes_sent = write(client_socket, buffer, bytes_read)) < 0) {
            perror("error occured while sending data to client");
            close(client_socket);
            close(dest_socket);
            freeaddrinfo(res0);
            return NULL;
        }
        printf(CYAN "\t\tsent %d bytes to client\n" RESET, bytes_sent);
    }
    printf(CYAN "\t\tsend %d bytes to client\n" RESET, bytes_sent);

    freeaddrinfo(res0);
    close(client_socket);
    close(dest_socket);
    printf(GREEN "\tclosed connection %s:%d\n" RESET, inet_ntoa(client_address.sin_addr),
           ntohs(client_address.sin_port));
    return NULL;
}


int main() {
    const int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == server_socket) {
        perror("socket initialization failed\n");
        return EXIT_FAILURE;
    }

    printf("Type in port number\n");
    int port;
    scanf("%d", &port);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_UNSPEC;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    if (-1 == bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr))) {
        perror("bind failed");
        close(server_socket);
        return EXIT_FAILURE;
    }

    if (-1 == listen(server_socket, CLIENTS_LIMIT)) {
        perror("listen failed");
        close(server_socket);
        return EXIT_FAILURE;
    }
    printf(PURPLE "server started on port: %d\n", port);
    printf(PURPLE "server listening for %d clients max\n", CLIENTS_LIMIT);
    fflush(stdout);

    while (1) {
        int client_socket;
        struct sockaddr_in client_address;
        socklen_t client_addr_size = sizeof(client_address);
        if (-1 == (client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_addr_size))) {
            perror("accept failed");
            close(server_socket);
            return EXIT_FAILURE;
        }

        fflush(stdout);

        pthread_t handler_thread;
        pthread_attr_t attr;
        if (-1 == pthread_attr_init(&attr)) {
            perror("pthread_attr_init failed");
            close(server_socket);
            close(client_socket);
            return EXIT_FAILURE;
        }
        if (-1 == pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) {
            perror("failed to set detached state");
            close(server_socket);
            close(client_socket);
            return EXIT_FAILURE;
        }
        client_handler_ctx ctx = {client_socket, client_address};
        if (-1 == pthread_create(&handler_thread, &attr, &client_handler, &ctx)) {
            perror("failed to create thread");
            close(server_socket);
            close(client_socket);
            return EXIT_SUCCESS;
        }
    }
}

