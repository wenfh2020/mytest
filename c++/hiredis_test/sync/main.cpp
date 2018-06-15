/*
 * wfh
 * 2018-06-15
 * hiredis sync test.
 */

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif
#include "hiredis/hiredis.h"
#ifdef __cplusplus
}
#endif

#define PORT 6379
#define IP "127.0.0.1"

int main() {
    /*
    char* cmd;
    int len;
    len = redisFormatCommand(&cmd, "SET foo bar");
    std::cout <<cmd <<std::endl;
    */

    redisContext* pConn = redisConnect(IP, PORT);
    if (pConn != NULL && pConn->err) {
        printf("connection err: %s \n", pConn->errstr);
        return 0;
    }

    redisReply* pReply = NULL;
    pReply = (redisReply*)redisCommand(pConn, "SET %s %s", "foo", "bar");
    freeReplyObject(pReply);

    pReply = (redisReply*)redisCommand(pConn, "GET %s", "foo");
    printf("%s \n", pReply->str);
    freeReplyObject(pReply);

    redisFree(pConn);
    return 1;
}
