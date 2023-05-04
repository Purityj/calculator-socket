#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr, client_addr;
    char buffer[1024] = {0};
    int client_addr_len;

    // Create a socket file descriptor
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\nSocket creation error \n");
        return -1;
    }

    // Set server address and port
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // Bind the socket to the server address and port
    if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nBind error \n");
        return -1;
    }

    printf("Server listening on port %d\n", PORT);

    while (1)
    {
        // Receive data from a client
        client_addr_len = sizeof(client_addr);
        int valread = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *)&client_addr, &client_addr_len);

        // Parse the user's input
        int num1, num2, result;
        char operator;
        sscanf(buffer, "%d %c %d", &num1, &operator, &num2);

        // Perform the calculation based on the operator
        switch (operator)
        {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 == 0)
            {
                result = -1;
                printf("Cannot divide by zero\n");
            }
            else
            {
                result = num1 / num2;
            }
            break;
        default:
            result = -1;
            printf("Invalid operator\n");
        }

        // Send the result back to the client
        char response[1024];
        sprintf(response, "Result: %d\n", result);
        sendto(sock, response, strlen(response), 0, (struct sockaddr *)&client_addr, client_addr_len);
    }

    return 0;
}
