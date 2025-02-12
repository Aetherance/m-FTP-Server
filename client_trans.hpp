#include<sys/sendfile.h>
#define UPLOAD_MAX 512000000

void upload(int sock,char filepath[]) {
    int fd = open(filepath,O_RDONLY);
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

void CommandLine::setActv() {
    string port;
    Message("主动模式: 输入端口号");
    getline(cin,port);
    int port_int = atoi(port.data());

    send(server_fd,port.data(),7,0);
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in sin;
    memset(&sin,0,sizeof(sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port_int);
    sin.sin_addr.s_addr = INADDR_ANY;
    
    int bindret = bind(lfd,(sockaddr*)&sin,sizeof(sockaddr_in));
    if(bindret == -1) {
        perror("setActv:bind");
        exit(-1);
    }
    listen(lfd,1);
    sockaddr_in c_sin;
    socklen_t len = sizeof(sockaddr_in); // 逆天
    int sock = accept(lfd,(sockaddr*)&c_sin,&len);
    send(server_fd,"connect",7,0);
    if(sock == -1) {
        perror("accept-client");
    }
    isConnected = true;
    isPASV = false;
    echo("");
    string path;
    Message("请输入文件路径");
    getline(cin,path);

    echo("文件上传成功!");
    upload(sock,path.data());
}

void CommandLine::setPasv() {


    isPASV = true;
}


void download(int sock,char filepath[]) {
    int fd = open(filepath,O_RDWR | O_APPEND | O_CREAT | O_EXCL , 0644 );
    // while (  ) {
        
    // }
    
}