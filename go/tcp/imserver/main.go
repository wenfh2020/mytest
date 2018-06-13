package main

import (
	"bufio"
	"encoding/binary"
	"encoding/json"
	"fmt"
	"net"
	"time"
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
	rawHeaderLen = int16(14)
)

type AckRelationUser struct {
	Code    int    `json:"code"`
	Info    string `json:"info"`
	MsgFlag string `json:"msg_flag"`
}

type Proto struct {
	Ver   int16           `json:"ver"`  // protocol version
	Cmd   int32           `json:"cmd"`  // operation for request
	SeqId int32           `json:"seq"`  // sequence number chosen by client
	Body  json.RawMessage `json:"body"` // binary body bytes(json.RawMessage is []byte)
}

func tcpWriteProto(wr *bufio.Writer, proto *Proto) (err error) {
	// write
	fmt.Println("packet len = ", uint32(rawHeaderLen)+uint32(len(proto.Body)))
	if err = binary.Write(wr, binary.BigEndian, uint32(rawHeaderLen)+uint32(len(proto.Body))); err != nil {
		return
	}
	//if err = binary.Write(wr, binary.BigEndian, rawHeaderLen); err != nil {
	//  return
	//}

	fmt.Println("ver = ", proto.Ver)
	if err = binary.Write(wr, binary.BigEndian, proto.Ver); err != nil {
		return
	}

	fmt.Println("oper = ", proto.Cmd)
	if err = binary.Write(wr, binary.BigEndian, proto.Cmd); err != nil {
		return
	}

	fmt.Println("seqid = ", proto.SeqId)
	if err = binary.Write(wr, binary.BigEndian, proto.SeqId); err != nil {
		return
	}
	if proto.Body != nil {
		fmt.Println("cipher body: ", proto.Body)
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
		//headerLen int16
	)
	// read
	if err = binary.Read(rd, binary.BigEndian, &packLen); err != nil {
		return
	}

	fmt.Println("packet len = ", packLen)
	//	fmt.Println("packLen: %d", packLen)
	//	if err = binary.Read(rd, binary.BigEndian, &headerLen); err != nil {
	//		return
	//	}
	//	fmt.Println("headerLen: %d", headerLen)
	if err = binary.Read(rd, binary.BigEndian, &proto.Ver); err != nil {
		return
	}

	fmt.Println("ver = ", proto.Ver)
	//	fmt.Println("ver: %d", proto.Ver)
	if err = binary.Read(rd, binary.BigEndian, &proto.Cmd); err != nil {
		return
	}

	fmt.Println("oper = ", proto.Cmd)
	//	fmt.Println("operation: %d", proto.Cmd)
	if err = binary.Read(rd, binary.BigEndian, &proto.SeqId); err != nil {
		return
	}

	fmt.Println("seqid = ", proto.SeqId)
	//	fmt.Println("seqId: %d", proto.SeqId)
	var (
		n = int(0)
		t = int(0)
		//		bodyLen = int(packLen - int32(headerLen))
		bodyLen = int(packLen - int32(rawHeaderLen))
	)
	//	fmt.Println("read body len: %d", bodyLen)
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

func main() {
	fmt.Println("begin.....")

	var pTcpAddr *net.TCPAddr
	pTcpAddr, _ = net.ResolveTCPAddr("tcp", "127.0.0.1:8080")
	pTcpListerner, _ := net.ListenTCP("tcp", pTcpAddr)

	fmt.Println("127.0.0.1:8080")
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
	fmt.Println("tcpPipe...")

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
		fmt.Println("tcpReadProto")
		if err = tcpReadProto(pReader, proto); err != nil {
			fmt.Println(err)
			//fmt.Println("tcpReadProto() error(%v)", err)
			//log.Error("tcpReadProto() error(%v)", err)
			return
		}

		if proto.Cmd == OP_HEARTBEAT {
			fmt.Println("heartbeat....")
			fmt.Println(string(proto.Body))
		} else if proto.Cmd == OP_TEST_REATION_USER {
			fmt.Println("relation user....")
			fmt.Println(string(proto.Body))

			proto1 := new(Proto)
			proto1.Ver = proto.Ver
			proto1.Cmd = OP_TEST_REATION_USER_REPLY
			proto1.SeqId = proto.SeqId

			var emptyJSONBody = []byte("{}")
			oAckRlatUser := AckRelationUser{0, "ok", "123"}
			if b, err := json.Marshal(oAckRlatUser); err == nil {
				proto1.Body = b
			} else {
				proto1.Body = emptyJSONBody
			}
			// relation user.
			fmt.Println("relation user ack...")
			//proto1.Body = nil
			if err = tcpWriteProto(pWriter, proto1); err != nil {
				fmt.Println("tcpWriteProto() error ", err)
				return
			}
		}

		if proto.Cmd == OP_HEARTBEAT_REPLY {
			fmt.Println("receive heartbeat")
			//	fmt.Println("receive heartbeat")
			if err = pConn.SetReadDeadline(time.Now().Add(25 * time.Second)); err != nil {
				fmt.Println("conn.SetReadDeadline() error")
				//log.Error("conn.SetReadDeadline() error(%v)", err)
				return
			}
		} else if proto.Cmd == OP_TEST_REPLY {
			fmt.Println(string(proto.Body))
			//	fmt.Println("body: %s", string(proto.Body))
		} else if proto.Cmd == OP_SEND_SMS_REPLY {
			fmt.Println(string(proto.Body))
			//	fmt.Println("body: %s", string(proto.Body))
		}
	}
}
