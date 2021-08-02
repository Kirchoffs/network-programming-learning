#include "lib/common.h"

#define MESSAGE_SIZE 102400

void send_data(int sockfd) {
    char *query;
    query = malloc(MESSAGE_SIZE + 1);
    for (int i = 0; i < MESSAGE_SIZE; i++) {
        query[i] = 'a';
    }
    query[MESSAGE_SIZE] = '\0';

    const char *cp;
    cp = query;
    size_t remaining = strlen(query);
    while (remaining) {
        int n_written = send(sockfd, cp, remaining, 0);
        fprintf(stdout, "send into buffer %ld \n", n_written);
        if (n_written <= 0) {
            error(1, errno, "send failed");
            return;
        }
        remaining -= n_written;
        cp += n_written;
    }

    return;
}

int main(int argc, char **argv) {
    int socket_fd;
    struct sockaddr_in server_addr;

    if (argc != 2)
        error(1, 0, "usage: tcpclient <IPaddress>");

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    int connect_rt = connect(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (connect_rt < 0) {
        error(1, errno, "connect failed");
    }
    send_data(socket_fd);
    exit(0);
}