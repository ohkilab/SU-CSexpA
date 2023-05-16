/*
 ============================================================================
 Name        : TCPServerLE.c
 Author      : Yasuhiro Noguchi
 Version     : 0.0.1
 Copyright   : Copyright (C) HEPT Consortium
 Description : TCP Server implemented by libevent
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include "libserver.h"

int delayTime = 0;

// SIGINTイベントハンドラ
void sigintHandler(evutil_socket_t fd, short ev, void* arg) {
	// イベントループを終了する
	struct event_base* base = (struct event_base*)arg;
	event_base_loopexit(base, NULL);
}

bool start(const char* portNum) {

	// libeventの初期化
	struct event_base* base = event_base_new();

	// SIGINTのイベントハンドラを登録
	struct event* sig;
	sig = evsignal_new(base, SIGINT, sigintHandler, base);
	if ( evsignal_add(sig, NULL) != 0 ) {
		perror("evsignal_add: sig");
		return false;
	}

	int ssocket = serverTCPSocket(portNum);
	if (ssocket == -1) {
		fprintf(stderr, "server_socket(%s):error\n", portNum);
		return false;
	}
	fprintf(stderr, "ready for accept\n");

	// libeventにサーバソケットを登録
	struct event ev;
	event_assign(&ev, base, ssocket, EV_READ|EV_PERSIST, acceptHandler, base);
	if ( event_add(&ev, NULL) != 0 ) {
		perror("event_add: ev");
		close(ssocket);
		return false;
	}

	// イベント処理実行
	event_base_dispatch(base);

	fprintf(stderr, "evnet_dispatch() -> end\n");

	// リソース開放
	event_del(&ev);
	event_del(sig);
	free(sig);
	event_base_free(base);

	close(ssocket);
	return true;
}

int main(int argc, char* argv[]) {
	if ( argc < 2 || argc > 3 ) {
		printf("Usage: %s PORT [DELAYTIME]\n", argv[0]);
		exit(-1);
	}
	char* port = argv[1];
	if ( argc == 3 ) {
		delayTime = atoi(argv[2]);
		fprintf(stderr, "delayTime=%d (usec)\n", delayTime);
	}
	start(port);
	return EXIT_SUCCESS;
}
