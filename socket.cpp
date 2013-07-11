#include "socket.h"

LxcSocket::LxcSocket()
{
	m_socket = INVALID_SOCKET;
}

LxcSocket::LxcSocket(SOCKET handle)
{
	m_socket = handle;
}

LxcSocket::~LxcSocket()
{
	close();
}

bool LxcSocket::init()
{
#ifdef WIN32
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata)) return false;
#endif
	return true;
}

void LxcSocket::cleanup()
{
#ifdef WIN32
	WSACleanup();
#endif
}

bool LxcSocket::socket(int af, int type, int proto)
{
	m_socket = ::socket(af, type, proto);
	return INVALID_SOCKET != m_socket;
}

bool LxcSocket::bind(unsigned long ip, unsigned short port)
{
	struct sockaddr_in addr;
	int len = sizeof(addr);
	memset(&addr, 0, len);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(ip);
	addr.sin_port = htons(port);
	return 0 == ::bind(m_socket, (const struct sockaddr*)&addr, len);
}

bool LxcSocket::bind(const char* ip, unsigned short port)
{
	struct sockaddr_in addr;
	pton(ip, port, &addr);
	return 0 == ::bind(m_socket, (const struct sockaddr*)&addr, sizeof(addr));
}

bool LxcSocket::listen(int max)
{
	return 0 == ::listen(m_socket, max);
}

SOCKET LxcSocket::accept(struct sockaddr_in& address)
{
	memset(&address, 0, sizeof(address));
	int addrlen = sizeof(address);
	return ::accept(m_socket, (struct sockaddr*)&address, &addrlen);
}

SOCKET LxcSocket::accept()
{
	return ::accept(m_socket, NULL, NULL);
}

bool LxcSocket::connect(const char* ip, unsigned short port)
{
	struct sockaddr_in addr;
	pton(ip, port, &addr);
	return 0 == ::connect(m_socket, (const struct sockaddr*)&addr, sizeof(addr));
}

bool LxcSocket::shutdown(int how)
{
	return 0 == ::shutdown(m_socket, how);
}

void LxcSocket::close()
{
#ifdef WIN32
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
#else
	close(m_socket);
#endif
}

bool LxcSocket::nonBlock(bool enable)
{
#ifdef WIN32
	unsigned long value = enable ? 1 : 0;
	return SOCKET_ERROR == ::ioctlsocket(m_socket, FIONBIO, &value);
#else
#endif
}

bool LxcSocket::wouldBolck(int error)
{
#ifdef WIN32
	if (SOCKET_ERROR == error)
	{
		return WSAEWOULDBLOCK == WSAGetLastError();
	}
#else
#endif	
	return false;
}

int LxcSocket::read(char* buf, int len)
{
	return recv(m_socket, buf, len, 0);
}

int LxcSocket::write(const char* buf, int len)
{
	return send(m_socket, buf, len, 0);
}

bool LxcSocket::writeAll(const char* buf, int len)
{
	int leftLen = len;
	const char* ptr = buf;
	while (leftLen > 0)
	{
		int actual = send(m_socket, buf, len, 0);
		if (actual == SOCKET_ERROR)
		{
			continue;
		}
		if (actual <= 0)
		{
			return false;
		}
		leftLen -= actual;
		ptr += actual;
	}
	return true;
}

void LxcSocket::pton(const char* ip, unsigned short port, struct sockaddr_in* addr)
{
	memset(addr, 0, sizeof(struct sockaddr_in));
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = inet_addr(ip);
	addr->sin_port = htons(port);
}

bool LxcSocket::peername(struct sockaddr_in& peer)
{
	int len = sizeof(peer);
	return 0 == getpeername(m_socket, (struct sockaddr*)&peer, &len);
}

bool LxcSocket::localname(struct sockaddr_in& local)
{
	int len = sizeof(local);
	return 0 == getsockname(m_socket, (struct sockaddr*)&local, &len);
}
