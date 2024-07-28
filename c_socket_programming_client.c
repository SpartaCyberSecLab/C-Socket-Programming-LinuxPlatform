#include "header.h"


void listenAndPrint(int socketFD);
void startListenningAndPrintMessagesOnNewThread(int socketFD);

int main()
{
    struct sockaddr_in *address = addressFormating("127.0.0.1", 8080);
    int socketFD = socketCreator();
    int result = connect(socketFD, (struct sockaddr*) address, sizeof(*address));
    char* line = NULL;
    size_t lineSize = 0;
    printf("TYPE YOURE MESSAGE or EXIT(exit): \n");
    startListenningAndPrintMessagesOnNewThread(socketFD);
    while (true)
    {
        printf(">>>");
        ssize_t countLine = getline(&line, &lineSize, stdin);
        if (countLine > 0)
        {
            if (strcmp(line, "exit\n") == 0)
            {
                break;
            }else{

                ssize_t amountWasSent = send(socketFD, line, countLine, 0);
            }            
        }
    }
    close(socketFD);
    return 0;
}


void startListenningAndPrintMessagesOnNewThread(int socketFD)
{
    pthread_t id;
    pthread_create(&id, NULL, listenAndPrint, socketFD);
}


void listenAndPrint(int socketFD)
{
    char buffer[1024];
    while (true)
    {
        ssize_t amountRecive = recv(socketFD, buffer, 1024, 0);
        if(amountRecive > 0)
        {
            buffer[amountRecive] = 0;
            printf("RECIVED: %s\n", buffer);
        }
        if(amountRecive == 0)
        {
            break;
        }
    }
    close(socketFD);
}
