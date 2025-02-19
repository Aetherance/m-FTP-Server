# m-FTP-server

## 简介

我学习Linux网络编程后实现的一个简单的FTP服务器,支持基本的 FTP 操作,可以在主动模式和被动模式工作。

服务器通过TCP协议和客户端连接。服务器的2100端口用于交换控制信息。

客户端可以通过`PASV`或`PORT`命令于服务器建立用于文件传输的连接。

客户端通过`LIST`命令查看服务器的目录信息，通过`STOR`和`RETR`命令上传和下载文件。

服务器使用`epoll`和非阻塞`I/O`处理客户端请求，并将任务提交至线程池执行。

## 构建

clone仓库
```
git clone git@github.com:Aetherance/m-FTP-Server.git
```

在`/m-FTP-Server`目录下创建`build`目录,并进入
```
mkdir build
cd build
```

使用`cmake`构建
```
cmake ..
make
```

运行服务端
```
./server
```

运行客户端
```
./client
```