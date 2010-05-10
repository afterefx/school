/*
 * Name: Christopher Carlisle
 * Date: 4/15/10
 * Subject: Computer Networks
 * email: christopher.c.carlisle@gmail.com
 */
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    if(argc < 2) //check for appropriate number of cli arguments
    {
        printf("There must be at least one IP address.\n"); //print out instructions 
        return -1; //end program
    }

    struct hostent *hostPtr; //holds host data
    char *ip_addr_dot=argv[1]; //receive cli argument as char array
    u_long ip_addr_long; //holds address in binary format
    char* addr_ptr; //pointer to address in char array
    ip_addr_long = inet_addr(ip_addr_dot); //convert IPv4 dotted decmial notation to int value
    addr_ptr = (char *) &ip_addr_long; //make int value into char array
    hostPtr = gethostbyaddr(addr_ptr, 4, AF_INET); //get host data for address
    if(hostPtr == NULL) //if host is not found
        printf(" Host %s not found\n", ip_addr_dot); //print results to screen
    else 
    {
        printf("The official name of the site is: %s\n", hostPtr->h_name); //print hostname

        int counter=0; //count number of aliases and point to cell position
        if(hostPtr->h_aliases[counter] != NULL)
            printf("====================================\n");//print separator for aliases if they are found
        while(hostPtr->h_aliases[counter] != NULL) //loop through all aliases
        {
            printf("Alias %d of the site is: %s\n", counter+1, hostPtr->h_aliases[counter]);//print alias name
            counter++; //increment number of aliases
        }
    }

    return 0; //exit
}

