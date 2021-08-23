#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<termios.h>
#include<unistd.h>
#define PORT 4455

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[1024];
    socklen_t addr_size;
    addr_size = sizeof(server_address);

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if(client_socket < 0) {
        printf("Socket Connection Failed!!\n");
        exit(0);

    } else {
        printf("Socket Connection Successful!!\n\n");

    }

    bzero(&server_address, addr_size);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    while(1>0) {
        bzero(buffer, 1024);
        int n=0;

        printf("Enter Message to Server: ");
        while((buffer[n++] = getchar()) != '\n');

        sendto(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_address, addr_size);

        if(strncmp("exit", buffer, 4) == 0) {
            printf("You have left the chat room!!\n");
            break;
        }

        printf("Recieving Message from Server......\n");

        bzero(buffer, 1024);
        n = recvfrom(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_address, &addr_size);

        if(strncmp("exit", buffer, 4) == 0) {
            printf("The server has been closed!!\n");
            break;
        }

        printf("%s\n", buffer);

    }

    close(client_socket);

    return 0;

}