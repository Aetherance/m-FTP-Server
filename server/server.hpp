#include<iostream>
#include<arpa/inet.h>
#include"threadpool"

using namespace std;

class Server
{   
public:
    Server();
    ~Server();
    void Listen();
    void Accept();
    void Connect();
    void Epoll();
    
private:
    int ctl_fd;
    
};

Server::Server() {

}

Server::~Server() {

}

void Server::Listen() {

}

void Server::Accept() {

}

void Server::Connect() {

}