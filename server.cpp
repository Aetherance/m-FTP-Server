#include"threadpool"
#include"server"

threadpool pool(5);
int main() {
    int lfd = Listen(2100,10);
    int epfd = epoll_create(1);
    epoll_event ev;
    ev.data.fd = lfd;
    ev.events = EPOLLIN;
    setNOBLOCK(lfd);
    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&ev);
    while (true) {
        epoll_event rev[16];
        int fds = epoll_wait(epfd,rev,16,-1);
        for(int i = 0;i<fds;i++) {
            if(rev[i].data.fd == lfd) {
                int sock = Accept(lfd);
                cout<<"connected"<<endl;
                epoll_event ev_sock;
                ev_sock.data.fd = sock;
                ev_sock.events = EPOLLIN;
                setNOBLOCK(sock);
                epoll_ctl(epfd,EPOLL_CTL_ADD,sock,&ev_sock);
            }
            else {
                char buff[1024];
                int n_read;
                n_read = read(rev[i].data.fd,buff,1024);
                if(n_read == 0) {
                    close(rev[i].data.fd);
                    epoll_ctl(epfd,EPOLL_CTL_DEL,rev[i].data.fd,nullptr);
                }
                else if(n_read > 0) {
                    if(strcmp(buff,"LIST") == 0) {
                        LIST(rev[i].data.fd);
                    }
                }
            }
        }

    }
}