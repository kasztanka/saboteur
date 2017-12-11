#include "Client.h"

Client::Client(pollfd * client_fd) {
    this->client_fd = client_fd;
}