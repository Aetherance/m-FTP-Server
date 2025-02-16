#include"server.hpp"

CommandParser::CommandParser() {}
CommandParser::~CommandParser() {}

vector<string> CommandParser::split(string s,char ch = ' ')
{
    vector<string>result;
    int pos = 0;
    while (s[pos]==ch)
        pos++;
    
    while (pos< s.size())
    {
        int n = 0;
        while (s[pos+n]!=ch&&pos+n<s.size())
        {
            n++;
        }
        result.push_back(s.substr(pos,n));
        pos += n;
        while (s[pos] ==ch&&pos<s.size())
        {
            pos++;
        }
    }
    return result;
}

void CommandParser::parse(string CommandMsg,int fd) {
    line = CommandMsg;
    target_fd = fd;
    vector<string>cmd = split(line,' ');
    cout<<cmd[1]<<endl;
    this->cmd = cmd;
    cout<<line<<endl;
    if(cmd[0] == "LIST") {
        pool->submit([this](){list();});
    } else if(cmd[0] == "PORT") {
        pool->submit([this](){port();});
    } else if(cmd[0] == "STOR") {
        pool->submit([this](){stor();});
    }
    
    
}

void CommandParser::list() {
    log(target_fd,"访问了服务器目录(LIST)");
    int pid = fork();
    if(pid == 0) {
        dup2(target_fd,1);
        char *args[] = {(char*)"ls",(char*)"-l",(char*)"../root",(char*)"--color=yes",nullptr};
        execvp("ls",args);
    }
    wait(nullptr);
}

void CommandParser::port() {
    unsigned int port = atoi(cmd[1].c_str());
    log(target_fd,"正在开启主动模式 端口:"+cmd[1]);
    Connect(target_fd,port);
    send(target_fd,"200 主动连接已建立",1024,0);
}

void CommandParser::stor() {
    filesystem::path filepath(cmd[1]);
    filesystem::path filename = "../root/" + filepath.filename().string();
    int sock = (*active_map)[target_fd];
    log(sock,"正在上传一个文件 "+filename.filename().string());
    int fd = open(filename.c_str(),O_WRONLY | O_CREAT | O_APPEND | O_TRUNC , 0644);
    char buff[1024];
    int n = 0;
    while ((n = read(sock,buff,1024)) > 0) {
        write(fd,buff,n);
    }
    close(fd);
}