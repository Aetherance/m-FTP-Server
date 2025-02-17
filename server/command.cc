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
        while (s[pos+n]!=ch&&pos+n<s.size()) {
            n++;
        }
        result.push_back(s.substr(pos,n));
        pos += n;
        while (s[pos] ==ch&&pos<s.size()) {
            pos++;
        }
    }
    return result;
}

void CommandParser::parse(string CommandMsg,int fd) {
    line = CommandMsg;
    target_fd = fd;
    vector<string>cmd = split(line,' ');
    this->cmd = cmd;
    cout<<line<<endl;
    if(cmd[0] == "LIST") {
        pool->submit([this](){list();});
    } else if(cmd[0] == "PORT") {
        pool->submit([this](){port();});
    } else if(cmd[0] == "STOR") {
        pool->submit([this](){stor();});
    } else if(cmd[0] == "RETR") {
        pool->submit([this](){retr();});
    } else if(cmd[0] == "PASV") {
        pool->submit([this](){pasv();});
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
    trans_mode = 1;
    send(target_fd,"200 主动连接已建立",1024,0);
    unsigned int port = atoi(cmd[1].c_str());
    log(target_fd,"正在开启主动模式 端口:"+cmd[1]);
    Connect(target_fd,port);
}

void CommandParser::stor() {
    filesystem::path filepath(cmd[1]);
    filesystem::path filename = "../root/" + filepath.filename().string();
    
    int sock = -1;
    if(trans_mode == 1) {
        sock = (*active_map)[target_fd];
    } else if(trans_mode == 0) {
        sock = (*passive_map)[target_fd];
    }

    log(sock,"正在上传一个文件 "+filename.filename().string());
    int fd = open(filename.c_str(),O_WRONLY | O_CREAT | O_TRUNC , 0644);
    char buff[1024];
    int n = 0;
    while ((n = read(sock,buff,1024)) > 0) {
        write(fd,buff,n);
        cout<<buff<<n<<endl;
    }
    close(fd);
    close(sock);
    trans_mode = -1;
    send(target_fd,"200 文件上传成功",1024,0);
}

void CommandParser::retr() {
    int sock = -1;
    if(trans_mode == 1) {
        sock = (*active_map)[target_fd];
    } else if(trans_mode == 0) {
        sock = (*passive_map)[target_fd];
    }

    log(sock,"正在下载一个文件");
    string filepath = "../root/"+cmd[1];
    int fd = open(filepath.c_str(),O_RDONLY);
    if(fd == -1) {
        perror("sendfile: open");
    }
    if(sendfile(sock,fd,nullptr,UPLOAD_MAX) == -1) {
        perror("sendfile");
        send(target_fd,"500 文件下载失败",1024,0);
    } else {
        send(target_fd,"200 文件下载成功",1024,0);
    };
    close(sock);
    trans_mode = -1;
    
}

bool isUsable(unsigned int port) {
    int try_fd = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in sin;
    memset(&sin,0,sizeof(sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);
    if(bind(try_fd,(sockaddr*)&sin,sizeof(sockaddr_in)) == -1) {
        close(try_fd);
        return false;
    }
    close(try_fd);
    return true;
}

unsigned int portGenerator() {
    srand(time(NULL));
    unsigned int port;
    while (!isUsable(port = rand() % (65535 - 1024 + 1) + 1024));
    return port;
}

void CommandParser::pasv() {
    trans_mode = 0;
    unsigned int port = portGenerator();
    sockaddr_in sin;
    memset(&sin,0,sizeof(sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = INADDR_ANY;
    
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    bind(lfd,(sockaddr*)&sin,sizeof(sockaddr_in));
    listen(lfd,1);
    sockaddr_in client_sin;
    socklen_t len = sizeof(sockaddr_in);
    char msg[1024];
    sprintf(msg,"正在建立被动连接 %d",port);
    send(target_fd,msg,1024,0);
    int sock = accept(lfd,(sockaddr*)&client_sin,&len);
    close(lfd);
    if(sock == -1) {
        send(target_fd,"510 被动连接建立失败",1024,0);
    } else {
        send(target_fd,"200 被动连接已建立",1024,0);
    }
    log(target_fd,"正在建立被动连接");
    
    passive_map->insert({target_fd,sock});
}