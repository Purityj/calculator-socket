#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

int main()
{
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    int num1, num2, result;
    char operator;

    // Create a socket file descriptor
    int sock = 0;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Loop until the user enters 'q' to quit
    while (1)
    {
        printf("Enter an equation (e.g. 5 + 3): ");
        scanf("%d %c %d", &num1, &operator, &num2);

        // Send the user's input to the server
        char request[1024];
        sprintf(request, "%d %c %d", num1, operator, num2);
        sendto(sock, request, strlen(request), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

        // Receive the result from the server
        int valread = recvfrom(sock, buffer, 1024, 0, NULL, NULL);

        // Print the result to the console
        printf("%s", buffer);
    }

    return 0;
}
