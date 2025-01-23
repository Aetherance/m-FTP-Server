#include"client.hpp"

unordered_set<string>command_set = {
    "LIST"
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
        cout<<ReadRespose();
    }
}

vector<string> split(string,char);

bool Client::isCommand() {
    vector<string>cmd = split(line,' ');
    if(command_set.count(cmd[0])) {
        return true;
    }
    cout<<"Command Not Found"<<endl;
    return false;
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