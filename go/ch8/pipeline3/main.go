package main

import "fmt"

func counter(out chan<- int) {
	for x := 0; x < 100; x++ {
		fmt.Println("counter: %d", x)
		out <- x
	}

	close(out)
}

func squarer(out chan<- int, in <-chan int) {
	for v := range in {
		fmt.Println("squarer: %d", v)
		out <- v * v
	}

	close(out)
}

func printer(in <-chan int) {
	for v := range in {
		fmt.Println("printer: %d", v)
	}
}

func main() {
	naturals := make(chan int)
	squares := make(chan int)

	go counter(naturals)
	go squarer(squares, naturals)
	printer(squares)
}
