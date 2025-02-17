#include"server.hpp"

unique_ptr<threadpool>Server::pool = make_unique<threadpool>(4);
unique_ptr<map<int,int>>Server::active_map = make_unique<map<int,int>>();
unique_ptr<map<int,int>>Server::passive_map = make_unique<map<int,int>>();

short Server::trans_mode = -1;

Server::Server() {
    
}

Server::~Server() {}

void Server::setNoblock(int fd) {
    int flag = fcntl(fd,F_GETFL);
    if(flag == -1) {
        perror("setNOBLOCK:GETFL");
        return;
    }
    if(fcntl(fd,F_SETFL,flag | O_NONBLOCK) == -1) {
         perror("setNOBLOCK:SETFL");
         return;
    };
}

void Server::Listen(int listen_len) {
    lis_fd = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in sin;
    memset(&sin,0,sizeof(sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(ctl_port);
    sin.sin_addr.s_addr = INADDR_ANY;
    if(bind(lis_fd,(sockaddr*)&sin,sizeof(sockaddr_in)) == -1) {
        perror("listen:bind");
    }
    listen(lis_fd,listen_len);
}

int Server::Accept() {
    sockaddr_in client_sin;
    socklen_t client_sin_len = sizeof(sockaddr_in);
    int sock = accept(lis_fd,(sockaddr*)&client_sin,&client_sin_len);
    return sock;
}

void Server::Epoll() {
    epfd = epoll_create(1);
    epoll_event lis_fd_ev;
    lis_fd_ev.data.fd = lis_fd;
    lis_fd_ev.events = EPOLLIN;
    setNoblock(lis_fd);
    epoll_ctl(epfd,EPOLL_CTL_ADD,lis_fd,&lis_fd_ev);
    while(true) {
        epoll_event ret_events[16];
        int nfd = epoll_wait(epfd,ret_events,16,-1);
        for(int i = 0;i<nfd;i++) {
            if(ret_events[i].data.fd == lis_fd) {
                int ctl_fd = Accept();
                setNoblock(ctl_fd);
                epoll_event ctl_fd_ev;
                ctl_fd_ev.data.fd = ctl_fd;
                ctl_fd_ev.events = EPOLLIN;

                log(ctl_fd,"与服务器建立了连接");
                epoll_ctl(epfd,EPOLL_CTL_ADD,ctl_fd,&ctl_fd_ev);
            }
            else {
                char command_buff[1024];
                memset(command_buff,0,1024);
                int read_stat = read(ret_events[i].data.fd,command_buff,1024);
                if(read_stat == 0) {
                    log(ret_events[i].data.fd,"断开连接");
                    epoll_ctl(epfd,EPOLL_CTL_DEL,ret_events[i].data.fd,nullptr);
                    close(ret_events[i].data.fd);
                    close((*active_map)[ret_events[i].data.fd]);
                    active_map->erase(ret_events[i].data.fd);
                }
                else if(read_stat > 0) {
                    CommandParser parser;
                    parser.parse(command_buff,ret_events[i].data.fd);
                }
            }
        }
    }
}

void Server::Connect(int ctlsock,unsigned int port) {
    sockaddr_in client_sin;
    socklen_t client_sin_len = sizeof(sockaddr_in);
    getpeername(ctlsock,(sockaddr*)&client_sin,&client_sin_len);
    client_sin.sin_port = htons(port);
    int trans_sock = socket(AF_INET,SOCK_STREAM,0);
    connect(trans_sock,(sockaddr*)&client_sin,client_sin_len); 
    log(ctlsock,"成功开启主动模式");
    active_map->insert({ctlsock,trans_sock});
}