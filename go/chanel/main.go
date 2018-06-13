package main

import (
	"fmt"
	"io"
	"net/http"
)

type DataPasser struct {
	logs chan string
}

func (p *DataPasser) handleHello(w http.ResponseWriter, r *http.Request) {
	p.logs <- r.URL.String()
	io.WriteString(w, "hello world")
}

func (p *DataPasser) log() {
	for item := range p.logs {
		fmt.Println("2.Item", item)
	}
}

func main() {
	fmt.Println("init....")
	passer := &DataPasser{logs: make(chan string)}
	go passer.log()

	http.HandleFunc("/2", passer.handleHello)
	http.ListenAndServe(":9999", nil)
}
