#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

vector<int> fds;

void threadFunc() {

    for(int i = 0; i < 100; ++i) {
        int client = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(9677);
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        connect(client, (struct sockaddr*)&servaddr, sizeof servaddr);

        char buff[10] = "hello";
        send(client, buff, 10, 0);
        recv(client, buff, 10, 0);
        cout<<buff<<endl;
        fds.push_back(client);
    }
}


int main() {
    vector<shared_ptr<thread>> ts;
    for(int i = 0; i < 100; ++i) {
        shared_ptr<thread> t = make_shared<thread>(threadFunc);
        ts.push_back(t);
    }

    //sleep();
    for(auto i : ts) {
        i->join();
    }

    for(auto i : fds) {
        close(i);
    }
    
}
