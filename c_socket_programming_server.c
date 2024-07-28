#include "header.h"


struct AcceptedSocket
{
    int _acceptedSocketFD;
    struct sockaddr_in _address;
    int _error;
    bool _acceptedSuccessfully;
};


struct AcceptedSocket* acceptIncommingConnection(int serverSockFD);
void acceptNewConnectionAndReciveAndPrintItsData(int serverSockFD);
void reciveAndPrintIncommingData(int socketFD);
void startAcceptingIncommingData(int serverSockFD);
void reciveAndPrintingDataOnSeparateThread(struct AcceptedSocket* pSocket);
void sendRecivedMessageToTheOtherClients(char* buffer, int socketFD);
struct AcceptedSocket* acceptedSockets[10];
int acceptedSocketsCount = 0;


void startAcceptingIncommingData(int serverSockFD)
{
    while (true)
    {
        struct AcceptedSocket* clientSocket = acceptIncommingConnection(serverSockFD);
        acceptedSockets[acceptedSocketsCount++] = clientSocket;
        reciveAndPrintingDataOnSeparateThread(clientSocket);
    }
}


struct AcceptedSocket * acceptIncommingConnection(serverSockFD)
{
    struct sockaddr_in clientAddress;
    int clientAddressSize = sizeof(struct sockaddr_in);
    int clientSocketFD = accept(serverSockFD, (struct sockaddr*)&clientAddress, &clientAddressSize);
    struct AcceptedSocket* acceptedSocket = malloc(sizeof(struct AcceptedSocket));
    acceptedSocket->_address = clientAddress;
    acceptedSocket->_acceptedSocketFD = clientSocketFD;
    acceptedSocket->_acceptedSuccessfully = clientSocketFD > 0;
    if (!acceptedSocket->_acceptedSuccessfully)
    {
        acceptedSocket->_error = clientSocketFD;

    }
    return acceptedSocket;
}


void reciveAndPrintingDataOnSeparateThread(struct AcceptedSocket* pSocket)
{
        pthread_t id;
        pthread_create(&id, NULL, reciveAndPrintIncommingData, pSocket->_acceptedSocketFD);
}


void reciveAndPrintIncommingData(int socketFD)
{
    char buffer[1024];
    while (true)
    {
        ssize_t amountRecive = recv(socketFD, buffer, 1024, 0);
        if(amountRecive > 0)
        {
            buffer[amountRecive] = 0;
            printf("RECIVED: %s\n", buffer);
            sendRecivedMessageToTheOtherClients(buffer, socketFD);
        }
        if(amountRecive == 0)
        {
            break;
        }
    }
    close(socketFD);
}


void sendRecivedMessageToTheOtherClients(char* buffer, int socketFD)
{
    for (int i = 0; i < acceptedSocketsCount; i++)
    {
        if(acceptedSockets[i]->_acceptedSocketFD != socketFD)
        {
            send(acceptedSockets[i]->_acceptedSocketFD, buffer, strlen(buffer), 0);
        }
    }
}


int main()
{
    int serverSockFD = socketCreator();
    struct sockaddr_in *serverAddress = addressFormating("127.0.0.1", 8080);
    int resultBind = bind(serverSockFD, (struct sockaddr*)serverAddress, sizeof(*serverAddress));
    int resultListen = listen(serverSockFD, 4);
    startAcceptingIncommingData(serverSockFD);
    shutdown(serverSockFD, SHUT_RDWR);
    return 0;
}
