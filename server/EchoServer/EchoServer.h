#include "TcpServer.h"
class EchoServer : public TcpServer{
public:
    EchoServer(int port, int poolSize);
    ~EchoServer();
    void ReadCompleteCall(const TcpConnectionPtr &conn, Buffer *buff);
    void WriteCompleteCall(const TcpConnectionPtr &conn, Buffer *buff);
};