#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

int main(int argc, char const *argv[]) {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    // Filling server information
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    while (1) {
        int num1, num2;
        char operator;
        printf("Enter an expression to calculate (e.g. 2+3): ");
        scanf("%d %c %d", &num1, &operator, &num2);

        char message[1024];
        snprintf(message, sizeof(message), "%d%c%d", num1, operator, num2);

        // Send the message to server
        sendto(sock, message, strlen(message), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        printf("Message sent to server.\n");

        // Wait for response from server
        valread = recvfrom(sock, buffer, 1024, 0, NULL, NULL);
        printf("Result received from server: %s\n", buffer);

        // Write result to file
        FILE *fp;
        fp = fopen("results.txt", "a");
        if (fp == NULL) {
            perror("Error opening file.");
        } else {
            fprintf(fp, "%d %c %d = %s\n", num1, operator, num2, buffer);
            fclose(fp);
        }
    }

    return 0;
}
