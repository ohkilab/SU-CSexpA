/*
 * libserver.h
 *
 *  Created on: 2016/06/13
 *      Author: yasuh
 */

#ifndef LIBSERVER_H_
#define LIBSERVER_H_

#include <stdbool.h>
#include <uv.h>

void on_new_connection(uv_stream_t *server, int status);
uv_loop_t *loop;

#else

extern uv_loop_t *loop;

#endif /* LIBSERVER_H_ */
