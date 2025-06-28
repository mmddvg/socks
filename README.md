# socks

show case of a socks implementation in C used from Go codebase  

## building C files

build the C files using this command : `gcc -c c/socks.c c/sockets.c  -I./c`

this is better handled via a make file but i'll leave that for later

## use

after running the program you can use curl to test it `curl --socks4 127.0.0.1:9090 http://example.com`

## bind method

bind method is not implement so this program can't be currently used for inbound connections
