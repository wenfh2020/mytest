package main

import (
	"fmt"
	"os"
	"reflect"
	"strings"
	"sync"
	"time"
)

type ObjectType uint

const (
	Invalid ObjectType = iota
	Value
	Hash
	Array
)

func mygo(ch chan<- struct{}) {
	ch <- struct{}{} //signal.

	fmt.Println("mygo...")
	for i := 0; i < 100; i++ {
		fmt.Println(i)
	}
}

func main() {
	/*
			done := make(chan struct{})

			fmt.Println("start...")
			go mygo(done)
			fmt.Println("end...")

			<-done
			fmt.Println("end2...")

			close(done)

			ch := make(chan bool)
			go func() {
				fmt.Println("hello world")
				ch <- true
				close(ch)
			}()

			for v := range ch {
				fmt.Println(v)
			}

			fmt.Println("end")
			//<-ch

		runtime.GOMAXPROCS(runtime.NumCPU())

		wg := sync.WaitGroup{}
		wg.Add(10)

		for i := 0; i < 10; i++ {
			go Go(&wg, i)
		}

		wg.Wait()

		fmt.Println(getName()) */

	//	var test uint32
	/*
		test := 4294967295
		fmt.Println(test)
		fmt.Println(reflect.TypeOf(test))
		a := 429496729611
		fmt.Println(reflect.TypeOf(a))*/

	for i := 0; i < 42949672961; i++ {
		if reflect.ValueOf(i).Kind() == reflect.Uint32 {
			fmt.Println(i)
		}
	}

	fmt.Println("end....")
}

func tracefile(str_content string) {
	fd, _ := os.OpenFile("a.txt", os.O_RDWR|os.O_CREATE|os.O_APPEND, 0644)
	fd_time := time.Now().Format("2006-01-02 15:04:05")
	fd_content := strings.Join([]string{"======", fd_time, "=====", str_content, "\n"}, "")
	buf := []byte(fd_content)
	fd.Write(buf)
	fd.Close()
}

func getType(data interface{}) ObjectType {
	v := reflect.ValueOf(data)
	switch v.Kind() {
	case reflect.String, reflect.Bool, reflect.Int, reflect.Int8, reflect.Int16, reflect.Int32, reflect.Int64, reflect.Uint, reflect.Uint8, reflect.Uint16, reflect.Uint32, reflect.Uint64, reflect.Uintptr, reflect.Float32, reflect.Float64, reflect.Complex64, reflect.Complex128:
		return Value
	case reflect.Slice, reflect.Array:
		return Array
	case reflect.Map:
		return Hash
	default:
		return Invalid

	}
}

func getName() (firstName, secondName string) {
	return "Hello", "world"
}

func Go(wg *sync.WaitGroup, index int) {
	a := 1
	for i := 0; i < 100000000; i++ {
		a += 1
	}

	fmt.Println(index, a)
	wg.Done()
}
