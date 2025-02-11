#include<iostream>
#include<arpa/inet.h>
#include<memory.h>
#include <termios.h>
#include <unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>
using namespace std;

int server_fd;

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
        pos_echo = 1;
        pthread_mutex_init(&mutex,nullptr);
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        max_pos_echo = w.ws_row - 5;
     }
    void get();
    void echo(string);
    void parse();

private:
    void clearLine() { printf("\033[A\033[2K"); }
    void moveCursor() { printf("\033[9999H"); }
    string data;
    int pos_echo;
    int max_pos_echo;
    pthread_mutex_t mutex;
};

void CommandLine::get() {
    moveCursor();
    printf("\033[A");
    while(true) {
        cout<<":";
        getline(cin,data);
        pthread_mutex_lock(&mutex);
        clearLine();
        pthread_mutex_unlock(&mutex);
        parse();
    }
}

void CommandLine::echo(string data) {
    pthread_mutex_lock(&mutex);
    printf("\033[%dH", pos_echo);
    printf("%s",data.data());
    // pos_echo ++;
    if(pos_echo >= max_pos_echo) {
        system("clear");
        pos_echo = 1;
    }
    moveCursor();
    pthread_mutex_unlock(&mutex);
}

void CommandLine::parse() {
    if(data == "LIST") {
        send(server_fd,"LIST",10,0);
        char buff[1024] = {};
        recv(server_fd,buff,1024,0);
        system("clear");
        echo("服务器目录:\n"+(string)buff);
    }
}