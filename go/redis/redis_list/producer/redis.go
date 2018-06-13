package main

import (
	"time"

	"github.com/gomodule/redigo/redis"
)

//压力测试 IDLE_COUNT 要和 ACTIVE_COUNT相等，否则会产生很多 TIME_WAIT 状态的 TCP 链接
const (
	IDLE_COUNT   = 2   //连接池空闲个数
	ACTIVE_COUNT = 2   //连接池活动个数
	IDLE_TIMEOUT = 180 //空闲超时时间
)

var (
	REDIS_DB     int
	RedisClients *redis.Pool
)

func GetRedisConn() (conn redis.Conn) {
	return RedisClients.Get()
}

func InitRedis(strHost string) {
	REDIS_DB = 0
	RedisClients = createPool(IDLE_COUNT, ACTIVE_COUNT, IDLE_TIMEOUT, strHost)
}

func createPool(maxIdle, maxActive, idleTimeout int, address string) (obj *redis.Pool) {
	obj = new(redis.Pool)
	obj.MaxIdle = maxIdle
	obj.MaxActive = maxActive
	obj.Wait = true
	obj.IdleTimeout = (time.Duration)(idleTimeout) * time.Second
	obj.Dial = func() (redis.Conn, error) {
		c, err := redis.Dial("tcp", address)
		if err != nil {
			return nil, err
		}
		c.Do("SELECT", REDIS_DB)
		return c, err
	}
	return
}
