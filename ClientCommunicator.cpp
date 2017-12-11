#include "ClientCommunicator.h"

struct ConnectionBrokenException : public exception {};
struct ConnectionClosedException : public exception {};

ClientCommunicator::ClientCommunicator(Client * client) {
    this->client = client;
    this->client_descriptor = client->client_fd->fd;
}

void ClientCommunicator::handle_client_communicate() {
    bool close_conn = false;
    try {
        int communicate = receive_int(client_descriptor);
        switch (communicate) {
            case ClientCommunicator::MESSAGE:
                send_int(client_descriptor, ClientCommunicator::MESSAGE);
                handle_message();
                break;
            default:
                break;
        }
    } catch (ConnectionClosedException &e) {
        close_conn = true;
    } catch (ConnectionBrokenException &e) {
        perror("read() failed");
        close_conn = true;
    }
    if (close_conn)
        close_connection(client);
}

int ClientCommunicator::receive_int(int descriptor) {
    int temp;
    int result = 0;
    int got, left = sizeof(int);
    while ((got = read(descriptor, &temp, left)) > 0) {
        left -= got;
        result += temp;
        if (left <= 0) break;
    }
    result = ntohl(result);
    if (got < 0) throw ConnectionBrokenException();
    else if (got == 0) throw ConnectionClosedException();
    return result;
}

string ClientCommunicator::receive_text(int descriptor) {
    int length = receive_int(descriptor);
    int got, buffer_size = 20;
    int all = 0;
    char buffer[buffer_size];
    string text = "";
    while ((got = read(descriptor, buffer, min(buffer_size - 1, length - all))) > 0) {
        buffer[got] = '\0';
        text += buffer;
        all += got;
        if (all >= length) break;
    }
    if (got < 0) throw ConnectionBrokenException();
    else if (got == 0) throw ConnectionClosedException();
    return text;
}

void ClientCommunicator::send_buffer(int descriptor, char * buffer, int length) {
    int sent, left = length;
    while ((sent = write(descriptor, buffer + (length - left), left)) > 0) {
        left -= sent;
        if (left <= 0) break;
    }
    if (sent < 0) throw ConnectionBrokenException();
    else if (sent == 0) throw ConnectionClosedException();
}

void ClientCommunicator::send_int(int descriptor, int number) {
    number = htonl(number);
    char * buffer = new char[sizeof(int)]();
    for (int i = 0; i < sizeof(int); i++) {
        buffer[i] = (number >> (i * 8)) & 0xFF;
    }
    send_buffer(descriptor, buffer, sizeof(int));
}

void ClientCommunicator::send_text(int descriptor, string text, int length) {
    send_int(descriptor, length);
    char * buffer = new char[length]();
    strncpy(buffer, text.c_str(), length);
    send_buffer(descriptor, buffer, length);
}

void ClientCommunicator::close_connection(Client * client) {
    cout << "closing connection..." << endl;
    close(client->client_fd->fd);
    client->client_fd->fd *= -1;
}

void ClientCommunicator::handle_message() {
    string message = receive_text(client_descriptor);
    cout << "Sending message: " << message << endl;
    send_text(client_descriptor, message, message.size());
}