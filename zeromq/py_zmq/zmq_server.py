import zmq
context = zmq.Context()
responser = context.socket(zmq.DEALER)
responser.setsockopt(zmq.RCVTIMEO, 10000)
responser.connect("tcp://localhost:6002")
while True:
    ident, message = response_socket.recv_multipart()
    pass
    _response = "1"     												 #处理完的数据
    response_socket.send_multipart([ident, _response])
