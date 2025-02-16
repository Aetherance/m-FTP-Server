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
#include<map>

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
    void Connect(int , unsigned int);
    void Epoll();
    void setNoblock(int);
    static unique_ptr<threadpool>pool;
        
private:
    unsigned int ctl_port = 2100; // 控制信息传输端口
    int lis_fd;
    int epfd;
    map<int,int>active_map;
    
    
};

class CommandParser : public Server
{
public:
    CommandParser();
    ~CommandParser();
    vector<string>split(string,char);
    void parse(string,int);
    void list();
    void port();
private:
    string line;
    int target_fd;
    vector<string>cmd;

};



#endif