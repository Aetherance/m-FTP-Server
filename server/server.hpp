#ifndef SERVER_H_SOURCE
#define SERVER_H_SOURCE

#include<iostream>
#include<arpa/inet.h>
#include"threadpool.h"
#include<memory.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<vector>
#include<wait.h>
#include<memory>

using namespace std;

class logger
{
public:
    void log(char [],unsigned int ,const char * );
    void log(int ,const char * );
    void log(int ,string );
private:

    
};

class Server : public logger
{   
public:
    Server();
    ~Server();
    void Listen(int);
    int Accept();
    void Connect();
    void Epoll();
    void setNoblock(int);
    static unique_ptr<threadpool>pool;
        
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
    void list();
private:
    string line;
    int target_fd;

};



#endif