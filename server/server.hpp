#ifndef SERVER_H_SOURCE
#define SERVER_H_SOURCE

#include<iostream>
#include<arpa/inet.h>
#include"threadpool"
#include<memory.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<vector>
#include<wait.h>

using namespace std;

class Server
{   
public:
    Server();
    ~Server();
    void Listen(int);
    int Accept();
    void Connect();
    void Epoll();
    void setNoblock(int);
        
private:
    unsigned int ctl_port = 2100; // 控制信息传输端口
    int lis_fd;
    int epfd;
};

class CommandParser : public Server
{
public:
    CommandParser();
    ~CommandParser();
    vector<string>split(string,char);
    void parse(string,int);
private:
    string line;
    int target_fd;

};

#endif