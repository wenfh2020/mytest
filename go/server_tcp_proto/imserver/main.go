package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"net"
)

const (
	CMD_REQ_NOTICE_FRIEND = int32(1001)
	CMD_ACK_NOTICE_FRIEND = int32(1002)

	CMD_REQ_NOTICE_GROUP = int32(1003)
	CMD_ACK_NOTICE_GROUP = int32(1004)

	CMD_REQ_NOTICE_GROUP_ROLE = int32(1005)
	CMD_ACK_NOTICE_GROUP_ROLE = int32(1006)

	CMD_REQ_NOTICE_RELAY_SERVER = int32(4109)
	CMD_ACK_NOTICE_RELAY_SERVER = int32(4110)
)

type AckNotice struct {
	Code    int    `json:"code"`
	Info    string `json:"info"`
	MsgFlag string `json:"msg_flag"`
}

const (
	strLocalAddr = "127.0.0.1:8080"
)

func main() {
	fmt.Println("begin......, server =", strLocalAddr)

	var pTcpAddr *net.TCPAddr
	pTcpAddr, _ = net.ResolveTCPAddr("tcp", strLocalAddr)
	pTcpListerner, _ := net.ListenTCP("tcp", pTcpAddr)

	defer pTcpListerner.Close()

	for {
		pTcpConn, err := pTcpListerner.AcceptTCP()
		if err != nil {
			continue
		}

		fmt.Println("a client connect: " + pTcpConn.RemoteAddr().String())
		go tcpPipe(pTcpConn)
	}

	fmt.Println("end.....\n")
}

func tcpPipe(pConn *net.TCPConn) {
	strRemoteAddr := pConn.RemoteAddr().String()
	defer func() {
		fmt.Println("disconnect :" + strRemoteAddr)
		pConn.Close()
	}()

	var err error
	proto := new(Proto)
	pReader := bufio.NewReader(pConn)
	pWriter := bufio.NewWriter(pConn)

	for {
		if err = tcpReadProto(pReader, proto); err != nil {
			fmt.Println(err)
			return
		}
		fmt.Println(string(proto.Body))

		/*
			dst := new(bytes.Buffer)
			json.Indent(dst, proto.Body, "", "    ")
			fmt.Println(dst)
		*/

		pProtoWrite := new(Proto)
		pProtoWrite.Ver = proto.Ver
		pProtoWrite.SeqId = proto.SeqId
		Dispatch(pWriter, pProtoWrite)
	}
}

func Dispatch(w *bufio.Writer, p *Proto) (err error) {
	if p.Cmd == CMD_REQ_NOTICE_FRIEND {
		fmt.Println("friend notice-------")

		p.Cmd = CMD_ACK_NOTICE_FRIEND
		oAckRlatUser := AckNotice{0, "ok", "friend notice"}
		SendAck(w, p, oAckRlatUser)
	} else if p.Cmd == CMD_REQ_NOTICE_RELAY_SERVER {
		fmt.Println("relay server notice-------")

		p.Cmd = CMD_ACK_NOTICE_RELAY_SERVER
		oAckRlatUser := AckNotice{0, "ok", "relay"}
		SendAck(w, p, oAckRlatUser)
	} else if p.Cmd == CMD_REQ_NOTICE_GROUP {
		fmt.Println("group notice-------")

		p.Cmd = CMD_ACK_NOTICE_GROUP
		oAckRlatUser := AckNotice{0, "ok", "group notice"}
		SendAck(w, p, oAckRlatUser)
	}

	return
}

func SendAck(wr *bufio.Writer, p *Proto, oObj interface{}) (err error) {
	fmt.Println("tcpSendAck()")

	if p.Body, err = json.Marshal(oObj); err != nil {
		fmt.Println(err)
		p.Body = []byte("{}")
	}

	if err = tcpWriteProto(wr, p); err != nil {
		fmt.Println("tcpSendAck() error ", err)
		return
	}
	return
}
