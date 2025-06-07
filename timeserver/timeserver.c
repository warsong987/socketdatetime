#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include<time.h>

#define PORT        8080
#define	MAXLINE		4096

int main(int argc, char **argv) 
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    char buffer[MAXLINE];
    time_t ticks;

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0))==0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr *) &address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3)<0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1)
    {
        if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
            perror("accept");
            exit(EXIT_FAILURE);
        }
   
        int valread = read(new_socket, buffer, sizeof(buffer));
        printf("Received from client: %s\n", buffer);

        ticks = time(NULL);
        struct tm tm = *localtime(&ticks);
        char * time_str = ctime(&tm);
        send(new_socket, time_str, sizeof(tm), 0);
        printf("Response sent to client\n");

        close(new_socket);
    }

    return 0;
}

