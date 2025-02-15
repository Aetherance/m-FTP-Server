#include"client.hpp"

int main() {
    Client client;
    client.Connect();
    client.ParseCommand();
    return 0;
}