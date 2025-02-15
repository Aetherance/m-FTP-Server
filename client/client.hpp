#include<iostream>
#include<arpa/inet.h>
#include<memory.h>
#include<string>
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
private:
    string line;
    int ctl_fd = 2100;
    string addr = "127.0.0.1";

};