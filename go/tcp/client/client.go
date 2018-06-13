package main

import (
	"bufio"
	"fmt"
	"net"
	"time"
)

var quitSemaphore chan bool

func main() {
	fmt.Println("begin.....")

	var pTcpAddr *net.TCPAddr
	pTcpAddr, _ = net.ResolveTCPAddr("tcp", "127.0.0.1:9999")
	pConn, _ := net.DialTCP("tcp", nil, pTcpAddr)

	defer pConn.Close()
	fmt.Println("connected!")
	go onMessageRecived(pConn)

	szMessage := []byte("time\n")
	pConn.Write(szMessage)
	<-quitSemaphore
}

func onMessageRecived(pConn *net.TCPConn) {
	pReader := bufio.NewReader(pConn)

	for {
		strMessage, err := pReader.ReadString('\n')
		fmt.Println(strMessage)
		if err != nil {
			quitSemaphore <- true
			break
		}

		time.Sleep(time.Second)

		szMessage := []byte(strMessage)
		pConn.Write(szMessage)
	}
}
