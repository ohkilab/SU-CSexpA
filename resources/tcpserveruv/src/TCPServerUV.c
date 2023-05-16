/*
 ============================================================================
 Name        : TCPServerUV.c
 Author      : Yasuhiro Noguchi
 Version     : 0.0.1
 Copyright   : Copyright (C) HEPT Consortium
 Description : TCP Server implemented by libuv
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <uv.h>
#include "libserver.h"

int delayTime = 0;

struct sockaddr_in addr;
#define DEFAULT_BACKLOG 128

bool start(const int portNum) {

    loop = uv_default_loop();

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    uv_ip4_addr("0.0.0.0", portNum, &addr);

    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, on_new_connection);
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return 1;
    }
    return uv_run(loop, UV_RUN_DEFAULT);
}

int main(int argc, char* argv[]) {
	if ( argc < 2 || argc > 3 ) {
		printf("Usage: %s PORT [DELAYTIME]\n", argv[0]);
		exit(-1);
	}
	int port = atoi(argv[1]);
	if ( argc == 3 ) {
		delayTime = atoi(argv[2]);
		fprintf(stderr, "delayTime=%d (usec)\n", delayTime);
	}
	start(port);
	return EXIT_SUCCESS;
}
