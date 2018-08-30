/* wfh - 2018/8/28
 * simple socket client.
 * from: https://blog.csdn.net/u011068702/article/details/56678997
*/

#include<stdio.h>
#include <iostream>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>
#include<errno.h>
#include<string.h>
#include<arpa/inet.h>

//./main ip port
int main(int iArgs, char** ppArgv) {
    if (iArgs != 3) {
        printf("invalid arg!\n");
        return 1;
    }

    int iPort = atoi(ppArgv[2]);
    std::string strIp(ppArgv[1]);

    int iSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == iSockFd) {
        printf("socket error = %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in tAddr;
    bzero(&tAddr, sizeof(tAddr));
    tAddr.sin_family = AF_INET;
    tAddr.sin_port = htons(iPort);
    inet_pton(AF_INET, strIp.c_str(), &tAddr.sin_addr.s_addr);

    if (connect(iSockFd, (struct sockaddr*)(&tAddr), sizeof(struct sockaddr)) == -1) {
        printf("connect failed\n");
        exit(EXIT_FAILURE);
    }

    char szSendBuf[1024] = {0};
    char szRecvBuf[2014] = {0};

    while (1) {
        fgets(szSendBuf, sizeof(szSendBuf), stdin);
        send(iSockFd, szSendBuf, strlen(szSendBuf), 0);
        if (strcmp(szSendBuf, "exit\n") == 0) {
            break;
        }

        recv(iSockFd, szRecvBuf, sizeof(szRecvBuf), 0);
        fputs(szRecvBuf, stdout);
        memset(szSendBuf, 0, sizeof(szSendBuf));
        memset(szRecvBuf, 0, sizeof(szRecvBuf));
    }

    close(iSockFd);
    exit(EXIT_SUCCESS);
    return 0;
}
