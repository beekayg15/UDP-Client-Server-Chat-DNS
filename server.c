#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<termios.h>
#include<unistd.h>
#define PORT 4455

int main(){
    int server_socket;
    struct sockaddr_in server_address, client_address;
    char buffer[1024];
    socklen_t addr_size;
    addr_size = sizeof(server_address);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if(server_socket < 0) {
        printf("Socket Connection Failed!!\n");
        exit(0);

    } else {
        printf("Socket Connection Established!!\n");

    }

    bzero(&server_address, addr_size);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(server_socket, (struct sockaddr*)&server_address, addr_size) != 0) {
        printf("Error on Binding!!\n");
        exit(0);

    } else {
        printf("Binding Successful!!\n\n");

    }

    while(1>0){
        printf("Recieving Message from Client......\n");
        bzero(buffer, 1024);

        int n = recvfrom(server_socket, buffer, 1024, 0, (struct sockaddr*)&client_address, &addr_size);

        if(strncmp("exit", buffer, 4) == 0) {
            printf("The client has left!!\n");
            break;
        }

        printf("%s", buffer);

        n = 0;
        for(int i=0; buffer[i]!='\n'; i++) {
            n++;

        }

        FILE *f;
        char *msg;
        char sol[1024];
        f = fopen("DNS.txt", "r");
        if(f == NULL) {
            printf("DNS File Cannot be Read!!\n");
            msg = "exit";
            sol[0] = '\n';

        } else {
            printf("DNS File Read Successful!!\n");

        }

        int flag = 1;

        if(buffer[0] > '0' && buffer[0] <= '9') {
            msg = "Requsted Domain is ";

            char domain[1024];
            char ip[1024];
            char ch = getc(f);
            int temp = 0, i = 0, j = 0;

            while(ch != EOF) {
                if(temp) {
                    ip[i++] = ch;

                    if(ch == '\n') {
                        ip[i++] = '\0';

                        if(strncmp(buffer, ip, n) == 0) {
                            printf("Match Found!!\n");
                            for(int z=0; domain[z]!='\0'; z++) {
                                sol[z] = domain[z];
                                
                            }
                            flag = 0;
                            break;

                        }

                        i = 0;
                        j = 0;
                        temp = 0;
                        bzero(domain, 1024);
                        bzero(ip, 1024);

                    }

                } else {
                    domain[j++] = ch;

                    if(ch=='\n') {
                        domain[j++] = '\0';
                        temp = 1;

                    }

                }

                ch = getc(f);

            }

        } else if(buffer[0] >= 'a' && buffer[0] <= 'z') {
            msg = "Requsted IP Address is ";

            char domain[1024];
            char ip[1024];
            char ch = getc(f);
            int temp = 0, i = 0, j = 0;

            while(ch != EOF) {
                if(temp) {
                    ip[i++] = ch;

                    if(ch == '\n') {
                        ip[i++] = '\0';

                        if(strncmp(buffer, domain, n) == 0) {
                            printf("Match Found!!\n");
                            for(int z=0; ip[z]!='\0'; z++) {
                                sol[z] = ip[z];
                                
                            }
                            flag = 0;
                            break;

                        }

                        i = 0;
                        j = 0;
                        temp = 0;
                        bzero(domain, 1024);
                        bzero(ip, 1024);

                    }

                } else {
                    domain[j++] = ch;

                    if(ch == '\n') {
                        domain[j++] = '\0';
                        temp = 1;

                    }

                }

                ch = getc(f);

            }

        } 
        
        if(flag) {
            msg = "Invalid Input or Entry Not Found";
            sol[0] = '\n';
            
        }

        fclose(f);

        bzero(buffer, 1024);

        n = 0;
        for(int i=0; msg[i]!='\0'; i++) {
            buffer[i] = msg[i];
            n++;

        }
        for(int i=0; i==0 || sol[i-1]!='\n'; i++) {
            buffer[n++] = sol[i];

        }

        printf("Message to client: %s\n", buffer);

        sendto(server_socket, buffer, 1024, 0, (struct sockaddr*)&client_address, addr_size);
    
    }

    close(server_socket);

    return 0;

}