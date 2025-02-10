#include"client.hpp"
#include"threadpool"

int main() {
    server_fd = Connect("127.0.0.1",2100);
    threadpool pool(2);
    CommandLine line;
    
    pool.submit([&line](){
        line.get();
    });

    usleep(100);

    pool.submit([&line](){

    });

    return 0;
}