#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int serverPort = 8080; // localhost:8080 (Önerilen port)

char webpage[] = 
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html>"
"<html>"
"<head><title>C Server</title></head>"
"<body>Merhaba bu yazı yazılımın çalıştığının kanıtıdır.</body>"
"</html>"; 

int main(int argc, char *argv[])
{
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_len = sizeof(client_addr);
    int mbs_server, mbs_client;
    char buf[2048];
    int fdimg;
    int on = 1;

    mbs_server = socket(AF_INET, SOCK_STREAM, 0);
    if(mbs_server < 0)
    {
        perror("socket");
        exit(1);
    }

    setsockopt(mbs_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(serverPort);

    if(bind(mbs_server, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(mbs_server);
        exit(1);
    }

    if(listen(mbs_server, 10) == -1)
    {
        perror("listen");
        close(mbs_server);
        exit(1);
    }

    while (1)
    {
        
        mbs_client = accept(mbs_server, (struct sockaddr *) &server_addr, &sin_len);

        if (mbs_client == -1)
        {   
            perror("Connection failed... \n");
        }
        printf("Got client connection... \n");
        
        if (!fork())
        {
            close(mbs_server);
            memset(buf, 0, 2048);
            read(mbs_client, buf, 2047);

            printf("%s \n", buf);
            
            write(mbs_client, webpage, sizeof(webpage) - 1); // Ekrana bastır

            close(mbs_client);
            printf("closing... \n");
            exit(0);
        }
        close(mbs_client);
    }
    return 0;
}
