// Example 2: Sockets + overload API for better control

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
        char buf[1024] = { 0 };
        minihttp::SSLResult sr = verifySSL(buf, sizeof(buf));
        printf("SSL status flags (0 is good): 0x%x. Info: %s\n", sr, buf);
        minihttp::HttpSocket::_OnOpen();
    }

    virtual void _OnRequestDone()
    {
        printf("_OnRequestDone(): %s\n", GetCurrentRequest().resource.c_str());
        // Do *NOT* call close() in here!
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

    // minihttp::Download() in example 1 does this automatically,
    // but if this function is not used this has to be called first to init networking properly.
    minihttp::InitNetwork();
    atexit(minihttp::StopNetwork);

    HttpDumpSocket *ht = new HttpDumpSocket;
    ht->SetKeepAlive(3);
    ht->SetBufsizeIn(64 * 1024);
    ht->SetUserAgent("minihttp"); // Let the server know who we are. This is optional but it seems that some servers check that this is set.

    // HTTP GET
    ht->Download("example.com");
    ht->Download("http://www.ietf.org/rfc/rfc2616.txt");
    // Downloads requested in succession will be queued and processed one after another

    // HTTP GET with SSL, if SSL support is enabled:
    ht->Download("https://example.com"); // SSL connection
    ht->Download("raw.githubusercontent.com/fgenesis/minihttp/master/minihttp.h"); // transparent HTTP -> HTTPS redirection

    // Example HTTP POST request:
    minihttp::POST post;
    post.add("a", "b");
    post.add("x", "y");
    post.add("long string", "possibly invalid data: /x/&$+*#'?!;");
    post.add("normal", "data");
    ht->Download("https://httpbin.org/post", NULL, NULL, &post);

    minihttp::SocketSet ss;

    ss.add(ht, true); // true: Delete socket if closed and no task left.

    // This is non-blocking and could be done in background or by another thread.
    // Hogs quite some CPU doing it this way, though.
    while(ss.size())
        ss.update();

    return 0;
}
