#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define MAX_BUFFER 1024

void getCpuInfo(char *string)
{
    FILE *cpu_name = popen("lscpu | grep 'Model name:' | sed -r 's/Model name:\\s{1,}//g'", "r");
    fgets(string, MAX_BUFFER, cpu_name);
    fclose(cpu_name);
}

void getHostname(char *string)
{
    FILE *hostname = popen("cat /proc/sys/kernel/hostname", "r");
    fgets(string, MAX_BUFFER, hostname);
    fclose(hostname);     
}

void getCpuLoad(char *string)
{
    //FILE *cpu_load = popen("grep 'cpu ' /proc/stat > file1 | sleep 1 && grep 'cpu ' /proc/stat > file2 | cat file1 file2 |awk -v RS=\"\" '{print ($13-$2+$15-$4)*100/($13-$2+$15-$4+$16-$5) \"%\"}' ", "r");

    FILE *cpu_load = popen("grep 'cpu ' /proc/stat | sleep 1 && grep 'cpu ' /proc/stat | awk -v RS=\"\" '{print ($13-$2+$15-$4)*100/($13-$2+$15-$4+$16-$5) \"%\"}' ", "r");

    fgets(string, MAX_BUFFER, cpu_load);
    fclose(cpu_load);
}

int main(int argc, char* argv[])
{
    int port = atoi(argv[1]);

    int server_fd, new_socket;

    FILE *request;

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addrlen;

    char message[MAX_BUFFER];

    if(argc < 2)
    {
        fprintf(stderr, "Port failed\n");
        exit(EXIT_FAILURE); 
    }

    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
    {
        perror("setsockopt(SO_REUSEADDR) failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);

    if(bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 5) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    client_addrlen = sizeof(client_addr);
    
    while(1)
    {
        new_socket = accept(server_fd, (struct sockaddr *) &client_addr, &client_addrlen);
        if(new_socket < 0)
        {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        char buffer[MAX_BUFFER] = {0};

        int valread = read(new_socket, buffer, MAX_BUFFER);
        if(valread < 0)
        {
            perror("reading from socket failed");
        }

        if(strstr(buffer, "hostname") != NULL)
            getHostname(message);

        else if(strstr(buffer, "cpu-name") != NULL)
            getCpuInfo(message);    

        else if(strstr(buffer, "load") != NULL)
            getCpuLoad(message);
        
        valread = write(new_socket, message, strlen(message));

        if(valread < 0)
        {
            perror("writing to socket failed");
            exit(EXIT_FAILURE);
        }
        close(new_socket);
    }
    close(server_fd);
    return 0;
}