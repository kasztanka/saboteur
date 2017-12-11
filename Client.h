#ifndef SABOTEUR_SERVER_CLIENT_H
#define SABOTEUR_SERVER_CLIENT_H

#include <sys/poll.h>
#include <string>
using namespace std;

class Client {
public:
    pollfd * client_fd;
    string username;
    Client(pollfd * client_fd);
};


#endif //SABOTEUR_SERVER_CLIENT_H
