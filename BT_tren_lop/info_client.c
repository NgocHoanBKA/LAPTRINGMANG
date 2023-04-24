#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define BUFF_SIZE 1024

typedef struct
{
    char TenMT[50];
    char TenODia[40];
    float SoODia;
    float DungLuongODia;
} MayTinh;

int main(int argc, char *argv[]){
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("connect() failed");
        return 1;
    }

    char buf[BUFF_SIZE];
    while (1)
    {
        printf("Nhap ten may tinh:");
        fgets(buf, sizeof(buf), stdin);

        // send(client, buf, strlen(buf), 0);

        if (strncmp(buf, "exit", 4) == 0)
            break;

        // Nhập thông tin may tinh
        MayTinh mt;
        printf("O Dia: ");
        fgets(mt.TenODia, 40, stdin);
        printf("So luong o dia: ");
        scanf("%f", &mt.SoODia);
        printf("Dung luong o dia: ");
        scanf("%f", &mt.DungLuongODia);
        getchar(); // Nhập vào kí tự '\n' ở cuối

        // Đóng gói dữ liệu
        char buf[BUFF_SIZE];
        int offset = 0;
        memcpy(buf + offset, mt.TenMT, strlen(mt.TenMT));
        offset += strlen(mt.TenMT);
        memcpy(buf + offset, mt.TenODia, strlen(mt.TenODia));
        offset += strlen(mt.TenODia);
        memcpy(buf + offset, &mt.SoODia, sizeof(float));
        offset += sizeof(float);
        memcpy(buf + offset, &mt.DungLuongODia, sizeof(float));
        offset += sizeof(float);

        // Gửi dữ liệu tới server
        if (send(client, buf, offset, 0) < 0)
        {
            perror("Failed to send data to server");
            return 1;
        }
    }

    close(client);
}