//
// Created by adi on 11/01/19.
//

#include "ParallelServer.h"

using namespace server_side;

void* openParallelSocket(void* arg) {
    struct myParams *params = (myParams*) arg;
    int sockfd, newsockfd, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Initialize socket structure
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(params->port);

    // Now bind the host address using bind() call.
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    // Now start listening for the clients, here process will
    //  go in sleep mode and will wait for the incoming connection

    listen(sockfd, SOMAXCONN);
    clilen = sizeof(cli_addr);

    bool first = true;
    bool run = true;

    while (run) {

        if (!first) {
            timeval timeout;
            timeout.tv_sec = 10;
            timeout.tv_usec = 0;

            setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout));

            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
            if (newsockfd < 0) {
                if (errno == EWOULDBLOCK) {
                    cout << "timeout!" << endl;
                    run = false;
                    continue;
                } else {
                    perror("other error");
                    exit(3);
                }
            }
        } else {
            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
            if (newsockfd < 0) {
                perror("error");
            }
            first = false;
        }
        cout << "client on port: " << newsockfd << endl;

        params->c->handleClient(newsockfd);

        close(newsockfd);
    }
    close(sockfd);
    return nullptr;
}

void ParallelServer::open(int port, ClientHandler *c) {
    /*this->params->port = port;
    this->params->c = c;
    pthread_t trid;
    pthread_create(&trid, nullptr, openParallelSocket, this->params);*/
}

void ParallelServer::stop() {
}
