#include"server.hpp"

int main() {
    Server server;
    server.Listen(10);
    server.Epoll();
    return 0;
}
