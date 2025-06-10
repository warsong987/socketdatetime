#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/tcp.h>

#define PORT        8080
#define	MAXLINE		4096

union val {
    int i_val;
    
} val;

char *(*opt_val_str)(union val *, int);

static char strres[128];

static char	* sock_str_int(union val *ptr, int len);

static char	* sock_str_int(union val *ptr, int len)
{
	if (len != sizeof(int))
		snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
	else
		snprintf(strres, sizeof(strres), "%d", ptr->i_val);
	return(strres);
}

int main() {
    int sock = 0;
    socklen_t len;
    struct sockaddr_in serv_addr;
    char buffer[MAXLINE];


    if((sock = socket(AF_INET, SOCK_STREAM, 0))<0){
        printf("\n Socket creation error \n");
        return -1;
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    len = sizeof(val);
    if(getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &val,  &len)==-1)
    {
        perror("get params failed");
    } 
    else
    {
        printf("buffer size default = %s\n",  sock_str_int(&val, len));
    }
    
    if(getsockopt(sock, IPPROTO_TCP, TCP_MAXSEG, &val, &len)==-1)
    {
        perror("get params failed");
        return -1;
    } 
    else
    {
        printf("MSS default = %s\n",  sock_str_int(&val, len));
    }

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){
        printf("\nConnection Failed \n");
        return -1;
    }

    if(getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &val,  &len)==-1){
        perror("get params failed");
        return -1;
    } 
    else
    {
        printf("buffer size  changed = %s\n",  sock_str_int(&val, len));
    }

    if(getsockopt(sock, IPPROTO_TCP, TCP_MAXSEG, &val,  &len)==-1){
        perror("get params failed");
        return -1;
    } 
    else
    {
        printf("MSS changed = %s\n",  sock_str_int(&val, len));
    }

    char *message = "Hello from client";
    send(sock, message, strlen(message), 0);
    printf("Message sent to server\n");

    int valread = read(sock, buffer, sizeof(buffer));
    printf("Server response: %s\n", buffer);

    close(sock);

    return 0;
}