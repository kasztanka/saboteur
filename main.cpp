#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<string>
#include<iostream>
#include <sys/poll.h>
#include <vector>
#include "Client.h"
#include "ClientCommunicator.h"

using namespace std;

#define POLL_TIMEOUT 500
#define MAX_FDS 200
#define LISTEN_QUEUE_SIZE 32


struct client_t_data {
    Client * client;
    pollfd * client_fd;
    vector<Game *> * games;
};

void * handle_client_message(void * arg) {
    client_t_data * t_data = (client_t_data *) arg;
    Client * client = (*t_data).client;
    pollfd * client_fd = (*t_data).client_fd;
    vector<Game *> * games = (*t_data).games;

    ClientCommunicator * clientCommunicator = new ClientCommunicator(client, games);
    clientCommunicator->handle_client_message();

    client->client_fd->events = POLLIN;
    client_fd->events = POLLIN;
}

struct pollfd fds[MAX_FDS];
struct pollfd * fds_pointers[MAX_FDS];
int nfds = 1;
Client * clients[MAX_FDS];
vector<Game *> games;

int main(int argc, char *argv[]) {
    char key[21];
    int value, SERVER_PORT, ROOM_SIZE;
    FILE * config_file = fopen("config.ini", "r");
    while (feof(config_file) == 0) {
        fscanf(config_file, "%s = %d", key, &value);
        key[21] = '\0';
        if (strcmp(key, "port") == 0)
            SERVER_PORT = value;
        else if (strcmp(key, "room_size") == 0)
            ROOM_SIZE = value;
    }
    fclose(config_file);
    cout << "Room size: " << ROOM_SIZE << endl << "Server port: " << SERVER_PORT << endl;

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
    addr.sin_port = htons(SERVER_PORT);
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
            fds_pointers[nfds] = new pollfd();
            (*fds_pointers[nfds]).fd = new_desc;
            (*fds_pointers[nfds]).events = POLLIN;
            (*fds_pointers[nfds]).revents = 0;
            clients[nfds] = new Client(fds_pointers[nfds]);
            fds[nfds] = *(clients[nfds]->client_fd);
            nfds++;
        }

        for (int i = 1; i < nfds; i++) {
            if (fds[i].revents & POLLERR) {
                cout << "socket error" << endl;
                clients[i]->close_connection();
            }
            else if (fds[i].revents & POLLIN) {
                client_t_data t_data = {};
                t_data.client = clients[i];
                t_data.client_fd = &fds[i];
                t_data.games = &games;
                clients[i]->client_fd->events = 0;
                fds[i].events = 0;
                pthread_t t;
                pthread_create(&t, nullptr, handle_client_message, (void *) &t_data);
            }
        }

        int n = nfds;
        nfds = 1;
        for (int i = 1; i < n; i++) {
            clients[i]->pollfd_mutex.lock();
            if (clients[i]->client_fd->fd >= 0) {
                fds[nfds] = *(clients[i]->client_fd);
                clients[nfds] = clients[i];
                nfds++;
            }
            clients[i]->pollfd_mutex.unlock();
        }
    }

    return 0;
}