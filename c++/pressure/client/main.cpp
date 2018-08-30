/* wfh - 2018/8/28
 * epoll for multi clients to pressure.
 * tcpdump -i lo port 8333
 */

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define ASYNC_CONNECT  1
#define TEST_SEND_DATA "test1234567"

int SetSocketNoBlock(int iFd) {
    return fcntl(iFd, F_SETFL, fcntl(iFd, F_GETFL) | O_NONBLOCK);
}

int EpollAddEvent(int iEpollFd, int iFd) {
    printf("epoll add event, epoll fd = %d, socket fd = %d\n", iEpollFd, iFd);

    epoll_event tEvent;
    tEvent.data.fd = iFd;
    tEvent.events = EPOLLIN | EPOLLET | EPOLLOUT;
    if (epoll_ctl(iEpollFd, EPOLL_CTL_ADD, iFd, &tEvent) < 0) {
        printf("epoll %d ctrl add fd %d failed!\n", iEpollFd, iFd);
        return -1;
    }

    SetSocketNoBlock(iFd);
    return 0;
}

int EpollModEvent(int iEpollFd, int iFd, int iEvent) {
    printf("epoll mod event, epoll fd: %d, socket fd: %d, event: %d\n",
           iEpollFd, iFd, iEvent);

    epoll_event tEvent;
    tEvent.data.fd = iFd;
    tEvent.events = EPOLLET | EPOLLERR | iEvent;
    if (epoll_ctl(iEpollFd, EPOLL_CTL_MOD, iFd, &tEvent) < 0) {
        printf("ctl mod epfd failed! error: %d\n", errno);
        return -1;
    }

    SetSocketNoBlock(iFd);
    return 0;
}

int EpollDelEvent(int iEpollFd, int iFd) {
    printf("epoll delete event! epoll fd = %d, socket fd = %d\n", iEpollFd, iFd);
    return epoll_ctl(iEpollFd, EPOLL_CTL_DEL, iFd, 0);
}

void CloseConn(int iEpollFd, int iFd) {
    EpollDelEvent(iEpollFd, iFd);
    close(iFd);
}

int WriteData(int iFd, const char* pBuffer, int iLen) {
    printf("write %d bytes to socket = %d \n", iLen, iFd);
    if (iFd < 0 || NULL == pBuffer || 0 == iLen) {
        return -1;
    }

    int iWriteLen = 0;
    const char* pBufferIndex = pBuffer;

    while (iLen > 0) {
        iWriteLen = send(iFd, pBufferIndex, iLen, MSG_NOSIGNAL);
        if (iWriteLen < 0) {
            return -1;
        }

        iLen -= iWriteLen;
        pBufferIndex += iWriteLen;
    }

    return iWriteLen;
}

bool ReadData(int iFd, char* pBuffer, int iLen) {
    memset(pBuffer, 0, iLen);

    int iReadLen = recv(iFd, pBuffer, iLen, 0);
    if (iReadLen < 0) {
        return false;
    }

    if (0 == iReadLen) {
        return false;
    }

    printf("read data from socket = %d, buffer = %s, len = %d\n", iFd, pBuffer, iLen);
    return true;
}

void StartConn(int iEpollFd, int iNum, const char* pIP, int iPort) {
    printf("start conn count = %d, ip = %s, port = %d\n", iNum, pIP, iPort);

    struct sockaddr_in tAddr;
    bzero(&tAddr, sizeof(tAddr));
    tAddr.sin_family = AF_INET;
    tAddr.sin_port = htons(iPort);
    inet_pton(AF_INET, pIP, &tAddr.sin_addr.s_addr);

    for (int i = 0; i < iNum; i++) {
        usleep(10 * 1000);

        int iFd = socket(PF_INET, SOCK_STREAM, 0);
        if (iFd < 0) {
            printf("socket error: %d\n", errno);
            continue;
        }
        printf("create socket success, fd = %d\n", iFd);

#ifdef ASYNC_CONNECT
        EpollAddEvent(iEpollFd, iFd);
        connect(iFd, (struct sockaddr*)&tAddr, sizeof(tAddr));
#else
        if (connect(iFd, (struct sockaddr*)&tAddr, sizeof(tAddr)) < 0) {
            printf("connect addr faild! \n");
            continue;
        }
        printf("create connect success, fd = %d!\n", iFd);
        WriteData(iFd, TEST_SEND_DATA, strlen(TEST_SEND_DATA));
#endif
    }
}

int EpollRun(int iConns, const char* pIP, int iPort) {
    int iEpollFd = epoll_create(64);
    if (iEpollFd < 0) {
        printf("create epoll fd failed! err = %d", errno);
        return -1;
    }

    StartConn(iEpollFd, iConns, pIP, iPort);

    char szBuffer[1024] = {0};
    struct epoll_event arrEvents[10000];

    while (1) {
        int iEventCount = epoll_wait(iEpollFd, arrEvents, 10000, 2000);
        for (int i = 0; i < iEventCount; i++) {
            usleep(10 * 1000);

            int iFd = arrEvents[i].data.fd;
            int iEvents = arrEvents[i].events;
            if (iFd < 0) {
                printf("recv data error = %d, fd = %d", errno, iFd);
                continue;
            }

            if (iEvents & EPOLLIN) {
                printf("read data from EPOLLIN\n");
                if (!ReadData(iFd, szBuffer, sizeof(szBuffer) - 1)) {
                    CloseConn(iEpollFd, iFd);
                    continue;
                }
                EpollModEvent(iEpollFd, iFd, EPOLLOUT);
            } else if (iEvents & EPOLLOUT) {
                printf("read data from EPOLLOUT\n");
                if (WriteData(iFd, TEST_SEND_DATA, strlen(TEST_SEND_DATA)) < 0) {
                    CloseConn(iEpollFd, iFd);
                    continue;
                }
                EpollModEvent(iEpollFd, iFd, EPOLLIN);
            } else if (iEvents & EPOLLERR) {
                printf("read data from EPOLLERR\n");
                CloseConn(iEpollFd, iFd);
                continue;
            }
        }
    }

    return 0;
}

//./main 100 127.0.0.1 8333
int main(int iArgs, char** ppArgv) {
    if (iArgs != 4) {
        printf("invalid arg! \n");
        return 1;
    }

    int iConns = atoi(ppArgv[1]);
    int iPort = atoi(ppArgv[3]);
    std::string strIP(ppArgv[2]);
    printf("conns = % d, ip = %s, port = % d\n", iConns, strIP.c_str(), iPort);

    EpollRun(iConns, strIP.c_str(), iPort);
    return 0;
}
