#include"server.hpp"

void logger::log(char ip[],unsigned int port,const char * event) {
    time_t rawtime;
    struct tm *timeinfo;
    char time_str[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);
    const char* color = "\033[0m"; 
    const char* reset_color = "\033[0m";  // 重置

    if (strcmp(event,"与服务器建立了连接") == 0) {
        color = "\033[1m\033[032m";  // 绿
    } else if (strcmp(event,"断开连接") == 0) {
        color = "\033[1m\033[31m";  // 红
    }

    printf("[%s] [INFO] %sClient %s:%d %s%s\n", time_str, color, ip, port, event, reset_color);
}

void logger::log(int fd,const char * event) {
    sockaddr_in sin;
    socklen_t size = sizeof(sockaddr_in);
    getpeername(fd,(sockaddr*)&sin,&size);
    char ip[16];
    int port;
    inet_ntop(AF_INET,(sockaddr*)&sin.sin_addr,ip,sizeof(sockaddr_in));
    port = ntohs(sin.sin_port);
    log(ip,port,event);
}

void logger::log(int fd,string event) {
    log(fd,event.data());
}