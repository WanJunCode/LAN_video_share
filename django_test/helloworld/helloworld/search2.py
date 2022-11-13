# -*- coding: utf-8 -*-
 
from django.shortcuts import render
from django.views.decorators import csrf
import socket
import time

host = '127.0.0.1'
port = 123
MaxBytes = 1024*1024

def GetResponse(req):
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.settimeout(30)
    try:
        client.connect((host, port))
    except:
        return 'failed to connect'
    sendBytes = client.send(req.encode())
    if sendBytes <= 0:
        return 'failed to send'
    recvData = client.recv(MaxBytes)
    if not recvData:
        return 'failed to recv'
    localTime = time.asctime(time.localtime(time.time()))
    return bytes.decode(recvData)

# 接收POST请求数据
def search_post(request):
    ctx ={}
    if request.POST:
        ctx['rlt'] = request.POST['q']
        # 客户端发送 request.POST['q'] 并接收C++服务端响应
        ctx['response'] = GetResponse(request.POST['q'])
    return render(request, "post.html", ctx)
