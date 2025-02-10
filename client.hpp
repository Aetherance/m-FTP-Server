#include<iostream>
#include<arpa/inet.h>
#include<memory.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

int Connect(const char ip[16],unsigned int port) {
    int fd = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in sin;
    memset(&sin,0,sizeof(sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    inet_pton(AF_INET,ip,&sin.sin_addr);
    connect(fd,(sockaddr*)&sin,sizeof(sockaddr_in));
    return fd;
}

class CommandLine
{
public:
    CommandLine() { 
        pos_echo = 5;
        pthread_mutex_init(&mutex,nullptr);
     }
    void get();
    void echo(string);

private:
    void clearLine() { printf("\033[A\033[2K"); }
    void moveCursor() { printf("\033[9999H\033[A"); }
    string data;
    int pos_echo;
    pthread_mutex_t mutex;
};

void CommandLine::get() {
    moveCursor();
    while(true) {
        cout<<":";
        getline(cin,data);
        pthread_mutex_lock(&mutex);
        clearLine();
        pthread_mutex_unlock(&mutex);
    }
}

void CommandLine::echo(string data) {
    pthread_mutex_lock(&mutex);
    printf("\033[%dH", pos_echo);
    cout<<data;
    pos_echo ++;
    moveCursor();
    pthread_mutex_unlock(&mutex);
}