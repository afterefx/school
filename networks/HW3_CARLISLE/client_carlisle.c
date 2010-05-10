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

int main(void)
{
    struct sockaddr_in serv_sin; //socket info for server
    char buffer[500]; //buffer to hold data
    char buffer2[500]; //temp buffer

    int sockListen; //identifies the socket
    int sockAccept;//points to the open socket
    unsigned int addrLen; //or socklen_t addrLen

    //Setup address structure
    bzero((char *) &serv_sin, sizeof(serv_sin)); //zero out socket info
    serv_sin.sin_family = AF_INET; //socket family
    serv_sin.sin_addr.s_addr = INADDR_ANY; //listen on all ports and connect using default IP
    serv_sin.sin_port = htons(SERVER_PORT); //port to work on

    //Setup listening socket
    sockListen = socket(PF_INET, SOCK_STREAM, 0); //create socket

    
    //Connect to server
    int retcode; //holds return code for connection
    retcode = connect(sockListen, (struct sockaddr *) &serv_sin, sizeof(serv_sin)); //make connection

    if(retcode < 0) //if client failed to make connection
    {
        printf("Could not connect to server\n"); //show error
        exit(-1); //end program
    }

    //receive input from the user
    printf("\nPlease enter the first value: ");
    scanf("%s", buffer);
    printf("\nPlease enter the second value: ");
    scanf("%s", buffer2);

    strcat(buffer, ", "); //append delimiter to 1st number
    strcat(buffer, buffer2); //append second number
    
    int num1, num2; //ints to hold input
    sscanf(buffer, "%d, %d", &num1, &num2); //scan buffer for numbers and put into int format


    //send buffer to server
    int n=1; //flag for write command
    n = write(sockListen, buffer, sizeof(buffer));
    if(n < 0) //if error occured
        printf("Could not write to server.\n"); //print error to screen

    //prepare to read
    bzero(buffer, sizeof(buffer)); //zero out buffer and get ready to receive data

    n = read(sockListen, buffer, sizeof(buffer)); //read bytes from socket
    if(n < 0) //print error if nothing was received
        printf("Could not read from server.\n"); 

    if(n > 0)
    {
        int primeCount=1; //used to display the number of the prime
        char* number; //used to point to the string of prime numbers
        number = strtok(buffer, ","); //parse buffer into numbers to be printed out
        if(number != NULL && strcmp(number, "-1"))
        {
            //prints out lowest number first and largest second
            if(num1 < num2) 
                printf("The primes between %d and %d are:\n", num1, num2);
            if(num2 < num1)
                printf("The primes between %d and %d are:\n", num2, num1);

            while(number != NULL) //while there are numbers to be printed
            {
                printf("Prime %d is = %s\n", primeCount, number); //print the prime number
                number = strtok(NULL, ","); //grab next number
                primeCount++; //incremnt number of prime
            }
        }
        else //prints if there are no primes 
        {
                printf("The are no primes between %d and %d\n", num1, num2);
        }

        printf("\n");//place an empty line on the screen
        
    }
    else
        printf("Failed to accept message\n"); //prints if message from server failed
    close(sockAccept); //close socket

    return 0;
}

