OBJS=libserver.o TCPServerUV.o
SRCS=$(OBJS:%.o=%.c)
CFLAGS=-g -Wall -std=c99 -D_GNU_SOURCE
LDLIBS=-luv
TARGET=tcpserveruv
$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

clean:
	-rm -f ${OBJS} ${TARGET} ${TARGET}.exe

