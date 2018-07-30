/*
 * wfh
 * 2018-06-15
 * hiredis async test.
 */

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif
#include <hiredis/async.h>
#include <hiredis/hiredis.h>
#include <hiredis/adapters/libev.h>
#ifdef __cplusplus
}
#endif

#define PORT 6379
#define IP   "127.0.0.1"
#define TEST_CMD_COUNT 1000000

int g_iCmdCallback = 0;
unsigned long long g_ullBeginTime = 0;

std::string GetCurrentTime();
unsigned long long GetMicrosecond();

void RdsCbConnect(const redisAsyncContext* pRdsContext, int iStatus);
void RdsCbDisConnect(const redisAsyncContext* pRdsContext, int iStatus);
void RdsCbCmd(redisAsyncContext* pRdsContext, void* pReply, void* pData);

int main() {
    redisAsyncContext* pRdsContext = redisAsyncConnect(IP, PORT);
    if (pRdsContext->err != REDIS_OK) {
        printf("async redis connect failed! err code = %d, err msg = %s\n", pRdsContext->err, pRdsContext->errstr);
        return 1;
    }

    struct ev_loop* pLoop = EV_DEFAULT;
    redisLibevAttach(pLoop, pRdsContext);
    redisAsyncSetConnectCallback(pRdsContext, RdsCbConnect);
    redisAsyncSetDisconnectCallback(pRdsContext, RdsCbDisConnect);
    ev_run(pLoop, 0);
    return 0;
}

unsigned long long GetMicrosecond() {
    timeval stTime;
    gettimeofday(&stTime, NULL);
    return (stTime.tv_sec * 1000000 + stTime.tv_usec);
}

std::string GetCurrentTime() {
    time_t lTime = time(NULL);
    struct tm stTime;
    localtime_r(&lTime, &stTime);
    char szCurrentTime[20] = {0};
    strftime(szCurrentTime, 20, "%Y-%m-%d %H:%M:%S", &stTime);
    return std::string(szCurrentTime);
}

void SendProduceRequest(const redisAsyncContext* pRdsContext) {
    for (int i = 0; i < TEST_CMD_COUNT; i++) {
        size_t iArgSize = 3;
        size_t szArglen[iArgSize];
        const char* ppArg[iArgSize];

        ppArg[0] = "lpush";
        szArglen[0] = strlen("lpush");

        char szKey[64] = {0};
        snprintf(szKey, sizeof(szKey), "redislist_h");
        ppArg[1] = szKey;
        szArglen[1] = strlen(szKey);

        char szValue[1024] = {0};
        snprintf(szValue, sizeof(szValue), "{test 1 1 2 hello world %d}", i);
        ppArg[2] = szValue;
        szArglen[2] = strlen(szValue);
        redisAsyncCommandArgv((redisAsyncContext*)pRdsContext, RdsCbCmd, NULL, iArgSize, ppArg, szArglen);
    }
}

void SendCustomRequest(const redisAsyncContext* pRdsContext) {
    size_t iArgSize = 3;
    size_t szArglen[iArgSize];
    const char* ppArg[iArgSize];

    //brpop redislist_h 10
    ppArg[0] = "brpop";
    szArglen[0] = strlen("brpop");

    ppArg[1] = "redislist_h";
    szArglen[1] = strlen("redislist_h");

    ppArg[2] = "10";
    szArglen[2] = strlen("10");
    redisAsyncCommandArgv((redisAsyncContext*)pRdsContext, RdsCbCmd, NULL, iArgSize, ppArg, szArglen);
}

void SendSetRequest(const redisAsyncContext* pRdsContext) {
    for (int i = 0; i < TEST_CMD_COUNT; i++) {
        size_t iArgSize = 3;
        size_t szArglen[iArgSize];
        const char* ppArg[iArgSize];
        char szTestInfo[64] = {0};
        memset(szTestInfo, 'a', sizeof(szTestInfo));

        ppArg[0] = "SET";
        szArglen[0] = strlen("SET");

        char szKey[64] = {0};
        snprintf(szKey, sizeof(szKey), "key-%d", i);
        ppArg[1] = szKey;
        szArglen[1] = strlen(szKey);

        char szValue[1024] = {0};
        snprintf(szValue, sizeof(szValue), "value-%d-%s", i, szTestInfo);
        ppArg[2] = szValue;
        szArglen[2] = strlen(szValue);
        redisAsyncCommandArgv((redisAsyncContext*)pRdsContext, RdsCbCmd, NULL, iArgSize, ppArg, szArglen);
    }
}

void RdsCbConnect(const redisAsyncContext* pRdsContext, int iStatus) {
    printf("connect call back, status = %d \n", iStatus);
    printf("test write cmd count = %d\n", TEST_CMD_COUNT);

    g_ullBeginTime = GetMicrosecond();
    printf("test begin time: %s, %llu\n", GetCurrentTime().c_str(), g_ullBeginTime);

    //SendProduceRequest(pRdsContext);
    SendSetRequest(pRdsContext);
    //SendCustomRequest(pRdsContext);
}

void RdsCbDisConnect(const redisAsyncContext* pRdsContext, int iStatus) {
    printf("disconnect call back, status = %d \n", iStatus);
}

void RdsCbCmd(redisAsyncContext* pRdsContext, void* pReply, void* pData) {
    //printf("redis cmd call back, err code = %d, err msg = %s!\n", pRdsContext->err, pRdsContext->errstr);
    if (NULL == pReply) {
        printf("call back repplay null!\n");
        //SendCustomRequest(pRdsContext);
        return;
    }

    redisReply* pRdsReply = (redisReply*)pReply;
    //printf("%d, redis reply info: type = %d, string = %s\n", ++g_iCmdCallback, pRdsReply->type, pRdsReply->str);
    if (REDIS_REPLY_NIL == pRdsReply->type) {
        //SendCustomRequest(pRdsContext);
        printf("reply nil\n");
        return;
    }

    //SendCustomRequest(pRdsContext);

    //end.
    if (++g_iCmdCallback >= TEST_CMD_COUNT) {
        unsigned long long ullEndTime = GetMicrosecond();
        printf("test end time: %s, %llu, interval: %llu\n",
               GetCurrentTime().c_str(), ullEndTime, ullEndTime - g_ullBeginTime);

        redisAsyncDisconnect((redisAsyncContext*)pRdsContext);
    }
}
