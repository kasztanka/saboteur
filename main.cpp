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
#include <time.h>
#include <pthread.h>
#include<string>
#include<iostream>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <errno.h>
#include "Client.h"
#include "ClientCommunicator.h"

using namespace std;

#define SERVER_PORT 42010
#define POLL_TIMEOUT 500
#define MAX_FDS 200
#define LISTEN_QUEUE_SIZE 32


struct client_t_data {
    Client * client;
};

void close_connection(pollfd * client_fd) {
    cout << "closing connection..." << endl;
    close(client_fd->fd);
    client_fd->fd *= -1;
}

void * handle_client_message(void * arg) {
    client_t_data * t_data = (client_t_data *) arg;
    Client * client = (*t_data).client;

    ClientCommunicator * clientCommunicator = new ClientCommunicator(client);
    clientCommunicator->handle_client_communicate();

    client->client_fd->events = POLLIN;
}

struct pollfd fds[MAX_FDS];
int nfds = 1;
Client * clients[MAX_FDS];

int main(int argc, char *argv[]) {
    short server_port = SERVER_PORT;
    if (argc > 1) {
        char *p;
        long int conv = strtol(argv[1], &p, 10);
        if (*p == 0 && (unsigned short)conv == conv) {
            server_port = (short)conv;
        }
        else {
            fprintf(stderr, "Invalid argument: %s\n", argv[1]);
            exit(1);
        }
    }

    int listen_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_desc < 0) {
        perror("socket() failed");
        exit(2);
    }

    const int one = 1;
    int rc = setsockopt(listen_desc, SOL_SOCKET,  SO_REUSEADDR, (char*)&one, sizeof(one));
    if (rc < 0) {
        perror("setsockopt() failed");
        close(listen_desc);
        exit(3);
    }

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(server_port);
    rc = bind(listen_desc, (struct sockaddr*)&addr, sizeof(addr));
    if (rc < 0) {
        perror("bind() failed");
        close(listen_desc);
        exit(4);
    }

    rc = listen(listen_desc, LISTEN_QUEUE_SIZE);
    if (rc < 0) {
        perror("listen() failed");
        close(listen_desc);
        exit(5);
    }

    fds[0].fd = listen_desc;
    fds[0].events = POLLIN;

    while (true) {
        rc = poll(fds, nfds, POLL_TIMEOUT);

        if (rc < 0) {
            perror("poll() failed");
            break;
        }

        if (rc == 0) {
            // cout << "poll() timed out" << endl;
            continue;
        }

        if (fds[0].revents & POLLIN) {
            int new_desc = accept(listen_desc, nullptr, nullptr);
            if (new_desc < 0) {
                perror("accept() failed");
                break;
            }
            cout << "new connection accepted..." << endl;
            fds[nfds].fd = new_desc;
            fds[nfds].events = POLLIN;
            fds[nfds].revents = 0;
            clients[nfds] = new Client(&fds[nfds]);
            nfds++;
        }

        for (int i = 1; i < nfds; i++) {
            if (fds[i].revents & POLLERR) {
                cout << "socket error, closing connection..." << endl;
                close_connection(&fds[i]);
            }
            else if (fds[i].revents & POLLIN) {
                client_t_data t_data = {};
                t_data.client = clients[i];
                fds[i].events = 0;
                pthread_t t;
                pthread_create(&t, nullptr, handle_client_message, (void *) &t_data);
            }
        }

        int n = nfds;
        nfds = 1;
        for (int i = 1; i < n; i++) {
            if (fds[i].fd < 0)
                continue;
            fds[nfds] = fds[i];
            clients[nfds] = clients[i];
            nfds++;
        }
    }

    return 0;
}