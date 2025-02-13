#ifndef SERVER_CTL

#include"server_ctl.hpp"
#endif

void receive(int sock,string filepath) {
    filepath = "../root/" + filepath;
    int fd = open(filepath.data(),O_WRONLY | O_CREAT | O_APPEND | O_TRUNC , 0644);
    char buff[1024];
    int n = 0;
    while ((n = read(sock,buff,1024)) > 0  || n  == -1) {
        write(fd,buff,n);
    }
    close(fd);
}

void PORT(int sock) {
    char buff[1024] = {};
    while(recv(sock,buff,1024,0) == -1);
    int port = atoi(buff);
    sockaddr_in sin;
    socklen_t size = sizeof(sockaddr_in);
    getpeername(sock,(sockaddr*)&sin,&size);
    sin.sin_port = htons(port);
    int actv_server_fd = socket(AF_INET,SOCK_STREAM,0);
    connect(actv_server_fd,(sockaddr*)&sin,size);
    string port_str(buff);
    log(sock,"connected in active mode on "+port_str);
    actv_map.insert({sock,actv_server_fd});
}
