package main

import (
	"encoding/json"

	log "github.com/thinkboy/log4go"
)

const (
	OP_HANDSHARE               = int32(0)
	OP_HANDSHARE_REPLY         = int32(1)
	OP_HEARTBEAT               = int32(2)
	OP_HEARTBEAT_REPLY         = int32(3)
	OP_SEND_SMS                = int32(4)
	OP_SEND_SMS_REPLY          = int32(5)
	OP_DISCONNECT_REPLY        = int32(6)
	OP_AUTH                    = int32(7)
	OP_AUTH_REPLY              = int32(8)
	OP_TEST                    = int32(254)
	OP_TEST_REPLY              = int32(255)
	OP_TEST_REATION_USER       = int32(1001)
	OP_TEST_REATION_USER_REPLY = int32(1002)
)

const (
	//	rawHeaderLen = int16(16)
	rawHeaderLen = int16(14)
)

const (
	ProtoTCP          = 0
	ProtoWebsocket    = 1
	ProtoWebsocketTLS = 2
)

type Proto struct {
	Ver   int16           `json:"ver"`  // protocol version
	Cmd   int32           `json:"cmd"`  // operation for request
	SeqId int32           `json:"seq"`  // sequence number chosen by client
	Body  json.RawMessage `json:"body"` // binary body bytes(json.RawMessage is []byte)
}

func (p *Proto) Print() {
	log.Info("\n-------- proto --------\nver: %d\nop: %d\nseq: %d\nbody: %s\n", p.Ver, p.Cmd, p.SeqId, string(p.Body))
}
