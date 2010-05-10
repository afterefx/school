/*
 * Name: Christopher Carlisle
 * Date: 4/15/10
 * Subject: Computer Networks
 * email: christopher.c.carlisle@gmail.com
 */
#include <stdio.h>
#include <math.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>

//Function prototypes
void addOne(u_long*); //increments to the next network address
int calcHosts(int, int, int, int); //calculates the number of hosts on the network
                                   //for the subnet provided

int main(int argc, char **argv)
{
    //check for arguments from the cli
    if(argc < 3)
    {
        printf("There must be at least one subnet and one IP address.\n");
        return -1;
    }

    //Initialize variables
    int soct1, soct2, soct3, soct4; //used to hold numbers for address between each dot
    int ioct1, ioct2, ioct3, ioct4; //used to hold numbers for address between each dot
    int numHosts; //holds # of hosts in the network to scan for

    //Read in subnet and ip address from cli
    char* sub_addr_dot=argv[1]; //char array of cli input
    char* ip_addr_dot=argv[2]; //char array of cli input

    //Parse strings to get ints to work with
    sscanf(argv[1], "%d.%d.%d.%d", &soct1, &soct2, &soct3, &soct4);
    sscanf(argv[2], "%d.%d.%d.%d", &ioct1, &ioct2, &ioct3, &ioct4);

    //Convert to binary and obtain network address
    u_long ip_addr_long, sub_addr_long, net_addr_long; //holds addresses in binary
    ip_addr_long = inet_addr(ip_addr_dot); //convert ip to binary
    sub_addr_long = inet_addr(sub_addr_dot); //convert subnet to binary
    net_addr_long = inet_addr(ip_addr_dot) & inet_addr(sub_addr_dot); //get network address by bitwise AND

    //Find number of hosts to go through for loop
    numHosts = calcHosts(soct1, soct2, soct3, soct4);

    //move network address to a variable that will change as we go through the
    //loop for the number of hosts we calculated for the subnet
    u_long current_addr_long = net_addr_long;

    int i; //i is used to iterate through the loop
    struct in_addr currentAddy; //this struct is used to print out the current address
    for(i=1; i <= numHosts; i++)
    {
        addOne(&current_addr_long); //move to the next host in the network
        currentAddy.s_addr = current_addr_long; //move current address into struct
        printf("Host: %s\n", inet_ntoa(currentAddy)); //print out current address

        //ip work
        char* addr_ptr; //holds current address as a char array
        struct hostent *hostPtr; //stuct to hold host data
        addr_ptr = (char *) &current_addr_long; //hold address in char array
        hostPtr = gethostbyaddr(addr_ptr, 4, AF_INET); //gets data for host in struct

        if(hostPtr == NULL)
            printf("Host %s not found\n", inet_ntoa(currentAddy)); //print out current address
        else
        {
            printf("The official name of the site is: %s\n", hostPtr->h_name); //prints the official hostname for the site

            int aliasCount=1; //helps output proper number for the alias when printing
            while(*(hostPtr->h_aliases) != NULL) //loops through all aliases and prints them
            {
                printf("Alias %d: %s\n", aliasCount, *(hostPtr->h_aliases)); //print alias
                aliasCount++; //increment number of aliases
                hostPtr->h_aliases++; //go to next alias position
            }
            
            //Display IP addresses
            struct in_addr *addrPtr; //a separate in_addr struct used to print IPs from host info
            while(*(hostPtr->h_addr_list) != NULL) //loop through all IP's
            {
                addrPtr = (struct in_addr *) *hostPtr->h_addr_list; //get ip from host info to in_addr struct
                printf("IP address: %s\n", inet_ntoa(*addrPtr)); //print info for address
                hostPtr->h_addr_list++; //go to the next address
            }
        }

        printf("===================================\n"); //print a separating line for the next host
    }

    //network information
    struct in_addr networkAddy; //used to hold network address
    networkAddy.s_addr = net_addr_long; //store calculated network address
    printf("Network Address: %s\n", inet_ntoa(networkAddy)); //print the network address
    

    return 0;
}

/*****************************************************************
 * Function name: addOne
 * Purpose: incremnt the binary address
 * Receives: unsigned long binary network addres in network order
 * Post condition: Address is incremented by one.
 * ***************************************************************/
void addOne(u_long* net_addr_long)
{
    *net_addr_long = ntohl(*net_addr_long); //net to host long
    *net_addr_long += 1; //add 1 in bitwise fashion
    *net_addr_long = htonl(*net_addr_long); //host to net long
}

/**********************************************************************
 * Function name: calcHosts
 * Purpose: Calculate the number of hosts in a subnet
 * Recieves: 0-255 in each int to calculate the number of hosts.
 *           The order should be num1.num2.num3.num4 for the subnet
 * Post condition: Returns int of the # of hosts to scan for according
 *                 to the subnet
 **********************************************************************/
int calcHosts(int num1, int num2, int num3, int num4)
{
    int numBits=32; //number of bits left for the host to use
    double numberH; //number of hosts calculated at the end of the function

    //a switch used to calculate how many bits to subtract 
    //from 32 so that we can find the number of hosts
    switch(num1) //check first number
    {
        case 255: //if 255 then go to the next 2nd number
            numBits-=8;
            switch(num2) //if 255 then go to the next 3rd number
            {
                case 255:
                    numBits-=8;
                    switch(num3) //if 255 then go to the next 4th number

                    {
                        case 255:
                            numBits-=8;
                            switch(num4)
                            {
                                case 255:
                                    numBits-=8;
                                    break;
                                case 254:
                                    numBits-=7;
                                    break;
                                case 252:
                                    numBits-=6;
                                    break;
                                case 248:
                                    numBits-=5;
                                    break;
                                case 240:
                                    numBits-=4;
                                    break;
                                case 224:
                                    numBits-=3;
                                    break;
                                case 192:
                                    numBits-=2;
                                    break;
                                case 128:
                                    numBits-=1;
                                    break;
                            }
                            break;
                        case 254:
                            numBits-=7;
                            break;
                        case 252:
                            numBits-=6;
                            break;
                        case 248:
                            numBits-=5;
                            break;
                        case 240:
                            numBits-=4;
                            break;
                        case 224:
                            numBits-=3;
                            break;
                        case 192:
                            numBits-=2;
                            break;
                        case 128:
                            numBits-=1;
                            break;
                    }
                break;
            case 254:
                numBits-=7;
                break;
            case 252:
                numBits-=6;
                break;
            case 248:
                numBits-=5;
                break;
            case 240:
                numBits-=4;
                break;
            case 224:
                numBits-=3;
                break;
            case 192:
                numBits-=2;
                break;
            case 128:
                numBits-=1;
                break;
        }
            break;
        case 254:
            numBits-=7;
            break;
        case 252:
            numBits-=6;
            break;
        case 248:
            numBits-=5;
            break;
        case 240:
            numBits-=4;
            break;
        case 224:
            numBits-=3;
            break;
        case 192:
            numBits-=2;
            break;
        case 128:
            numBits-=1;
            break;
    }

    numberH = pow( 2, (double) numBits); //take the number of hosts left and make it the exponent to the power of 2
    numberH -=2; //subtract network address and broadcast address from number of hosts
    
    return (int) numberH; //return the number of hosts

}

