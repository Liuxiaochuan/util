#ifndef __LXC_SOCKET_H__
#define __LXC_SOCKET_H__

#ifdef WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif

#ifndef WIN32
typedef int SOCKET;
#endif

class LxcSocket
{
public:
	LxcSocket();
	explicit LxcSocket(SOCKET handle);
	virtual ~LxcSocket();

public:
	static bool init();
	static void cleanup();

public:
	bool socket(int af, int type, int proto = 0);
	bool bind(unsigned long ip, unsigned short port);
	bool bind(const char* ip, unsigned short port);
	bool listen(int max = 5);
	SOCKET accept(struct sockaddr_in& address);
	SOCKET accept();
	bool connect(const char* ip, unsigned short port);
	bool shutdown(int how);
	void close();

public:
	bool nonBlock(bool enable);
	bool wouldBolck(int error);

public:
	int read(char* buf, int len);
	int write(const char* buf, int len);
	bool writeAll(const char* buf, int len);

public:
	void pton(const char* ip, unsigned short port, struct sockaddr_in* addr);
	bool peername(struct sockaddr_in& peer);
	bool localname(struct sockaddr_in& local);

protected:
	SOCKET m_socket;
};

#endif
