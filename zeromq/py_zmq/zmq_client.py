import zmq
context = zmq.Context()


frontend = context.socket(zmq.ROUTER)
frontend.bind("tcp://*:6001")
frontend.setsockopt(zmq.RCVHWM, 100)

backend = context.socket(zmq.ROUTER)
backend.bind("tcp://*:6002")
# backend.setsockopt(zmq.SNDHWM, 1)

workers_list = OrderedDict()
clients_list = {}   #记录客户端的连接信息

poller = zmq.Poller()
poller.register(backend, zmq.POLLIN)
poller.register(frontend, zmq.POLLIN)

while True:
    socks = dict(poller.poll(1000))                                  #轮询处理
    if (backend in socks and socks[backend] == zmq.POLLIN):
        worker_addr,client_addr,reply  = backend.recv_multipart()    #接受服务器处理完的信息 带地址
        workers_list[worker_addr]=1                                  #记录worker的地址
        if client_addr in clients_list:                              #服务端处理完客户端的消息  
            frontend.send_multipart([client_addr, reply])            #客户端将服务端处理完的消息返回给客户端
        else:
            pass   											         #没有这个客户端  做一些记录

    if len(workers_list) > 0:
        if (frontend in socks and socks[frontend] == zmq.POLLIN):
            client_addr,request = frontend.recv_multipart()           #接收客户端的信息   包含客户端地址
            clients_list[client_addr] = 1    						  #记录客户端的连接地址
            worker_id = workers_list.popitem(False)[0]                #记录 id
            backend.send_multipart([worker_id,client_addr,request])   #转发客户端的消息和id地址  让服务端处理

