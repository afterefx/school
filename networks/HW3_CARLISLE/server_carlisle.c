/*
 * Name: Christopher Carlisle
 * Date: 4/15/10
 * Subject: Computer Networks
 * email: christopher.c.carlisle@gmail.com
 */
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int SERVER_PORT = 8892; //server port to work on
const int Q_LEN = 5; //number of waiting clients

//Function Prototype
int checkPrime(int);

int main(void)
{
    struct sockaddr_in serv_sin; //holds socket address info for server
    struct sockaddr_in cli_sin; //holds socket address info for client
    char buffer[500]; //buffer to accept data
    int sockListen; //identifies the socket
    int sockAccept; //points to the open socket
    unsigned int addrLen; //holds the size of the socket
    int length; //holds number of bytes received from read on socket

    //Setup socket information
    bzero((char *) &serv_sin, sizeof(serv_sin)); //zero out struct
    serv_sin.sin_family = AF_INET; //address family
    serv_sin.sin_addr.s_addr = INADDR_ANY; //listen on all interfaces and send on default IP
    serv_sin.sin_port = htons(SERVER_PORT); //port number

    //Setup listening socket
    sockListen = socket(PF_INET, SOCK_STREAM, 0); //returns socket descriptor
    if(sockListen < 0) //error on creating socket
    {
        printf("Failed to create listening socket\n");
        exit(1);
    }
    if(bind(sockListen, (struct sockaddr *) &serv_sin, sizeof(serv_sin)) < 0)//bind to socket
    {
        printf("Failed to bind listening socket to address\n");
        exit(1);
    }
    if(listen(sockListen, Q_LEN) < 0) //too many hosts connected
    {
        printf("Failed to listen\n");
        exit(1);
    }

    addrLen = sizeof(cli_sin);//get size of client structure

    //wait for connection requests
    while(1)
    {
        //Accept connection request and return separate socket for comm with client
        sockAccept = accept(sockListen, (struct sockaddr *) &cli_sin, &addrLen);
        if(sockAccept < 0) //failed to accept connection
        {
            printf("Failed to accept connection\n");
            exit(1);
        }
        while(sockAccept >0)
        {
            //read bytes from socket and set length = to number of bytes received
            length = read(sockAccept, buffer, sizeof(buffer));

            if(length > 0)
            {
                int num1, num2; //holds input from user
                //printf("%s\n", buffer); //debug statement

                sscanf(buffer, "%d, %d", &num1, &num2); //parse buffer into separate variables

                //Display client's msg //debug statement
                //printf("\n"); //debug statement

                bzero(buffer, sizeof(buffer)); //zero out buffer

                char temp[500]; //create a temporary array to help construct buffer
                                //buffer not used later on with sprintf b/c
                                //sprintf replaces the string rather than
                                //appends to it.
                bzero(temp, sizeof(temp)); //zero out temp buffer

                int i=0; //iterator 
                if(num1 > num2) //used to decide which number to use as base for incrementation
                {
                    for(i; i+num2 < num1; i++) //for each number between the 2 entered by the user
                    {
                        // check if current number is prime and one is not a prime number
                        if(checkPrime(i+num2) && (i+num2) != 1)
                        {
                            sprintf( temp, "%d,", i+num2); //add # to temp list if true
                            strcat(buffer, temp); //append to buffer
                        }
                    }
                }
                else if(num2 > num1) //used to decide which number to use as base for incrementation
                {
                    for(i; i+num1 < num2; i++) //for each number between the 2 entered by the user
                    {
                        // check if current number is prime and one is not a prime number
                        if(checkPrime(i+num1) && (i+num1) != 1)
                        {
                            sprintf( temp, "%d,", i+num1); //add # to temp list if true
                            strcat(buffer, temp); //append to buffer
                        }
                    }
                }
                else //if they are equal print a flag
                {
                    sprintf(buffer, "-1"); //place error flag on buffer to let client know 
                                           //somethhing went wrong.
                }

                write(sockAccept, buffer, sizeof(buffer)); //write to buffer

            }
            else
                printf("Failed to accept message\n");
                break;
        }
        close(sockAccept);//close the socket
    }
    return 0;
}

/*****************************************************
 * Function name: checkPrime
 * Receives: number to be checked 
 * Post condition: returns 1 (true) or 0(false)
 *                 as to whether or not the number
 *                 is prime
 ****************************************************/
int checkPrime(int number)
{
    int i=0; //iterator

    //check only the first half of the number becuase any number greater than
    //half of the number that we are checking, when multiplied by 2 turns out to
    //be greater than the current number we are checking (e.g. 50/2 = 25
    //26*2=52) no point in going past the halfway mark. But just in case I went
    //1 past ^^
    for(i=2; i < (number/2)+1; i++) //start at 2 and incremnt up to half teh number
    {
        if(number%i == 0) //if number has a remainder of zero then the number is not prime
            return 0; //return false
    }

    return 1; //return true
}

