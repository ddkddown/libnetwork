#include "TcpServer.h"
class EchoServer : public TcpServer{
public:
    EchoServer(int port, int poolSize);
    ~EchoServer();
    void ReadCompleteCall(TcpConnection &conn, Buffer &buff);
    void WriteCompleteCall(TcpConnection &conn, Buffer &buff);
};