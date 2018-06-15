/*
 * wfh
 * 2018-06-14
 * libev network test.
 * from: http://chenzhenianqing.com/articles/1051.html
 * client: telnet 127.0.0.1 8333
 * tool: tcpdump -i lo port 8333 -w 123.cap
 */

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
#define CLOSE_FD_TIME 3

struct client_info {
    int fd;
};

void timeout_cb(struct ev_loop* loop, ev_timer* w, int revents);
void read_cb(struct ev_loop* loop, struct ev_io* watcher, int revents);
void accept_cb(struct ev_loop* loop, struct ev_io* watcher, int revents);

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
    int timeout = 10;
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

    printf("running...\n");

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

    printf("client connected fd = %d\n", client_sd);

    struct ev_io* w_client = (struct ev_io*)malloc(sizeof(struct ev_io));
    ev_io_init(w_client, read_cb, client_sd, EV_READ);
    ev_io_start(loop, w_client);

    struct client_info* pClientInfo = (client_info*)malloc(sizeof(struct client_info));
    pClientInfo->fd = client_sd;

    printf("start timer...\n");

    ev_timer* timeout_watcher = (struct ev_timer*)malloc(sizeof(struct ev_timer));
    timeout_watcher->data = (void*)pClientInfo;
    ev_timer_init(timeout_watcher, timeout_cb, CLOSE_FD_TIME, 1);
    ev_timer_start(loop, timeout_watcher);
}

void timeout_cb(struct ev_loop* loop, ev_timer* w, int revents) {
    printf("timeout\n");

    //get client fd and then close it.
    struct client_info* pClientInfo = (struct client_info*)w->data;
    printf("close fd = %d\n", pClientInfo->fd);
    close(pClientInfo->fd);
    //
    //shutdown(pClientInfo->fd, SHUT_RDWR);
    //
    //it's ok for client to send data to server. but server can not get read event.
    //shutdown(pClientInfo->fd, SHUT_RD);
    //
    //shutdown(pClientInfo->fd, SHUT_WR);
    free(pClientInfo);

    //stop timer.
    ev_timer_stop(loop, w);
    free(w);
}

void read_cb(struct ev_loop* loop, struct ev_io* watcher, int revents) {
    printf("read_cb\n");
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

    printf("get message = %s\n", szBuffer);
    send(watcher->fd, szBuffer, read, 0);
    bzero(szBuffer, read);
}
