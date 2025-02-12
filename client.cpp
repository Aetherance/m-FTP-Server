#include"client_ctl.hpp"
#include"client_trans.hpp"
#include"threadpool"
int main() {
    CommandLine line;
    signal(SIGINT,quit);
    server_fd = Connect("127.0.0.1",2100);
    threadpool pool(2);
    
    pool.submit([&line](){
    });
        line.get();

    return 0;
}