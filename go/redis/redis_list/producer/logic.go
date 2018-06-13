package main

import (
	"fmt"
	"strconv"
	"sync"
	"time"
)

var oSynWait sync.WaitGroup

type ReqNoticeFriend struct {
	MsgFlag      string `json:"msg_flag"`
	RelationType uint32 `json:"relation_type"`
	UserID       uint32 `json:"userId"`
	ObjectID     uint32 `json:"object_id"`
	Message      string `json:"message"`
	DataVer      string `json:"data_ver"`
}

func testReq(iVer int) (szBytes []byte, err error) {
	p := new(Proto)
	p.Ver = 1
	p.SeqId = int32(iVer)
	p.Cmd = 1

	oReq := ReqNoticeFriend{}
	oReq.MsgFlag = "test"
	oReq.RelationType = 1
	oReq.UserID = 1
	oReq.ObjectID = 2
	oReq.Message = "hello world"
	oReq.DataVer = strconv.Itoa(iVer)

	if szBytes, err = Packet(p, oReq); err != nil {
		fmt.Println("packet failed!")
		return
	}

	//unpack
	p2 := new(Proto)
	var oReqAck ReqNoticeFriend
	if err = UnPack(szBytes, p2, &oReqAck); err != nil {
		fmt.Println("unpacket failed!")
		return
	}

	//fmt.Println(oReqAck)
	return
}

func testLogic(iVer int) (err error) {
	defer func() {
		oSynWait.Done()
		time.Sleep(time.Millisecond * 500)
		<-chMaxGo
	}()

	//测试逻辑
	var szBytes []byte
	szBytes, err = testReq(iVer)
	if err != nil {
		fmt.Println(err)
		return
	}

	//生产者生产数据
	Produce(szBytes)
	return
}
