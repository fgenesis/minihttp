/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details. */ 

#ifndef MINIHTTPSOCKET_H
#define MINIHTTPSOCKET_H


// ---- Compile config -----
#define MINIHTTP_SUPPORT_HTTP
#define MINIHTTP_SUPPORT_SOCKET_SET
// -------------------------



#include <string>

namespace minihttp
{

bool InitNetwork();
void StopNetwork();

bool SplitURI(const std::string& uri, std::string& host, std::string& file);


class TcpSocket
{
public:
    TcpSocket();
    virtual ~TcpSocket();

    bool open(const char *addr = NULL, unsigned int port = 0);
    void close();
    bool update(); // returns true if something interesting happened (incoming data, closed connection, etc)

    bool isOpen(void);

    void SetBufsizeIn(unsigned int s);
    bool SetNonBlocking(bool nonblock);
    unsigned int GetBufSize() { return _inbufSize; }
    const char *GetHost(void) { return _host.c_str(); }
    bool SendBytes(const char *str, unsigned int len);

protected:

    virtual void _OnData(); // data received callback. Internal, should only be overloaded to call _OnRecv()

    virtual void _OnRecv(char *buf, unsigned int size) = 0;
    virtual void _OnClose() {}; // close callback
    virtual void _OnOpen() {} // called when opened
    virtual bool _OnUpdate() { return true; } // called before reading from the socket

    void _ShiftBuffer();

    char *_inbuf;
    char *_readptr; // part of inbuf, optionally skipped header
    char *_writeptr; // passed to recv(). usually equal to _inbuf, but may point inside the buffer in case of a partial transfer.

    unsigned int _inbufSize; // size of internal buffer
    unsigned int _writeSize; // how many bytes can be written to _writeptr;
    unsigned int _recvSize; // incoming size, max _inbufSize - 1

    unsigned int _lastport; // port used in last open() call

    bool _nonblocking; // Default true. If false, the current thread is blocked while waiting for input.

    intptr_t _s; // socket handle. really an int, but to be sure its 64 bit compatible as it seems required on windows, we use this.

    std::string _host;
};

} // end namespace minihttp


// ------------------------------------------------------------------------

#ifdef MINIHTTP_SUPPORT_HTTP

#include <map>
#include <queue>

namespace minihttp
{

enum HttpCode
{
    HTTP_NULL = 0,       // used as a generic "something in the code is wrong" indicator, or initial value
    HTTP_OK = 200,
    HTTP_NOTFOUND = 404,
};

struct Request
{
    Request() : user(NULL) {}
    Request(const std::string& r, void *u = NULL) 
        : resource(r), user(u) {}

    std::string header; // set by socket
    std::string resource;
    void *user;
};

class HttpSocket : public TcpSocket
{
public:

    HttpSocket();
    virtual ~HttpSocket();

    void SetKeepAlive(unsigned int secs) { _keep_alive = secs; }
    void SetUserAgent(const std::string &s) { _user_agent = s; }
    void SetAcceptEncoding(const std::string& s) { _accept_encoding = s; }
    void SetFollowRedirect(bool follow) { _followRedir = follow; }

    bool SendGet(Request& what, bool enqueue);
    bool SendGet(const std::string what, void *user = NULL);
    bool QueueGet(const std::string what, void *user = NULL);

    unsigned int GetRemaining(void) { return _remaining; }

    unsigned int GetStatusCode(void) { return _status; }
    unsigned int GetContentLen(void) { return _contentLen; }
    bool ChunkedTransfer(void) { return _chunkedTransfer; }
    bool ExpectMoreData(void) { return _remaining || _chunkedTransfer; }

    Request *GetCurrentRequest() { return _inProgress ? &_curRequest : NULL; }

protected:

    virtual void _OnData(); // data received callback. Internal, should only be overloaded to call _OnRecv()
    virtual void _OnRecv(char *buf, unsigned int size) = 0;
    virtual void _OnOpen(); // called when opene
    virtual bool _OnUpdate(); // called before reading from the socket

    // new ones:
    virtual void _OnRequestDone() {}

    void _ProcessChunk(void);
    bool _EnqueueOrSend(const Request& req, bool forceQueue = false);
    void _DequeueMore(void);
    void _ParseHeader(void);
    void _FinishRequest(void);

    std::string _user_agent;
    std::string _accept_encoding;
    std::string _tmpHdr; // used to save the http header if the incoming buffer was not large enough

    unsigned int _keep_alive; // http related
    unsigned int _remaining; // http "Content-Length: X" - already recvd. 0 if ready for next packet.
                             // For chunked transfer encoding, this holds the remaining size of the current chunk
    unsigned int _contentLen; // as reported by server
    unsigned int _status; // http status code, HTTP_OK if things are good

    std::queue<Request> _requestQ;
    Request _curRequest;

    bool _inProgress;
    bool _chunkedTransfer;
    bool _mustClose; // keep-alive specified, or not
    bool _followRedir; // Default true. Follow 3xx redirects if this is set.
};

} // end namespace minihttp

#endif

// ------------------------------------------------------------------------

#ifdef MINIHTTP_SUPPORT_SOCKET_SET

#include <set>

namespace minihttp
{

class SocketSet
{
public:
    virtual ~SocketSet();
    void deleteAll(void);
    bool update(void);
    void add(TcpSocket *s);
    void remove(TcpSocket *s);
    inline size_t size(void) { return _store.size(); }

protected:
    std::set<TcpSocket*> _store;
};

#endif


} // end namespace minihttp


#endif
