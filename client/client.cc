#include"client.hpp"
Client::Client()
{

}

Client::~Client()
{

}

void Client::Connect() {
    ctl_fd = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in sin;
    memset(&sin,0,sizeof(sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(2100);
    inet_pton(AF_INET,addr.c_str(),&sin.sin_addr);
    connect(ctl_fd,(sockaddr*)&sin,sizeof(sockaddr_in));
}

void Client::SendRespose(string msg) {
    send(ctl_fd,msg.c_str(),msg.size(),0);
}

string Client::ReadRespose() {
    char buff[1024] = {};
    recv(ctl_fd,buff,1024,0);
    return string(buff);
}

void Client::ParseCommand() {
    while(true) {
        getline(cin,line);
        SendRespose(line);
        cout<<ReadRespose();
    }
}

