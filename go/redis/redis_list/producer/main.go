package main

import (
	"fmt"
	"time"
)

const (
	MESSAGE_NUM = 100000                    //测试的消息数量
	TIME_FOMAT  = "2006-01-02 15:04:05.000" //打印时间格式
	REDIS_ADDR  = "127.0.0.1:6379"          //redis 的ip地址和端口
	MAX_GO_CHAN = 10000                     //根据硬件的配置设置最大并发个数
)

/*
chMaxGo 为了控制协程的并发数量，
压力测试一下子起来1,000,000个协程，
配置低的机器瞬间把资源耗尽，进程被强制 kill,
可根据压测机器硬件情况，配置队列长度

例如 2g 内存的机器，并发 100,000 个协程，内存一下子被耗尽，
系统会触发保护机制，把用户进程给 kill 掉
*/
var chMaxGo chan int

func Count(ch chan int) {
	ch <- 1
	ch <- 2
	ch <- 3
	ch <- 4
	ch <- 5
	fmt.Println("counting...", len(ch))
}

func main() {
	/*
		chs := make([]chan int, 10)

		fmt.Println("main len", len(chs))

		for i := 0; i < 10; i++ {
			chs[i] = make(chan int, 10)
			go Count(chs[i])
		}

		//fmt.Println(chs)

		for _, ch := range chs {
			<-ch
			//fmt.Println("result", ch)
		}

		return
	*/
	InitRedis(REDIS_ADDR)
	chMaxGo = make(chan int, MAX_GO_CHAN)

	oOldTime := time.Now()
	fmt.Println("begin time:", oOldTime.Format(TIME_FOMAT))
	fmt.Println("running...")

	for i := 0; i < MESSAGE_NUM; i++ {
		oSynWait.Add(1)
		chMaxGo <- 1

		go testLogic(i)
	}

	oSynWait.Wait()
	close(chMaxGo)

	fmt.Println("end time:", time.Now().Format(TIME_FOMAT))
	fmt.Println("Produce message:", MESSAGE_NUM)
	fmt.Println("avg:", MESSAGE_NUM/time.Now().Sub(oOldTime).Seconds())
}
