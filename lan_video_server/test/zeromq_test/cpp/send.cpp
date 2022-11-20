#include "test_struct.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <zmq.h>

int main()
{
    void *context = zmq_ctx_new();
    assert(context != NULL);

    void *publisher = zmq_socket(context, ZMQ_PUB);
    assert(publisher != NULL);

    int ret = zmq_bind(publisher, "tcp://*:5555");
    assert(ret == 0);

    int i = 0;

    frame_test frame;

    while (1)
    {
        printf("pub ctx: server i = %d\n", i);
        frame.frameid = i;
        frame.status = 1;
        frame.radius = 10 * i;
        frame.area = frame.radius * frame.radius * 3.14159;

        // 直接发送结构体内存
        ret = zmq_send(publisher, &frame, sizeof(frame_test), 0);
        i++;
        sleep(1);
    }

    zmq_close(publisher);
    zmq_ctx_destroy(context);

    return 0;
}