#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif
#include "hiredis/hiredis.h"
#ifdef __cplusplus
}
#endif

int main() {
    /*
    char* cmd;
    int len;
    len = redisFormatCommand(&cmd, "SET foo bar");
    std::cout <<cmd <<std::endl;
    */

    redisContext* pConn = redisConnect("127.0.0.1", 6379);
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
