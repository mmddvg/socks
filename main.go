package main

/*
#cgo CFLAGS: -I./c
#cgo LDFLAGS: ./socks.o ./sockets.o
#include "sockets.h"
#include "socks.h"
#include <stdlib.h>
#include <string.h>

*/
import "C"
import (
	"fmt"
	"log"
	"unsafe"
)

func handleConnection(cfd C.int, inp *C.req) {
	defer C.close(cfd)

	if inp.cd == 1 {
		fmt.Println("connect")

		destfd := C.create_client_socket(inp.dest_ip, inp.dest_port)
		if destfd < 0 {
			log.Println("create_client_socket failed")
			response := C.make_connect_res(91, inp.dest_port, inp.dest_ip)
			C.send(cfd, unsafe.Pointer(&response), C.size_t(unsafe.Sizeof(response)), 0)
			return
		}

		response := C.make_connect_res(90, inp.dest_port, inp.dest_ip)
		C.send(cfd, unsafe.Pointer(&response), C.size_t(unsafe.Sizeof(response)), 0)
		fmt.Println("sent response connect")

		C.bi_proxy(cfd, destfd)
		C.close(destfd)
	} else if inp.cd == 2 {
		fmt.Println("bind")
	} else {
		fmt.Println("invalid cd")
	}
}

func main() {
	var srv C.server_socket

	ret := C.create_server_socket(C.CString("127.0.0.1"), C.PORT, &srv)
	if ret < 0 {
		log.Fatalf("create_server_socket failed")
	}
	if C.listen(srv.sfd, C.QUEUE_LENGTH) == -1 {
		log.Fatalf("listen failed")
	}

	fmt.Printf("listening on port %d\n", C.PORT)

	for {
		fmt.Println("starting new iteration")

		var inp *C.req = (*C.req)(C.malloc(C.sizeof_req))
		C.memset(unsafe.Pointer(inp), 0, C.sizeof_req)

		cfd := C.recv_req(srv.sfd, (*C.struct_sockaddr)(unsafe.Pointer(&srv.sock)), inp)
		if cfd < 0 {
			log.Println("recv_req failed")
			C.free(unsafe.Pointer(inp))
			continue
		}

		fmt.Printf("successfully read data: %d\n", cfd)
		fmt.Printf("UserID: %s\n", C.GoString(&inp.userid[0]))
		fmt.Printf("dest ip : %d\n", C.ntohl(inp.dest_ip))
		fmt.Printf("dest port : %d\n", C.ntohs(inp.dest_port))

		go func(cfd C.int, inp *C.req) {
			defer C.free(unsafe.Pointer(inp))
			handleConnection(cfd, inp)
		}(cfd, inp)
	}
}
