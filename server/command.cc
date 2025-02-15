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
    vector<string>cmd = split(line);
    if(cmd[0] == "LIST") {
        int pid = fork();
        if(pid == 0) {
            dup2(fd,1);
            char *args[] = {(char*)"ls",(char*)"-l",(char*)"--color=yes",nullptr};
            execvp("ls",args);
        }
        wait(nullptr);
    }
}