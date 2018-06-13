package main

import (
	"encoding/json"
	"fmt"
)

type Proto struct {
	Ver   int16           `json:"ver"`  // protocol version
	Cmd   int32           `json:"cmd"`  // operation for request
	SeqId int32           `json:"seq"`  // sequence number chosen by client
	Body  json.RawMessage `json:"body"` // binary body bytes(json.RawMessage is []byte)
}

func Packet(p *Proto, pBody interface{}) (szBytes []byte, err error) {
	if p == nil || pBody == nil {
		err = fmt.Errorf("invalid arg")
		return
	}

	//pack body
	p.Body = []byte("{}")
	szBytes, err = json.Marshal(pBody)
	if err != nil {
		fmt.Println(err)
		return
	}
	p.Body = szBytes

	//pack proto
	szBytes, err = json.Marshal(p)
	if err != nil {
		fmt.Println(err)
		return
	}

	return
}

func UnPack(szBytes []byte, p *Proto) (err error) {
	if p == nil || len(szBytes) == 0 {
		err = fmt.Errorf("invalid arg")
		return
	}

	if err = json.Unmarshal(szBytes, p); err != nil {
		fmt.Println(err)
		return
	}

	return
}
