#include<iostream>
#include<arpa/inet.h>
#include<memory.h>
#include<string>
#include<fcntl.h>
#include<unordered_set>
#include<vector>

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
private:
    string line;
    int ctl_fd = 2100;
    string addr = "127.0.0.1";
};