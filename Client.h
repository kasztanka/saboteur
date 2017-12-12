#ifndef SABOTEUR_SERVER_CLIENT_H
#define SABOTEUR_SERVER_CLIENT_H

#include <sys/poll.h>
#include <string>
#include <mutex>
#include <unistd.h>
#include <iostream>
using namespace std;

class Client {
public:
    pollfd * client_fd;
    mutex pollfd_mutex;
    bool active;
    string username;
    Client(pollfd * client_fd);
    void close_connection();
};


#endif //SABOTEUR_SERVER_CLIENT_H
