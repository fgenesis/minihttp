// minihttp main.cpp - Sample code how to download a file.
// Released under the WTFPL (See minihttp.h)

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "minihttp.h"

// Overloaded socket class that handles incoming data.
// This one just prints to standard output.
class HttpDumpSocket : public minihttp::HttpSocket
{
public:
    virtual ~HttpDumpSocket() {}

protected:
    virtual void _OnClose()
    {
        puts("_OnClose()");
        minihttp::HttpSocket::_OnClose();
    }
    virtual void _OnOpen()
    {
        puts("_OnOpen()");
        minihttp::SSLResult sr = verifySSL();
        printf("SSL status flags (0 is good): 0x%x\n", sr);
        minihttp::HttpSocket::_OnOpen();
    }

    virtual void _OnRequestDone()
    {
        printf("_OnRequestDone(): %s\n", GetCurrentRequest().resource.c_str());
    }

    virtual void _OnRecv(void *buf, unsigned int size)
    {
        if(!size)
            return;
        printf("===START==[Status:%d, Size:%d]======\n", GetStatusCode(), size);
        fwrite(buf, 1, size, stdout);
        puts("\n===END====================");
    }
};

int main(int argc, char *argv[])
{
#ifdef SIGPIPE
    // On *NIX systems, don't signal writing to a closed socket.
    signal(SIGPIPE, SIG_IGN);
#endif

    minihttp::InitNetwork();
    HttpDumpSocket *ht = new HttpDumpSocket;

    ht->SetKeepAlive(3);

    ht->SetBufsizeIn(64 * 1024);
    //ht->Download("http://www.ietf.org/rfc/rfc2616.txt");
    //ht->Download("http://example.com"); // Queue another one
    //ht->Download("https://example.com"); // SSL connection
    ht->Download("raw.githubusercontent.com/fgenesis/minihttp/master/minihttp.h"); // transparent HTTP -> HTTPS redirection

    minihttp::SocketSet ss;

    ss.add(ht, true); // Delete socket if closed and no task left.

    // This is non-blocking and could be done in background or by another thread.
    // Hogs quite some CPU doing it this way, though.
    while(ss.size())
        ss.update();

    return 0;
}
