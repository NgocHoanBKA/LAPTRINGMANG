#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

int main() 
{
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))) 
    {
        perror("bind() failed");
        return 1;
    }

    if (listen(listener, 5)) 
    {
        perror("listen() failed");
        return 1;
    }

    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (client == -1)
    {
        perror("accept() failed");
        return 1;
    }
    printf("New client connected: %d\n", client);

    // Truyen nhan du lieu
    char buf[256];
    char TenMT[50];
    char TenODia[64];
    float SoODia;
    float DungLuongODia;

    FILE *f = fopen("mt_log.txt", "a");

    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        
        // Xu ly du lieu nhan duoc
        buf[ret] = 0;

        // Tach cac thong tin tu xau ky tu
        memcpy(TenMT, buf, 8);
        TenMT[8] = 0;

        int TenMT_len = ret - 25;
        memcpy(TenMT, buf + 9, TenMT_len);
        TenMT[TenMT_len] = 0;

        memcpy(TenODia, buf + TenMT_len + 10, 10);
        TenODia[10] = 0;


        printf("TenMT: %s\nSoODia: %.2f\nTenODia: %s\nDungLuongODia: %.2f", 
            TenMT, SoODia, TenODia, DungLuongODia);


        fprintf(f, "%s %s\n",
            inet_ntoa(clientAddr.sin_addr),
            buf);
    }

    fclose(f);

    close(client);  
    close(listener);    

    return 0;
}