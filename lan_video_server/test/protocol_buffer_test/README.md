This file was generated by an older version of protoc which is
需要升级 protoc 的版本
查看版本信息 protoc --version

Explicit 'optional' labels are disallowed in the Proto3 syntax. To define 'optional' fields in Proto3, simply remove the 'optional' label, as fields are 'optional' by default.

required：声明该值是必要的值，不能为空（proto3舍弃）
optional：声明该值是可选的值，可以为空（proto3舍弃）
//proto3舍弃以上两种修饰，值都能为空
repeated：声明该值为多个数值，可以组成数组的形式


No syntax specified for the proto file: person.proto. Please use 'syntax = "proto2";' or 'syntax = "proto3";' to specify a syntax version. (Defaulted to proto2 syntax.)
在配置文件开头使用
syntax = "proto3";
syntax = "proto2";
指定


版本不一致的问题，是因为通过命令行下载的protoc版本太低了
root@9f93757e560b:/# protoc --version
libprotoc 3.0.0
但是通过conan下载的protocbuf版本为 protobuf/3.17.1，所以在编译时会报版本不一致，可以在conan中bin目录下找到3.17.1版本的protoc可执行文件


生成的 class 继承自 public ::google::protobuf::Message，能够使用 SerializeXXX (序列化) ParseFromXXX (反序列化) 函数


protocol buffer编译错误 参考以下链接
https://blog.csdn.net/m0_59666413/article/details/121787638


语法规则
https://blog.csdn.net/zxng_work/article/details/78943138


入门用例
https://blog.csdn.net/qq_44272681/article/details/124598549


序列化和反序列化
https://blog.csdn.net/sealyao/article/details/6940245