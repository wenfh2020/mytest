package main

import (
	"bufio"
	"encoding/binary"
	"encoding/json"
	"fmt"
	"net"
	"time"

	log "github.com/thinkboy/log4go"
)

type ReqRelationUser struct {
	MsgFlag      string `json:"msg_flag"`
	RelationType uint32 `json:"relation_type"`
	UserID       uint32 `json:"userId"`
	ObjectID     uint32 `json:"object_id"`
}

type AckRelationUser struct {
	Code    int    `json:"code"`
	Info    string `json:"info"`
	MsgFlag string `json:"msg_flag"`
}

func initTCP() {
	log.Trace("initTcp")
	log.Debug(Conf.TCPAddr)

	conn, err := net.Dial("tcp", Conf.TCPAddr)
	if err != nil {
		fmt.Println("net.Dial(\"%s\") error(%v)", Conf.TCPAddr, err)
		log.Error("net.Dial(\"%s\") error(%v)", Conf.TCPAddr, err)
		return
	}

	log.Trace(Conf.TCPAddr)

	seqId := int32(0)
	wr := bufio.NewWriter(conn)
	rd := bufio.NewReader(conn)
	proto := new(Proto)
	proto.Ver = 1

	// writer
	go func() {
		proto1 := new(Proto)

		proto1.Ver = 1
		for {
			var emptyJSONBody = []byte("{}")
			oRlatUser := ReqRelationUser{"123", 2, 11, 22}
			if b, err := json.Marshal(oRlatUser); err == nil {
				proto1.Body = b
			} else {
				proto1.Body = emptyJSONBody
			}
			// relation user.
			log.Debug("relation user...")
			proto1.Cmd = OP_TEST_REATION_USER
			proto1.SeqId = seqId
			//proto1.Body = nil
			if err = tcpWriteProto(wr, proto1); err != nil {
				log.Error("tcpWriteProto() error(%v)", err)
				return
			}
			seqId++
			time.Sleep(10000 * time.Millisecond)
		}
	}()
	// reader
	for {
		log.Debug("tcpReadProto")

		if err = tcpReadProto(rd, proto); err != nil {
			log.Error("tcpReadProto() error(%v)", err)
			return
		}

		if proto.Cmd == OP_TEST_REATION_USER_REPLY {
			log.Debug("ack relation user-----")
			var oObj AckRelationUser
			body, _ := proto.Body.MarshalJSON()
			if err := json.Unmarshal(body, &oObj); err != nil {
				log.Error(err)
			}

			log.Debug("body = %v", oObj)
		} else if proto.Cmd == OP_HEARTBEAT_REPLY {
			log.Debug("receive heartbeat")
			if err = conn.SetReadDeadline(time.Now().Add(25 * time.Second)); err != nil {
				log.Error("conn.SetReadDeadline() error(%v)", err)
				return
			}
		} else if proto.Cmd == OP_TEST_REPLY {
			log.Debug("body: %s", string(proto.Body))
		} else if proto.Cmd == OP_SEND_SMS_REPLY {
			log.Debug("body: %s", string(proto.Body))
		}
	}
}

func tcpWriteProto(wr *bufio.Writer, proto *Proto) (err error) {
	// write
	log.Debug("packet len = %d", uint32(rawHeaderLen)+uint32(len(proto.Body)))
	if err = binary.Write(wr, binary.BigEndian, uint32(rawHeaderLen)+uint32(len(proto.Body))); err != nil {
		return
	}
	//if err = binary.Write(wr, binary.BigEndian, rawHeaderLen); err != nil {
	//	return
	//}

	log.Debug("ver = %d", proto.Ver)
	if err = binary.Write(wr, binary.BigEndian, proto.Ver); err != nil {
		return
	}

	log.Debug("oper = %d", proto.Cmd)
	if err = binary.Write(wr, binary.BigEndian, proto.Cmd); err != nil {
		return
	}

	log.Debug("seqid = %d", proto.SeqId)
	if err = binary.Write(wr, binary.BigEndian, proto.SeqId); err != nil {
		return
	}
	if proto.Body != nil {
		log.Debug("cipher body: %v", proto.Body)
		if err = binary.Write(wr, binary.BigEndian, proto.Body); err != nil {
			return
		}
	}
	err = wr.Flush()
	return
}

func tcpReadProto(rd *bufio.Reader, proto *Proto) (err error) {
	var (
		packLen int32
	)
	// read
	if err = binary.Read(rd, binary.BigEndian, &packLen); err != nil {
		return
	}

	log.Debug("packLen: %d", packLen)
	if err = binary.Read(rd, binary.BigEndian, &headerLen); err != nil {
		return
	}
	log.Debug("headerLen: %d", headerLen)

	if err = binary.Read(rd, binary.BigEndian, &proto.Ver); err != nil {
		return
	}
	log.Debug("ver: %d", proto.Ver)
	if err = binary.Read(rd, binary.BigEndian, &proto.Cmd); err != nil {
		return
	}
	log.Debug("operation: %d", proto.Cmd)
	if err = binary.Read(rd, binary.BigEndian, &proto.SeqId); err != nil {
		return
	}
	log.Debug("seqId: %d", proto.SeqId)
	var (
		n = int(0)
		t = int(0)
		//	bodyLen = int(packLen - int32(headerLen))
		bodyLen = int(packLen - int32(rawHeaderLen))
	)
	log.Debug("read body len: %d", bodyLen)
	if bodyLen > 0 {
		proto.Body = make([]byte, bodyLen)
		for {
			if t, err = rd.Read(proto.Body[n:]); err != nil {
				return
			}
			if n += t; n == bodyLen {
				break
			} else if n < bodyLen {
			} else {
			}
		}
	} else {
		proto.Body = nil
	}
	return
}
