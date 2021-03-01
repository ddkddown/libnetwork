package main

import (
	"fmt"
	"net"
)

func handleConnection(conn net.Conn) {
	buf := make([]byte, 1024)
	len, err := conn.Read(buf)
	if err != nil {
		fmt.Println("error reading:", err.Error())
	}

	fmt.Println("recv msg:", string(buf[:len]))
	conn.Write(buf)
	conn.Close()
}

func main() {
	ln, err := net.Listen("tcp", ":9677")
	if err != nil {
		fmt.Println("listen 9677 failed!")
		return
	}

	defer ln.Close()
	for {
		conn, err := ln.Accept()
		if err != nil {
			fmt.Println("Accept conn failed!")
			break
		}

		go handleConnection(conn)
	}
}
