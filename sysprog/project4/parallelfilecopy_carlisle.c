/*************************************************
 * Christopher Carlisle
 * COSC 4348
 * HW 4
 * 4/25/2011
 * ccarlisle1@islander.tamucc.edu
 * Katangur
 *
Parallel File Copy from pages 443 – 444 in the textbook.
Name the main program file as “parallelfilecopy_lastname.c”,
and the executable file as parallelfilecopy_lastname.

Do not implement number 8 and 9 in the problem.
For number 7 you can use the gettimeofday() function to obtain the timings.
This function has the capability of providing time information in
microseconds.
You are not allowed to use the functions system() or any version of exec().

Directions
------------
* Take two cli arguments that are directory names and copy everything from
  one directory into another.
* Preserve subdirectory structure.
* Same filenames are used for source and destination.

#COMPLETE
1) void *copydirectory(void *arg)
    copy all the files from one directory into another dir. Dir names
    are passed in arg as two consecutive strings separated by null char.
    Assume both source and destination dir's exist when function is called.
    SUBDIR's are ignored. For each file create a thread to run the
    copyfilepass function from Program 12.7. For this version, wait for
    each thread to complete before creating the next one.

#COMPLETE
2) Write a main program that takes 2 cli args for the source and
   destination dir's. the main program creates a thread to run
   copydirectory and then does a pthread_join to wait for the copydirectory
   thread to complete. Use this program to test the first version of
   copydirectory

#COMPLETE
3) Modify the copydirectory function so that if the destination direcotry
   does not exist, copydirectory creates the directory. Test the new version.

# KINDA COMPLETE
4) Modify copydirectory so that after it creates a thread to copy a
   file, it continues to create threads to copy the other files.
   Keep the thread ID and open file descriptors for each copyfilepass
   thread in a linked list with a node structure similar to the following.

    typedef struct copy_struct {
        char *namestring;
        int sourcefd;
        int destinationfd;
        int bytescopied;
        pthread_t tid;
        struct copy_struct *next;
        } copyinfo_t;
        copyinfo_t *head = NULL;
        copyinfo_t *tail = NULL;

   After the copydirecotry function creates threads to copy all the files
   in the directory, it does a pthread_join on each thread in its list and
   frees the copyinfo_t structure.

# NOPE, didn't do
5) Modify the copyfilepass function Program 12.7 so that its parameter is
   a pointer to a copyinfo_t structure. Test the new version of
   copyfilepass and copydirectory.

#COMPLETE
6) Modify copydirecotry so that if a file is a directory instead of an ordinary
   file, copydirecotry creates a thread to run copydirecotry instead of
   copyfilepass. Test the new function

7) Devise a method for performing timings to compare an ordinar ycopy with the
   threaded copy.

*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "copyfilepass.h"

typedef struct copyArg_struct
{
    char *source;
    char *destination;
}copyArg;

typedef struct copy_struct {
    char *namestring;
    int sourcefd;
    int destinationfd;
    int bytescopied;
    pthread_t tid;
    struct copy_struct *next;
} copyinfo_t;

//function prototypes
void *copydirectory(void *arg);

//check commandline errors and create copydirectory thread
int main(int argc, char *argv[])
{
    //declarations
    copyArg args;
    int status;
    pthread_t mainThread; //create thread

    //error checking
    if(argc != 3)
    {
        printf("Execute parallelfilecopy_carlisle in the format of:\n\n");
        printf("\t./parallelfilecopy_carlisle sourceDir destDir\n\n");
        return -1;
    }

    //store args
    args.source = argv[1];
    args.destination = argv[2];

    //debug
    //printf("Source: %s\n", args.source);
    //printf("Destination: %s\n", args.destination);

    //create thread
    if((status=pthread_create(&mainThread, NULL, &copydirectory, &args)))
        printf("Thread creation failed: %d\n", status);

    pthread_join(mainThread, NULL); //join thread

    return 0;
}


//copy directory contents from source folder to destination folder
void *copydirectory(void *arg)
{
    copyArg * args = (copyArg*)arg;//incoming arguments
    copyinfo_t *head = NULL;//head linked list
    copyinfo_t *tail = NULL;//tail linked list

    //printf("thread received source: %s\n",(*args).source);
    //printf("thread received destination: %s\n",(*args).destination);

    //check if destination directory exists
    struct stat st;
    if(stat((*args).destination, &st)!=0)
        if(mkdir((*args).destination, 0754)) //create dir
            printf("Error occured making directory");


    //open directory
    DIR * directory = opendir((*args).source);
    struct dirent * entry;

    //get each file name
    while((entry = readdir(directory)) != NULL)
    {
        //move name to a good variable that's easy to use
        char * name = (*entry).d_name;

        //filter out .. and .
        if( strcmp(name, ".") != 0 && strcmp(name, "..") != 0)
        {
            //create the source filename with folder inserted at beginning
            char sourceFile[strlen((*args).source)+2];
            strcpy(sourceFile,(*args).source);
            strcat(sourceFile,"/");
            strcat(sourceFile,name);

            //debug
            //printf("\nfilename: %s\n",sourceFile);
            //printf("type: %d\n",(*entry).d_type);

            //check for regular file
            if((*entry).d_type == 8)
            {
                if(tail == NULL && head == NULL) //on first run initialize
                {
                    head = (copyinfo_t*) malloc(sizeof(copyinfo_t));
                    tail = head;
                }
                else //link after first run
                {
                    (*tail).next = (copyinfo_t*) malloc(sizeof(copyinfo_t));
                    tail = (*tail).next;
                }

                //allocate space for string and copy it
                (*tail).namestring = (char*) malloc(sizeof(char) * strlen(sourceFile));
                strcpy((*tail).namestring, sourceFile);

                //open the file
                FILE *fp;
                fp = fopen(sourceFile, "r");
                if(fp == NULL)
                    printf("Error opening file\n");
                else
                {
                    //file descriptor array
                    int *fdArray = (int*) malloc((sizeof(int) * 2));

                    //make proper destination path
                    char destFile[strlen((*args).destination)+2];
                    strcpy(destFile,(*args).destination);
                    strcat(destFile,"/");
                    strcat(destFile,name);

                    //debug
                    //printf("destFile: %s\n", destFile);

                    //place file descriptors in array
                    fdArray[0] = fileno(fp);
                    fdArray[1] = open(destFile, O_WRONLY | O_CREAT, 0644);
                    (*tail).sourcefd = fdArray[0];
                    (*tail).destinationfd = fdArray[1];

                    //debug
                    //printf("=====================\n");
                    //printf("fdArray[0] has %d\n",fdArray[0]);
                    //printf("fdArray[1] has %d\n",fdArray[1]);
                    //printf("=====================\n");

                    //create thread to copy file over
                    if((pthread_create(&(*tail).tid, NULL, &copyfilepass, fdArray)))
                        printf("Thread creation failed");
                    //else//debug
                        //printf("starting pthread_t tid: %ld\n", (long) (*tail).tid);

                    //had to put join here b/c execution was too fast to join
                    //after all three threads were created. this would cause a
                    //hang b/c second thread would signal before second join
                    //would be created. i even tried to move the pthread_create
                    //to a point where it would go through the linked list and
                    //create each thread and then immediately after going
                    //through and creating each thread it would start the
                    //pthread_joins. This was still too slow so I could not find
                    //a solution to the problem. Making the threads sleep for >
                    //5 seconds still did not help b/c the first one would sleep
                    //and so would the others so after the first one slept and
                    //the join waited the others would still finish before the
                    //pthread_join. I really did try.
                    int *threadstatus; //get bytes copied
                    pthread_join((*tail).tid, (void **)&threadstatus); //wait for thread
                    (*tail).bytescopied = *threadstatus;

                    //dealloc
                    free(fdArray);
                }
                //debug
                //printf("%s is a file\n",name);
                (*tail).next = NULL;
            }

            //check if folder
            if((*entry).d_type == 4)
            {
                //debug
                //printf("%s is a folder\n",name);

                pthread_t folderThread;//thread

                copyArg folderArgs;//args to send to thread

                //get source folder right
                folderArgs.source = (char*) malloc(strlen(sourceFile));
                strcpy(folderArgs.source, sourceFile);

                //get dest folder right
                folderArgs.destination = (char*) malloc(strlen((*args).destination));
                strcpy(folderArgs.destination, (*args).destination);
                strcat(folderArgs.destination,"/");
                strcat(folderArgs.destination,name);

                //debug
                //printf("folder source: %s\n",folderArgs.source);
                //printf("folder destination: %s\n",folderArgs.destination);

                //make thread
                if((pthread_create(&folderThread, NULL, &copydirectory, &folderArgs)))
                    printf("Thread creation failed\n");

                //wait for thread
                pthread_join(folderThread, NULL);
            }
        }
    }

    //joins
    //that i couldn't get to work down here
    //copyinfo_t *current = head;
    //while(1)
    //{
        //printf("joining: %s with %d \n", (*current).namestring, (int) (*current).tid);
        //int *threadstatus; //get bytes copied
        //pthread_join((*current).tid, (void **)&threadstatus); //wait for thread
        //(*current).bytescopied = *threadstatus;
        //printf("joined: %s\n", (*current).namestring);

        //if((*current).next == NULL)
            //break;
        //else
            //current = (*current).next;
    //}

    //debug
    //current = head;
    //while(1)
    //{
        //printf("\n==============================\n");
        //printf("char *namestring: %s\n", (*current).namestring);
        //printf("int sourcefd: %d\n", (*current).sourcefd);
        //printf("int destinationfd: %d\n", (*current).destinationfd);
        //printf("int bytescopied: %d\n", (*current).bytescopied);
        //printf("pthread_t tid: %d\n", (int) (*current).tid);
        //printf("struct copy_struct *next: %d\n", (int) (*current).next);


        //if((*current).next == NULL)
            //break;
        //else
            //current = (*current).next;
    //}




    //pass file descriptor array
    //copyfilepass();
}
