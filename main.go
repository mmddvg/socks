package main

/*
#cgo CFLAGS: -I./c
#cgo LDFLAGS: ./socks.o ./sockets.o
#include "socks.h"
*/
import "C"

func main() {
	C.hello()
}
