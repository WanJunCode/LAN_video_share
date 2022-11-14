# -*- coding: utf-8 -*-

from django.shortcuts import render
from django.views.decorators import csrf
import socket
import time
import json
import random

host = '127.0.0.1'
port = 123
MaxBytes = 1024*1024
sequence = random.randint(1, 1024)


def ConvertToJson(json_request):
    global sequence  # https://blog.csdn.net/weixin_45666566
    sequence += 1
    json_request["sequence"] = sequence
    json_str = json.dumps(json_request)
    print(json_str)
    return json_str


def GetResponse(json_request):
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.settimeout(30)
    try:
        client.connect((host, port))
    except:
        return 'failed to connect'

    # json 数据发送
    json_request_str = ConvertToJson(json_request)
    print("json request ", json_request_str)

    sendBytes = client.send(json_request_str.encode())
    if sendBytes <= 0:
        return 'failed to send'
    recvData = client.recv(MaxBytes)
    if not recvData:
        return 'failed to recv'
    localTime = time.asctime(time.localtime(time.time()))
    return bytes.decode(recvData)

# 接收POST请求数据


def search_post(request):
    ctx = {}
    if request.POST:
        ctx['rlt'] = {
            "name": request.POST['name'],
            "value": request.POST['value']
        }
        # 客户端发送 request.POST['q'] 并接收C++服务端响应
        ctx['response'] = GetResponse(ctx['rlt'])
    return render(request, "post.html", ctx)
