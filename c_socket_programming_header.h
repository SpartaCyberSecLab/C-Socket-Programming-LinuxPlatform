#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <malloc.h>
#include <pthread.h>


/*--------------------------------------------------------------------------------------------
SERVER-SIDE and CLIENT-SIDE
--------------------------------------------------------------------------------------------*/
int socketCreator();
int socketCreator()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}
/*--------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------
SERVER-SIDE and CLIENT-SIDE
--------------------------------------------------------------------------------------------*/

struct sockaddr_in* addressFormating(char *ip, int port);
struct sockaddr_in* addressFormating(char *ip, int port)
{   
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    if(strlen(ip) == 0)
    {
        address->sin_addr.s_addr = INADDR_ANY;
    }else{
        inet_pton(AF_INET, ip, &address->sin_addr.s_addr);
    }
    return address;
}
/*--------------------------------------------------------------------------------------------*/
