//from: http://chenzhenianqing.com/articles/1051.html
//

#include <stdio.h>
#include <ev.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#define PORT 8333
#define BUFFER_SIZE 64

void accept_cb(struct ev_loop* loop, struct ev_io* watcher, int revents);
void read_cb(struct ev_loop* loop, struct ev_io* watcher, int revents);

int main() {
    int sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        printf("socket error. errno: %d \n", errno);
        return -1;
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    int reuse = 1;
    int timeout = 5;
    ::setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ::setsockopt(sd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &timeout, sizeof(timeout));
    if (bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        printf("bing error. errno: %d \n", errno);
        return -1;
    }

    if (listen(sd, 2) < 0) {
        printf("listen error \n");
        return -1;
    }

    struct ev_io socket_watcher;
    struct ev_loop* loop = EV_DEFAULT;
    ev_io_init(&socket_watcher, accept_cb, sd, EV_READ);
    ev_io_start(loop, &socket_watcher);
    ev_run(loop, 0);
    return 0;
}

void accept_cb(struct ev_loop* loop, struct ev_io* watcher, int revents) {
    if (revents & EV_ERROR) {
        printf("error event in accept.\n");
        return;
    }

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sd = accept(watcher->fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_sd < 0) {
        printf("accept error\n");
        return;
    }

    printf("someone connected.\n");
    struct ev_io* w_client = (struct ev_io*)malloc(sizeof(struct ev_io));
    ev_io_init(w_client, read_cb, client_sd, EV_READ);
    ev_io_start(loop, w_client);
}

void read_cb(struct ev_loop* loop, struct ev_io* watcher, int revents) {
    if (EV_ERROR & revents) {
        printf("error event in read\n");
        return;
    }

    char szBuffer[BUFFER_SIZE] = {0};
    ssize_t read = recv(watcher->fd, szBuffer, BUFFER_SIZE, 0);
    if (read < 0) {
        printf("read event error, errno:%d\n", errno);
        return;
    }

    if (0 == read) {
        printf("some on disconnect, errno = %d.\n", errno);
        ev_io_stop(loop, watcher);
        free(watcher);
        return;
    }

    printf("get message = %s", szBuffer);
    send(watcher->fd, szBuffer, read, 0);
    bzero(szBuffer, read);
}
