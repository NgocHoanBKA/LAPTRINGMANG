#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>

int main()
{
    struct addrinfo *result, *p;
    int ret = getaddrinfo("hust.edu.vn", "http", NULL, &result);

    if(ret != 0){
        printf("Failed to get IP: %s\n", gai_strerror(ret));
        return 1;
    }
    printf("Right ret !\n");
    
}