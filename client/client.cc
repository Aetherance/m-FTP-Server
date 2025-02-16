#include"client.hpp"

unordered_set<string>command_set = {
    "LIST",
    "PORT",
    "STOR"
};

Client::Client() {}

Client::~Client() {}


void Client::Connect() {
    ctl_fd = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in sin;
    memset(&sin,0,sizeof(sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(2100);
    inet_pton(AF_INET,addr.c_str(),&sin.sin_addr);
    connect(ctl_fd,(sockaddr*)&sin,sizeof(sockaddr_in));
}

void Client::SendRespose(string msg) {
    send(ctl_fd,msg.c_str(),msg.size(),0);
}

string Client::ReadRespose() {
    char buff[1024] = {};
    recv(ctl_fd,buff,1024,0);
    return string(buff);
}

void Client::ParseCommand() {
    while(true) {
        getline(cin,line);
        if(!isCommand())continue;
        SendRespose(line);
        parse();
        cout<<ReadRespose()<<endl;
    }
}

vector<string> split(string,char);

bool Client::isCommand() {
    vector<string>cmd = split(line,' ');
    if(!command_set.count(cmd[0])) {
        cout<<"Command Not Found"<<endl;
        return false;
    }
    return true;
}

void upload(int, char[]);
void Client::parse() {
    vector<string>cmd = split(line,' ');
    if(cmd[0] == "PORT") {
        setActive(cmd[1]);
    } else if (cmd[0] == "STOR") {
        upload(active_fd,cmd[1].data());
    }
}

vector<string> split(string s,char ch = ' ')
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

void Client::setActive(string port_str) {
    int port = atoi(port_str.c_str());
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in sin;
    memset(&sin,0,sizeof(sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = INADDR_ANY;
    if(bind(lfd,(sockaddr*)&sin,sizeof(sockaddr_in)) == -1) {
        perror("bind");
    }
    listen(lfd,2);
    sockaddr_in server_sin;
    socklen_t server_sin_len = sizeof(sockaddr_in); 
    cout<<"正在连接至服务器..."<<endl;
    if((active_fd = accept(lfd,(sockaddr*)&sin,&server_sin_len)) == -1) {
        cout<<"连接失败"<<endl;
    }
    close(lfd);
}

void upload(int sock,char filepath[]) {
    int fd = open(filepath,O_RDONLY);
    cout<<filepath<<endl;
    if(fd == -1) {
        perror("open");
    }
    if(sock == -1) {
        perror("update sock");
    }
    if(sendfile(sock,fd,nullptr,UPLOAD_MAX) == -1) {
        perror("sendfile");
    };
}
