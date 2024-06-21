#include<bits/stdc++.h>
#include<bits/stdc++.h>
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



using namespace std;

static void errmsg(const char *msg)
{
    int err=errno;
    fprintf(stderr,"[%d] %s\n",err,msg);
    abort();
}


int main()
{
    int fd=socket(AF_INET,SOCK_STREAM,0);

    if(fd<0)
    {
        errmsg("socket:()");
    }
    struct sockaddr_in addr={};

    addr.sin_family=AF_INET;
    addr.sin_port=ntohs(1234); //0.0.0.0.1234

    addr.sin_addr.s_addr=ntohl(INADDR_LOOPBACK); //127.0.0.1

    int rv=connect(fd,(const struct sockaddr *)&addr,sizeof(addr));

    if(rv)
    {
        errmsg("connet");
    }

    char msg[]="hello";

    write(fd,msg,strlen(msg));

    char rbuf[64]={};

    ssize_t n =read(fd,rbuf,sizeof(rbuf)-1);

    if(n<0)
    {
        errmsg("read");
    }

    printf("server says : %s \n",rbuf);

    close(fd);

    return 0;
}