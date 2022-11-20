#include "test_struct.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <zmq.h>

int main()
{
    frame_test frame;

    printf("Hello sub!\n");

    void *context = zmq_ctx_new();
    assert(context != NULL);

    void *subscriber = zmq_socket(context, ZMQ_SUB);
    assert(subscriber != NULL);

    int ret = zmq_connect(subscriber, "tcp://127.0.0.1:5555");
    assert(ret == 0);

    ret = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);
    assert(ret == 0);

    while (1)
    {
        printf("enter while to sub ctx\n");
        ret = zmq_recv(subscriber, &frame, sizeof(frame_test), 0);
        if (ret > 0)
        {
            printf("id:  %d\n", frame.frameid);
            printf("status:  %d\n", frame.status);
            printf("radius:  %f\n", frame.radius);
            printf("area:  %f\n", frame.area);
            printf("\n");
            printf("\n");
        }
    }
    zmq_close(subscriber);
    zmq_ctx_destroy(context);
    return 0;
}