package main

import (
	"fmt"
	"net"
	"sync"
	"time"
)

var (
	wg    sync.WaitGroup
	slice []net.Conn
)

func routine(wg *sync.WaitGroup) {
	defer wg.Done()

	for i := 0; i < 1000; i++ {
		conn, _ := net.Dial("tcp", "0.0.0.0:9677")
		count, _ := conn.Write([]byte("hello, world"))
		t := make([]byte, 20)
		count, _ = conn.Read(t)
		fmt.Println(string(t[0:count]))
		slice = append(slice, conn)
	}
}

func main() {

	t1 := time.Now()
	for i := 0; i < 100; i++ {
		wg.Add(1)
		go routine(&wg)
	}

	wg.Wait()
	fmt.Println("elapsed: ", time.Since(t1))

	time.Sleep(200 * time.Second)

	for i := 0; i < len(slice); i++ {
		slice[i].Close()
	}

}
