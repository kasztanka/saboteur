#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include<string>
#include<iostream>
#include <algorithm>

using namespace std;

#define SERVER_PORT 42010
#define QUEUE_SIZE 5



int receive_int(int socket) {
    int temp, result = 0;
    int got, left = sizeof(int);
    while ((got = recv(socket, &temp, left, 0)) > 0) {
        left -= got;
        result += temp;
    }
    return result;
}

string receive_text(int socket) {
    int length = receive_int(socket);
    cout << length << endl;
    string temp;
    int got, buffer_size = 20;
    int all = 0;
    char buffer[buffer_size];
    while ((got = recv(socket, buffer, min(buffer_size - 1, length - all), 0)) > 0) {
        buffer[got] = '\0';
        temp += buffer;
        all += got;
        if (all >= length) break;
    }
    return temp;
}


int main(int argc, char* argv[])
{
    int server_socket_descriptor;
    int connection_socket_descriptor;
    int bind_result;
    int listen_result;
    char reuse_addr_val = 1;
    struct sockaddr_in server_address;

    memset(&server_address, 0, sizeof(struct sockaddr));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(SERVER_PORT);

    server_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_descriptor < 0)
    {
        fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda..\n", argv[0]);
        exit(1);
    }
    setsockopt(server_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse_addr_val, sizeof(reuse_addr_val));

    bind_result = bind(server_socket_descriptor, (struct sockaddr*)&server_address, sizeof(struct sockaddr));
    if (bind_result < 0)
    {
        fprintf(stderr, "%s: Błąd przy próbie dowiązania adresu IP i numeru portu do gniazda.\n", argv[0]);
        exit(1);
    }

    listen_result = listen(server_socket_descriptor, QUEUE_SIZE);
    if (listen_result < 0) {
        fprintf(stderr, "%s: Błąd przy próbie ustawienia wielkości kolejki.\n", argv[0]);
        exit(1);
    }

    while(1)
    {
        connection_socket_descriptor = accept(server_socket_descriptor, NULL, NULL);
        if (connection_socket_descriptor < 0)
        {
            fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda dla połączenia.\n", argv[0]);
            exit(1);
        }
        write(connection_socket_descriptor, "abc", 3);
        int result = receive_int(connection_socket_descriptor);
        cout << "odebrano int: " << result << endl;
        string text = receive_text(connection_socket_descriptor);
        cout << "odebrano: " << text << endl;
        text = receive_text(connection_socket_descriptor);
        cout << "odebrano: " << text << endl;
    }

    close(server_socket_descriptor);
    return(0);
}