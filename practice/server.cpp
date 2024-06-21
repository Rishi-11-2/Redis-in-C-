#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>

static void msg(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

static void errmsg(const char *msg) {
    fprintf(stderr, "[%d] %s: %s\n", errno, msg, strerror(errno));
    abort();
}

static void do_something(int confd) {
    char rbuf[64] = {};
    ssize_t n = read(confd, rbuf, sizeof(rbuf) - 1);

    if (n < 0) {
        msg("read() error");
        return;
    }

    printf("client says: %s\n", rbuf);

    const char *wbuf = "world of dark";
    write(confd, wbuf, strlen(wbuf));
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        errmsg("socket");
    }

    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);  // htons for network byte order
    addr.sin_addr.s_addr = htonl(INADDR_ANY);  // htonl for network byte order

    int rv = bind(fd, (const struct sockaddr*)&addr, sizeof(addr));
    if (rv) {
        errmsg("bind");
    }

    rv = listen(fd, SOMAXCONN);
    if (rv) {
        errmsg("listen");
    }

    while (true) {
        struct sockaddr_in client_addr = {};
        socklen_t socklen = sizeof(client_addr);
        int confd = accept(fd, (struct sockaddr*)&client_addr, &socklen);

        if (confd < 0) {
            msg("accept() error");
            continue;
        }

        do_something(confd);
        close(confd);
    }
    close(fd);
    return 0;
}
