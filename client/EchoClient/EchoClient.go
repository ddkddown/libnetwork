package main

import (
	"fmt"
	"net"
	"sync"
)

func routine(wg *sync.WaitGroup) {
	defer wg.Done()

	for i := 0; i < 100; i++ {
		conn, _ := net.Dial("tcp", "0.0.0.0:9677")
		count, _ := conn.Write([]byte("hello, world"))
		t := make([]byte, 20)
		count, _ = conn.Read(t)
		fmt.Println(string(t[0:count]))
		conn.Close()
	}
}

func main() {
	var wg sync.WaitGroup

	for i := 0; i <= 1000; i++ {
		wg.Add(1)
		go routine(&wg)
	}

	wg.Wait()
}
