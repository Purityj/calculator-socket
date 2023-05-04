#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345 

int perform_operation(int num1, int num2, char operator)
{
    switch(operator)
    {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            return num1 / num2;
        default:
            printf("Invalid operator\n");
            return 0;
    }
}

int main()
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    int num1, num2, result;
    char operator;

    // Create a socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to reuse address and port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Bind the socket to the specified address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Loop until the server is terminated
    while (1)
    {
        printf("Waiting for a connection...\n");

        // Accept an incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("Connection accepted\n");

        // Loop until the client disconnects
        while (1)
        {
            // Receive the client's input
            valread = read(new_socket, buffer, 1024);
            if (valread <= 0)
            {
                printf("Client disconnected\n");
                break;
            }

            // Parse the client's input
            sscanf(buffer, "%d %c %d", &num1, &operator, &num2);

            // Perform the requested operation
            result = perform_operation(num1, num2, operator);

            // Send the result back to the client
            char response[1024];
            sprintf(response, "%d\n", result);
            send(new_socket, response, strlen(response), 0);

            printf("Result sent: %d\n", result);
        }

        // Close the connection
        close(new_socket);
    }

    return 0;
}
