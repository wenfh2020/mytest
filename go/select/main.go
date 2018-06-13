package main

import (
	"fmt"
	"time"
)

func main() {
	fmt.Println("vim-go")

	chann := make(chan int)
	go enqueue(chann)

	for {
		select {
		case v, ok := <-chann:
			if ok {
				fmt.Println(v)
			} else {
				fmt.Println("close")
				return
			}
		default:
			//fmt.Println("waiting.")
		}
	}

	fmt.Println("main end")
}

func enqueue(ch chan int) {
	time.Sleep(3 * time.Second)
	fmt.Println("ch begin")
	ch <- 1
	fmt.Println("ch end")
	close(ch)
	fmt.Println("ch close")
}
