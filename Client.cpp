#include "Client.h"

Client::Client(pollfd * client_fd) {
    this->client_fd = client_fd;
    this->active = true;
}

void Client::close_connection() {
    cout << "closing connection..." << endl;
    pollfd_mutex.lock();
    if (active) {
        close(client_fd->fd);
        client_fd->fd *= -1;
        active = false;
    }
    pollfd_mutex.unlock();
}