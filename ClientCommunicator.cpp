#include "ClientCommunicator.h"

struct ConnectionBrokenException : public exception {};
struct ConnectionClosedException : public exception {};

ClientCommunicator::ClientCommunicator(Client * client) {
    this->client = client;
}

void ClientCommunicator::handle_client_message() {
    bool close_conn = false;
    try {
        int message = receive_int(client);
        switch (message) {
            case ClientCommunicator::CHAT_MESSAGE:
                handle_chat_message();
                break;
            default:
                break;
        }
    } catch (ConnectionClosedException &e) {
        close_conn = true;
    } catch (ConnectionBrokenException &e) {
        cout << "read() failed" << endl;
        close_conn = true;
    }
    if (close_conn)
        client->close_connection();
}

int ClientCommunicator::receive_int(Client * client) {
    int temp;
    int result = 0;
    int got, left = sizeof(int);

    client->pollfd_mutex.lock();
    if (client->active) {
        while ((got = read(client->client_fd->fd, &temp, left)) > 0) {
            left -= got;
            result += temp;
            if (left <= 0) break;
        }
    }
    client->pollfd_mutex.unlock();

    result = ntohl(result);
    if (got < 0) throw ConnectionBrokenException();
    else if (got == 0) throw ConnectionClosedException();
    return result;
}

string ClientCommunicator::receive_text(Client * client) {
    int length = receive_int(client);
    int got, buffer_size = 20;
    int all = 0;
    char buffer[buffer_size];
    string text = "";

    client->pollfd_mutex.lock();
    if (client->active) {
        while ((got = read(client->client_fd->fd, buffer, min(buffer_size - 1, length - all))) > 0) {
            buffer[got] = '\0';
            text += buffer;
            all += got;
            if (all >= length) break;
        }
    }
    client->pollfd_mutex.unlock();

    if (got < 0) throw ConnectionBrokenException();
    else if (got == 0) throw ConnectionClosedException();
    return text;
}

void ClientCommunicator::send_buffer(Client * client, char * buffer, int length) {
    int sent, left = length;
    client->pollfd_mutex.lock();
    if (client->active) {
        while ((sent = write(client->client_fd->fd, buffer + (length - left), left)) > 0) {
            left -= sent;
            if (left <= 0) break;
        }
    }
    client->pollfd_mutex.unlock();

    if (sent < 0) throw ConnectionBrokenException();
    else if (sent == 0) throw ConnectionClosedException();
}

void ClientCommunicator::send_int(Client * client, int number) {
    number = htonl(number);
    char * buffer = new char[sizeof(int)]();
    for (int i = 0; i < sizeof(int); i++) {
        buffer[i] = (number >> (i * 8)) & 0xFF;
    }
    send_buffer(client, buffer, sizeof(int));
}

void ClientCommunicator::send_text(Client * client, string text, int length) {
    send_int(client, length);
    char * buffer = new char[length]();
    strncpy(buffer, text.c_str(), length);
    send_buffer(client, buffer, length);
}

void ClientCommunicator::send_int_to_all(vector <Client *> recipients, int number) {
    bool close_conn;
    for (auto &recipient : recipients) {
        close_conn = false;
        try {
            send_int(recipient, number);
        } catch (ConnectionClosedException &e) {
            close_conn = true;
        } catch (ConnectionBrokenException &e) {
            cout << "read() failed" << endl;
            close_conn = true;
        }
        if (close_conn)
            recipient->close_connection();
    }
}

void ClientCommunicator::send_text_to_all(vector <Client *> recipients, string text, int length) {
    bool close_conn;
    for (auto &recipient : recipients) {
        close_conn = false;
        try {
            send_text(recipient, text, length);
        } catch (ConnectionClosedException &e) {
            close_conn = true;
        } catch (ConnectionBrokenException &e) {
            cout << "read() failed" << endl;
            close_conn = true;
        }
        if (close_conn)
            recipient->close_connection();
    }
}

void ClientCommunicator::handle_chat_message() {
    string chat_message = receive_text(client);
    cout << "Sending message: " << chat_message << endl;
    send_int_to_all(client->game->players, ClientCommunicator::CHAT_MESSAGE);
    send_text_to_all(client->game->players, chat_message, chat_message.size());
}