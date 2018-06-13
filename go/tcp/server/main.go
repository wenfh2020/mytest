package main

import (
	"bufio"
	"fmt"
	"net"
	"time"
)

func main() {
	fmt.Println("begin.....\n")

	var pTcpAddr *net.TCPAddr
	pTcpAddr, _ = net.ResolveTCPAddr("tcp", "127.0.0.1:8080")
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

	pReader := bufio.NewReader(pConn)

	for {
		strMessage, err := pReader.ReadString('\n')
		if err != nil {
			return
		}

		fmt.Println(string(strMessage))
		strMsg := time.Now().String() + "\n"
		szMessage := []byte(strMsg)
		pConn.Write(szMessage)
	}
}
