#include<iostream>
#include<arpa/inet.h>
#include<memory.h>
#include <termios.h>
#include <unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<signal.h>
#include<filesystem>

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
        pos_echo = 2;
        pthread_mutex_init(&mutex,nullptr);
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        max_pos_echo = w.ws_row - 5;
        isPASV = false;
        isConnected = false;
     }
    void get();
    void echo(string);
    void parse();
    void setPasv();
    void setActv();

private:
    void clearLine() { printf("\033[A\033[2K"); }
    void moveCursor() { printf("\033[9999H\033[A\033[A"); }
    string data;
    int pos_echo;
    int max_pos_echo;
    pthread_mutex_t mutex;
    bool isPASV;
    bool isConnected;
    int active_mode_sock;
};

void CommandLine::get() {
    system("clear");
    moveCursor();
    printf("\033[A\033[A");
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
    system("clear");
    cout<<"FTP server    "<<"当前模式  "<<(isPASV ? "被动" : "主动" )<< ":  " <<(isConnected ? "已连接" : "未连接" )<<endl;
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

void Message(const char * msg) {
    printf("\033[2K\b \b:");
    printf("\033[A\033[2K%s\n",msg);
} 
void Message(string msg) {
    Message(msg.data());
}

void upload(int,char*);

void CommandLine::parse() {
    if(data == "q"||data == "exit") {
        system("clear");
        exit(0);
    }
    else if(data == "LIST") {
        send(server_fd,"LIST",10,0);
        char buff[1024] = {};
        recv(server_fd,buff,1024,0);
        system("clear");
        echo("服务器目录:\n"+(string)buff);
    }
    else if(data == "PORT") {
        send(server_fd,"PORT",10,0);
        setActv();
    }
    else if(data == "STOR"&&isPASV == false) {
        string path;
        Message("请输入文件路径");
        getline(cin,path);
        filesystem::path p_parse(path);
        send(server_fd,"UPLOAD",10,0);
        upload(active_mode_sock,path.data());
        send(server_fd,p_parse.filename().string().c_str(),p_parse.filename().string().size(),0);
        echo("文件上传成功!");
    }
    else {
        Message("Command Not Found");
    }
}

void quit(int quit) {
    Message("输入 q 或 exit 退出");
}