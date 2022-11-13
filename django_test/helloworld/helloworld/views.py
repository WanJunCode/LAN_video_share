from django.http import HttpResponse
from django.shortcuts import render
from datetime import datetime
 
def runoob(request):
    context = {}
    context['hello'] = 'Hello World!'
    context['views_name'] = "菜鸟教程"
    context['views_list'] = ["菜鸟教程1","菜鸟教程2","菜鸟教程3"]   # 传入列表
    context['views_dict'] = {"name":"dict_value"} # 传入字典
    context['time'] = datetime.now() # 传入日期
    context['views_str'] = "<a href='https://www.runoob.com/'>新链接点击跳转</a>" # 传入链接
    return render(request, 'runoob.html', context)

def hello(request):
    return HttpResponse("Hello world !")