package main

import (
	"fmt"
)

func Produce(szBytes []byte) (err error) {
	pConn := GetRedisConn()
	if pConn.Err() != nil {
		fmt.Println(pConn.Err().Error())
		return
	}
	defer pConn.Close()

	if _, err = pConn.Do("lpush", "redislist", szBytes); err != nil {
		fmt.Println(err.Error())
		return
	}

	return
}

func GetMsg() (err error) {
	pConn := GetRedisConn()
	if pConn.Err() != nil {
		fmt.Println(pConn.Err().Error())
		return
	}
	defer pConn.Close()

	_, err = pConn.Do("get", "1:2:3")
	if err != nil {
		fmt.Println(err.Error())
		return
	}

	//fmt.Println(string(v))
	return
}

func SetMsg() (err error) {
	pConn := GetRedisConn()
	if pConn.Err() != nil {
		fmt.Println(pConn.Err().Error())
		return
	}
	defer pConn.Close()

	_, err = pConn.Do("set", "1:2:3", "GetRedisConnGetRedisConnGetRedisConn")
	if err != nil {
		fmt.Println(err.Error())
		return
	}

	//fmt.Println(string(v))
	return

}
