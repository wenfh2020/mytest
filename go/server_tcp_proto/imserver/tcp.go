package main

import (
	"bufio"
	"encoding/binary"
	"encoding/json"
	"fmt"
)

const (
	rawHeaderLen = int16(14)
)

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

	fmt.Println("packet len = ", packLen)
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
