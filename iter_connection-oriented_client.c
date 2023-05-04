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

    // Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        printf("Socket creation error\n");
        return -1;
    }

    // Connect to the server
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("Invalid address/ Address not supported\n");
        return -1;
    }

    if (connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connection Failed\n");
        return -1;
    }

    // Loop until the user quits
    while (1)
    {
        // Read the user's input
        printf("Enter an expression: ");
        scanf("%d %c %d", &num1, &operator, &num2);

        // Send the input to the server
        sprintf(buffer, "%d %c %d", num1, operator, num2);
        send(client_socket, buffer, strlen(buffer), 0);

        // Receive the result from the server
        memset(buffer, 0, sizeof(buffer));
        read(client_socket, buffer, 1024);

        // Print the result
        sscanf(buffer, "%d", &result);
        printf("Result: %d\n", result);
    }

    return 0;
}
