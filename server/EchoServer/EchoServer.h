#include "TcpServer.h"
class EchoServer : public TcpServer{
public:
    EchoServer(int port, int poolSize);
    ~EchoServer();
    int ReadHandler(int fd, void *data);
    int WriteHandler(int fd, void *data);
};