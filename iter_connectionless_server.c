#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

int main()
{
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[1024] = {0};
    int num1, num2, result;
    char operator;
    socklen_t addr_len = sizeof(cli_addr);

    // Create a socket
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0)
    {
        printf("Socket creation error\n");
        return -1;
    }

    // Bind the socket to a port
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Bind failed\n");
        return -1;
    }

    // Loop until the program is terminated
    while (1)
    {
        // Receive the expression from the client
        memset(buffer, 0, sizeof(buffer));
        recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&cli_addr, &addr_len);
        printf("Received expression: %s\n", buffer);

        // Parse the expression
        sscanf(buffer, "%d %c %d", &num1, &operator, &num2);

        // Calculate the result
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
                printf("Invalid operator\n");
                continue;
        }

        // Print the result
        printf("Result: %d\n", result);

        // Write the result to a file
        FILE *fp = fopen("results.txt", "a");
        if (fp != NULL)
        {
            fprintf(fp, "%d %c %d = %d\n", num1, operator, num2, result);
            fclose(fp);
        }
        else
        {
            printf("Failed to open file\n");
        }

        // Send the result to the client
        sprintf(buffer, "%d", result);
        sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&cli_addr, addr_len);
    }

    return 0;
}
