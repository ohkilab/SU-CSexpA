/*
 * libserver.h
 *
 *  Created on: 2016/06/13
 *      Author: yasuh
 */

#ifndef LIBSERVER_H_
#define LIBSERVER_H_

#include <stdbool.h>
#include <event.h>
#include <event2/event.h>
#include <evutil.h>

/**
 * @brief TCPサーバソケットを生成する
 * @param port_num 待ち受けポート番号
 * @return 生成したソケット
 */
int serverTCPSocket(const char* port_num);

/**
 * @brief サーバソケットのイベントハンドラ
 * @param sock サーバソケット
 * @param event イベント
 * @param arg 引数
 */
void acceptHandler(evutil_socket_t sock, short event, void* arg);

/**
 * @brief アクセプト時のイベントハンドラ：クライアントの入力に対してエコーバックを行う
 * @param acc ソケット
 * @param event イベント
 * @param arg 引数
 */
void echobackHandler(evutil_socket_t acc, short event, void* arg);

#endif /* LIBSERVER_H_ */
