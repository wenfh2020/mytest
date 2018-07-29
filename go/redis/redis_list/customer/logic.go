package main

import (
	"encoding/json"
	"fmt"
	"time"

	"github.com/gomodule/redigo/redis"
)

const (
	WAIT_TIME   = 1 //second
	TIME_FOMAT  = "2006-01-02 15:04:05.000"
	REDIS_ADDR  = "127.0.0.1:6379"
	MESSAGE_KEY = "redislist"
)

type ReqNoticeFriend struct {
	MsgFlag      string `json:"msg_flag"`
	RelationType uint32 `json:"relation_type"`
	UserID       uint32 `json:"userId"`
	ObjectID     uint32 `json:"object_id"`
	Message      string `json:"message"`
	DataVer      string `json:"data_ver"`
}

func logic(p *Proto) (err error) {
	var oReq ReqNoticeFriend
	if err = json.Unmarshal(p.Body, &oReq); err != nil {
		fmt.Println(err)
		return
	}

	//fmt.Println("[", time.Now().Format(TIME_FOMAT), "]", oReq, oReq.DataVer)
	return
}

func Custom() {
	c, err := redis.Dial("tcp", REDIS_ADDR)
	if err != nil {
		fmt.Println(err)
		return
	}

	defer c.Close()
	bStart := false
	iMessagesCount := 0
	oOldTime := time.Now()

	for {
		vals, err := redis.Values(c.Do("brpop", MESSAGE_KEY, WAIT_TIME))
		if err != nil {
			if bStart {
				fmt.Println("end time:", time.Now().Format(TIME_FOMAT))
				fmt.Println("custom message:", iMessagesCount)
				fmt.Println("avg:", iMessagesCount/int(time.Now().Sub(oOldTime).Seconds()))
				bStart = false
			}
			time.Sleep(3 * time.Second)
			continue
		}

		if !bStart {
			oOldTime := time.Now()
			fmt.Println("begin time:", oOldTime.Format(TIME_FOMAT))
			bStart = true
		}

		for i, v := range vals {
			if i != 0 {
				iMessagesCount = iMessagesCount + 1
				//fmt.Println(string(v.([]byte)))
				p := new(Proto)
				if err = UnPack(v.([]byte), p); err != nil {
					fmt.Println("unpacket failed!")
					return
				}

				//处理消息逻辑
				if err = logic(p); err != nil {
					fmt.Println("logic failed!")
				} else {
					//success remove doing
				}
			}
		}
	}
}
