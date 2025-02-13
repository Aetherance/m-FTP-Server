#define SERVER_CTL

#include<iostream>
#include<arpa/inet.h>
#include<memory.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<time.h>
#include<unordered_map>
#include<filesystem>
#include<sys/sendfile.h>

using namespace std;

unordered_map<int,int>actv_map;

int Listen(unsigned int port,int listen_lenth) {
    int sock_fd = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in sin;
    memset(&sin,0,sizeof(sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = INADDR_ANY;
    if(bind(sock_fd,(sockaddr*)&sin,sizeof(sockaddr_in)) == -1) {
        perror("bind");
    }
    listen(sock_fd,listen_lenth);
    return sock_fd;
}

void log(char ip[],unsigned int port,const char * event) {
    time_t rawtime;
    struct tm *timeinfo;
    char time_str[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);
    const char* color = "\033[0m";  // 绿
    const char* reset_color = "\033[0m";  // 重置

    if (event == "connected") {
        color = "\033[1m\033[032m";  // 蓝
    } else if (event == "disconnected") {
        color = "\033[1m\033[31m";  // 红
    }
    
    printf("[%s] [INFO] %sClient %s:%d %s%s\n", time_str, color, ip, port, event, reset_color);
}

void log(int fd,const char * event) {
    sockaddr_in sin;
    socklen_t size = sizeof(sockaddr_in);
    getpeername(fd,(sockaddr*)&sin,&size);
    char ip[16];
    int port;
    inet_ntop(AF_INET,(sockaddr*)&sin.sin_addr,ip,sizeof(sockaddr_in));
    port = ntohs(sin.sin_port);
    log(ip,port,event);
}

void log(int fd,string event) {
    log(fd,event.data());
}

int Accept(int lfd) {
    sockaddr_in c_sin;  
    socklen_t c_sin_size = sizeof(sockaddr_in);
    int cl_fd = accept(lfd,(sockaddr*)&c_sin,&c_sin_size);
    if(cl_fd == -1) {
        perror("accept");
    }
    char ip[16];
    int port;
    inet_ntop(AF_INET,&c_sin.sin_addr,ip,sizeof(c_sin));
    port = ntohs(c_sin.sin_port);
    log(ip,port,"connected");
    return cl_fd;
}

void setNOBLOCK(int fd) {
    int flag = fcntl(fd,F_GETFL);
    if(flag == -1) {
        perror("setNOBLOCK:GETFL");
        return;
    }
    if(fcntl(fd,F_SETFL,flag | O_NONBLOCK) == -1) {
         perror("setNOBLOCK:SETFL");
         return;
    };
}

void LIST(int sock) {
    int pid = fork();
    if(pid == 0) {
        dup2(sock,1);
        char *argv[] = {(char*)"ls",(char*)"-l",(char*)"../root",nullptr};
        execvp("ls",argv);
        perror("exec");
        _exit(1);
    }
    else {
        wait(nullptr);
    }
}