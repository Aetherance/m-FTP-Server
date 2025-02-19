#include<iostream>
#include<arpa/inet.h>
#include<memory.h>
#include<string>
#include<fcntl.h>
#include<unordered_set>
#include<vector>
#include<thread>
#include<sys/sendfile.h>
#include<unistd.h>

#define UPLOAD_MAX 512000000

using namespace std;

class Client
{
public:
    Client();
    ~Client();
    void Connect();
    void SendRespose(string);
    string ReadRespose();
    void ParseCommand();
    bool isCommand();
    void setActive(string);
    void parse();
    void setPassive();
    void PrintInfo();

private:
    string line;
    int ctl_fd = 2100;
    string addr = "127.0.0.1";
    bool isActive;
    bool isPassive;
    int active_fd;
    int passive_fd;
    unsigned int active_port;
    unsigned int passive_port;
};