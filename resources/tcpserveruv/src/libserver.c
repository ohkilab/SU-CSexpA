/*
 * libserver.c
 *
 *  Created on: 2016/06/11
 *      Author: yasuh
 */

#include "libserver.h"
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct
{
	uv_write_t req;
	uv_buf_t buf;
} write_req_t;

void free_write_req(uv_write_t *req)
{
	write_req_t *wr = (write_req_t *)req;
	free(wr->buf.base);
	free(wr);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = (char *)malloc(suggested_size);
	buf->len = suggested_size;
}

void on_close(uv_handle_t *handle)
{
	free(handle);
}

void echo_write(uv_write_t *req, int status)
{
	if (status)
	{
		fprintf(stderr, "Write error %s\n", uv_strerror(status));
	}
	free_write_req(req);
}

extern int delayTime;

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
	if (nread > 0)
	{
		usleep(delayTime);

		write_req_t *req = (write_req_t *)malloc(sizeof(write_req_t));
		char* reply_buf = (char*)malloc(sizeof(char)*nread+3);
		buf->base[nread-2] = '\0';
		fprintf(stderr, "[client]%s\n", buf->base); // コンソールに出力
		snprintf(reply_buf, 512, "%s:OK\r\n", buf->base);
		req->buf = uv_buf_init(reply_buf, strlen(reply_buf));
		free(buf->base);
		uv_write((uv_write_t *)req, client, &req->buf, 1, echo_write);
		return;
	}
	if (nread < 0)
	{
		if (nread != UV_EOF)
			fprintf(stderr, "Read error %s\n", uv_err_name(nread));
		uv_close((uv_handle_t *)client, on_close);
	}
	free(buf->base);
}

void on_new_connection(uv_stream_t *server, int status)
{
	if (status < 0)
	{
		fprintf(stderr, "New connection error %s\n", uv_strerror(status));
		// error!
		return;
	}

	uv_tcp_t *client = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);
	if (uv_accept(server, (uv_stream_t *)client) == 0)
	{
		struct sockaddr_storage from;
		int len;
		uv_tcp_getsockname(client, (struct sockaddr *) &from, &len);
		char hbuf[NI_MAXHOST];
		char sbuf[NI_MAXSERV];
		getnameinfo((struct sockaddr *) &from, len, hbuf,
				sizeof(hbuf), sbuf, sizeof(sbuf),
				NI_NUMERICHOST | NI_NUMERICSERV);
		fprintf(stderr, "accept:%s:%s\n", hbuf, sbuf);
		uv_read_start((uv_stream_t *)client, alloc_buffer, echo_read);
	}
	else
	{
		uv_close((uv_handle_t *)client, on_close);
	}
}
