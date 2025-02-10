#include"client.hpp"
#include"threadpool"

int main() {
    int server_fd = Connect("127.0.0.1",2100);
    threadpool pool(2);
    CommandLine line;
    pool.submit([&line](){
        line.get();
    });

    pool.submit([&line](){
        while (1)
        {
            line.echo("1");
            sleep(1);
        }
    });

    return 0;
}