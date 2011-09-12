/*
*********************************************************************
// Christopher Carlisle
// Systems Programming
// Programming Homework #3: Implementing Pipes with Shared Memory and Semaphores
// March 19, 2011
// Instructor: Dr. Ajay K. Katangur
//
********************************************************************
// *The program creates a software pipe and then forks a child.
// *The child reads from standard input and writes to the pipe.
// *The parent reads what the child has written to the pipe and outputs it to standard output.
// *When the child detects end-of-file on standard input, it closes the pipe for writing.
// *The parent then detects end-of-file on the pipe, closes the pipe for reading
//        (which destroys the pipe), and exits.
// *Execute the ipcs command to check that everything was properly destroyed.


################################################
                  NOTES
################################################
if the parent ends before the child and the child is
still in an infinite loop then the child will freak out
and never wait for input. STDIN STOPS WORKING
                  */

//get rid of sleeps and extra couts

//#define DEBUG  //for debug purposes
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <list>
#include <limits.h>
#include <string>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
using namespace std;

#define READ 0
#define WRITE 1
#define SHM_SIZE 1024

extern int errno;

//union semun
//{
    //int val;
    //struct semid_ds *buf;
    //ushort  *array;
    //struct seminfo *__buf;
//}arg;

typedef struct pipe
{
    int semid;                      // ID of protecting semaphore set
    int shmid;                      // ID of the shared memory segment
    char data[_POSIX_PIPE_BUF];     // buffer for the pipe data
    int data_size;                  // bytes currently in the pipe
    void *current_start;            // pointer to current start of data
    bool end_of_file;                       // true after pipe is closed for writing
} pipe_t;

//function prototypes
pipe_t *pipe_open();
int pipe_read(pipe_t *p, char *buf, int bytes);
int pipe_write(pipe_t *p, char *buf, int bytes);
int pipe_close(pipe_t *p, int how);
void child(pipe_t *p);
void parent(pipe_t *p);


int main(int argc, char **argv)
{
    pid_t pid; //process id
    pipe_t *p;
    string test;

    p = pipe_open();

    //fork child and parent
    //cout << "Basic cin attempt: ";
    //cin >> test;
    //cout << "You entered: " << test << endl;
    //cout << "About to fork\n";
    pid = fork();
    if (!pid) /* Child */
        child(p);
    else /* Parent */
        parent(p);

    return 0;
}

void child(pipe_t *p)
{
    char * str;
    int bytesCopied=0;


    //write to the pipe what was received
    //-1 returned when no access by mutual exclusion
    do
    {
        str = new char[_POSIX_PIPE_BUF];
        cout << "What do you want to say?\n";
        cin.getline(str,_POSIX_PIPE_BUF);
        if(cin.eof())
            break;
        else
        {
            //cout << "child: sending: " << str << endl;
            bytesCopied = pipe_write(p, str, strlen(str) );
            //cout << "child: bytesCopied: " << bytesCopied << endl;
        }
        delete [] str;

        sleep(1);
    }
    while(bytesCopied != 0);

    pipe_close(p, O_WRONLY);

}

//parent process needs to wait for child to finish
//otherwise it kills the child
void parent(pipe_t *p)
{
    char str[_POSIX_PIPE_BUF];
    int bytesRead;

    //while value returned from pipe_read is != 0
    //-1 used to indicate noting to be read
    //cout << "parent: going to do\n";
    do
    {
        bytesRead = pipe_read(p, str, _POSIX_PIPE_BUF);
        if(bytesRead > 0)
        {
            cout << "parent received: " << str << endl;
        }
    }while(bytesRead != 0 && !(*p).end_of_file);
    //cout << "parent: stopped while";

    pipe_close(p, O_RDONLY);
}

pipe_t *pipe_open()
{
    pipe_t *p = new pipe_t; //temp pipe struct
    int *temp;
    int shmid;
    int semid;
    int checkForError;

    try
    {

        //Create a shared memory segment to hold a pipe_t data structure by calling shmget. Use a key of IPC_PRIVATE and owner read/write permissions.
        shmid = shmget(IPC_PRIVATE, _POSIX_PIPE_BUF, 0600|IPC_CREAT);
        if(shmid == -1)
            throw 134;
        //else
            //cout << "pipe_open: SHMID: " << shmid << "\n";

        //Attach the segment by calling shmat. Cast the return value of shmat to a pipe_t * and assign it to a local variable p.
        temp = (int*) shmat(shmid, (void*) 0, 0);
        if( temp == (int*) -1)
            throw 142;
        else
        {
            //cout << "pipe_open: SHMAT aka shm_addr: " << temp << "\n";
            p = (pipe_t*) temp;
        }

        //Set p->shmid to the ID of the shared memory segment returned by the shmget.
        (*p).shmid = shmid;

        //Set p->data_size and p->end_of_file to 0.
        (*p).data_size = 0;
        (*p).end_of_file = 0;

        //Create a semaphore set containing two elements by calling semget with IPC_PRIVATE key and owner read, write, execute permissions.
        semid = semget(IPC_PRIVATE, 2, 0700);
        if(semid == -1)
            throw 158;
        //else
            //cout << "pipe_open: SEMID: " << semid << "\n";

        //Initialize both semaphore elements to 1, and put the resulting semaphore ID value in p->semid.
        semun arg;
        arg.array = new u_short[2];
        arg.array[0] = 1;
        arg.array[1] = 1;
        checkForError = semctl(semid, 0, SETALL, arg);
        if(checkForError == -1)
            throw 169;
        //else
            //cout << "pipe_open: successful in setting values\n";

        //set the semid in p
        (*p).semid = semid;

        //If all the calls were successful, return p.
        return p;
    }
    // If an error occurs, deallocate all resources, set errno, and return a NULL pointer.
    catch (int exitCode)
    {
        cout << "pipe_open: EXCEPTION CAUGHT: " << exitCode << "\n";
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid,0,IPC_RMID, 0);
        errno = exitCode;
        return NULL;
    }

}

int pipe_read(pipe_t *p, char *buf, int bytes)
{
    int returnVal;
    sembuf *semaphoreBuf;

    try
    {
        //Element zero provides mutual exclusion.
        if(!semctl((*p).semid, 0, GETVAL, NULL))
            return -1;

        //Perform semop on p->semid to atomically decrement semaphore element zero, and test semaphore element one for 0.
        semaphoreBuf = new sembuf;
        (*semaphoreBuf).sem_num = 0;
        (*semaphoreBuf).sem_op = -1;
        //(*semaphoreBuf).sem_flg = 2;
        if(semop((*p).semid, semaphoreBuf, 1) == -1)
            throw 211;

        delete semaphoreBuf;

        //when [1] Not zero means nothing in the buffer
        if(semctl((*p).semid, 1, GETVAL, NULL) != 0)
        {
            //reset semaphore zero to where it was before we started
            semaphoreBuf = new sembuf;
            (*semaphoreBuf).sem_num = 0;
            (*semaphoreBuf).sem_op = 1;
            //semaphoreBuf[0].sem_flg = 2;

            if(semop((*p).semid, semaphoreBuf, 1) == -1)
                throw 248;

            delete semaphoreBuf;

            //lets get out of here
            return -1;
        }
        //else
            //cout << "pipe_read: we got stuff in the buffer\n";


        //If p->data_size is greater than 0 do the following.
        if((*p).data_size > 0)
        {
            //cout << "pipe_read: Data to be read ^^ \n";

            //Copy at most bytes bytes of information starting at position p->current_start of the software pipe into buf.
            //Take into account the number of bytes in the pipe.
            int copyNum = (bytes < (*p).data_size) ? bytes : (*p).data_size;

            strcpy(buf, (*p).data);
            //cout << "pipe_read: (*p).data contains: " << (*p).data << endl;
            //cout << "pipe_read: buf now contains: " << buf << endl;

            //Update the p->current_start and p->data_size members of the pipe data structure.
            (*p).current_start = ((char*)(*p).current_start) + copyNum;
            (*p).data_size -= copyNum;

            //If successful, set the return value to the number of bytes actually read.
            returnVal = copyNum;
        }

        //Otherwise, if p->data_size is 0 and p->end_of_file is true, set the return value to 0 to indicate end-of-file.
        else if(!(*p).data_size && (*p).end_of_file)
        {
            //cout << "pipe_read: end of file set\n";
            returnVal = 0;
        }
        //else
            //cout << "made it in and nothin happed :\\\n";

        //Perform another semop operation to release access to the pipe. Increment element zero If no more data is in the pipe.
        //Also increment element one if p->end_of_file is false. Perform these operations atomically by a single semop call.
        if(!(*p).data_size && !(*p).end_of_file)
        {
            //cout << "pipe_read: releasing both sems\n";
            semaphoreBuf = new sembuf[2];
            semaphoreBuf[0].sem_num = 0;
            semaphoreBuf[0].sem_op = 1;
            //semaphoreBuf[0].sem_flg = 2;
            semaphoreBuf[1].sem_num = 1;
            semaphoreBuf[1].sem_op = 1;
            //semaphoreBuf[1].sem_flg = 2;

            if(semop((*p).semid, semaphoreBuf, 2) == -1)
                throw 252;
            delete [] semaphoreBuf;
        }
        else
        {
            //cout << "pipe_read: eof detected\n";
            semaphoreBuf = new sembuf;
            (*semaphoreBuf).sem_num = 0;
            (*semaphoreBuf).sem_op = 1;
            //semaphoreBuf[0].sem_flg = 2;

            if(semop((*p).semid, semaphoreBuf, 1) == -1)
                throw 252;

             delete semaphoreBuf;
        }

        return returnVal;

    }
    //If an error occurs, return –1 with errno set.
    catch(int exitCode)
    {
        cout << "pipe_read: EXCEPTION CAUGHT: " << exitCode << "\n";
        errno = exitCode;
        return -1;
    }

}

int pipe_write(pipe_t *p, char *buf, int bytes)
{
    sembuf * semaphoreBuf;

    //if(!semctl((*p).semid, 0, GETVAL, NULL))
        //return -1;

    //cout << "pipe_wite: going to try\n";
    try
    {
        //Perform a semop on p->semid to atomically decrement both semaphore elements zero and one.
        semaphoreBuf = new sembuf[2];
        semaphoreBuf[0].sem_num = 0;
        semaphoreBuf[0].sem_op = -1;
        //semaphoreBuf[0].sem_flg = 0;
        semaphoreBuf[1].sem_num = 1;
        semaphoreBuf[1].sem_op = -1;
        //semaphoreBuf[1].sem_flg = 0;

        //cout << "pipe_write: about to semop on semid: " << (*p).semid << " \n";
        if(semop((*p).semid, semaphoreBuf, 2) == -1)
            throw 291;
        //else
            //cout << "pipe_write: Successful semop decrement both semaphore elements\n";

        delete [] semaphoreBuf;

        //Copy at most _POSIX_PIPE_BUF bytes from buf into the pipe buffer.
        int copyNum = (_POSIX_PIPE_BUF < strlen(buf)) ? _POSIX_PIPE_BUF : strlen(buf);

        //cout << "pipe_write: doin the strncpy for " << copyNum << " bytes\n";
        strcpy((*p).data, buf);
        //cout << "pipe_write: (*p).data contains: " << (*p).data << endl;
        //cout << "pipe_write: buf contains: " << buf << endl;

        //Set p->data_size to the number of bytes actually copied, and set p->current_start to 0.
        (*p).data_size = copyNum;
        (*p).current_start = 0;

        //Perform another semop call to atomically increment semaphore element zero of the semaphore set.
        semaphoreBuf = new sembuf;
        (*semaphoreBuf).sem_num = 0;
        (*semaphoreBuf).sem_op = 1;
        if(semop((*p).semid, semaphoreBuf, 1) == -1)
            throw 317;

        delete semaphoreBuf;

        //If successful, return the number of bytes copied.
        return copyNum;
    }
    //If an error occurs, return –1 with errno set.
    catch(int exitCode)
    {
        cout << "pipe_write: EXCEPTION CAUGHT: " << exitCode << "\n";
        errno = exitCode;
        return -1;
    }

}

int pipe_close(pipe_t *p, int how)
{
    int checkForError;
    int semid_temp;

    try
    {
        //Use the semop function to atomically decrement element zero of p->semid. If the semop fails, return –1 with errno set.
        sembuf *buf = new sembuf;
        (*buf).sem_num = 0;
        (*buf).sem_op = -1;
        //(*buf).sem_flg = 2;
        if(semop((*p).semid, buf, 1) == -1)
            throw 262;

        //If how & O_WRONLY is true, do the following.
        //cout << "how == " << how << endl;
        //cout << "O_WRONLY == " << O_WRONLY << endl;
        //cout << "O_RDONLY == " << O_RDONLY << endl;
        if(how == O_WRONLY)
        {
            //cout << "pipe_close: called by child\n";
            //Set p->end_of_file to true.
            (*p).end_of_file = true;

            //Copy p->semid into a local variable, semid_temp.
            semid_temp = (*p).semid;

            //Perform a semctl to set element one of p->semid to 0.
            semun arg;
            arg.val = 0;
            if(semctl(semid_temp, 1, SETVAL, arg) == -1)
                throw 278;


            //Perform a shmdt to detach p.
            if(shmdt(p) == -1)
                throw 282;

            //Perform a semop to atomically increment element zero of semid_temp.
            (*buf).sem_num = 0;
            (*buf).sem_op = 1;
            //(*buf).sem_flg = 2;
            if(semop(semid_temp, buf, 1) == -1)
                throw 289;

            //If any of the semop, semctl, or shmdt calls fail, return –1 immediately with errno set.
            //cout << "CHILD IS DONE!!!!\n";
        }

        //If how & O_RDONLY is true, do the following.
        if(how == O_RDONLY)
        {
            //cout << "pipe_close: called by parent\n";
            //Perform a semctl to remove the semaphore p->semid. (If the writer is waiting on the semaphore set, its semop returns an error when this happens.)
           if( semctl((*p).semid, 0, IPC_RMID) == -1)
               throw 299;

            //Copy p->shmid into a local variable, shmid_temp.
            int shmid_temp = (*p).shmid;

            //Call shmdt to detach p.
            if(shmdt(p) == -1)
                throw 306;

            //Call shmctl to deallocate the shared memory segment identified by shmid_temp.
            if(shmctl(shmid_temp, IPC_RMID, NULL) == -1)
                throw 310;

            //cout << "PARENT IS DONE!!!!\n";
        }

    }
    //If any of the semctl, shmdt, or shmctl calls fail, return –1 immediately with errno set.
    catch(int exitCode)
    {
        cout << "pipe_close: EXCEPTION CAUGHT: " << exitCode << "\n";
        errno = exitCode;
        return -1;
    }
}
