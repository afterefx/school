//*********************************************************************
// Christopher Carlisle
// Systems Programming
// Programming Homework #3: Implementing Pipes with Shared Memory and Semaphores
// March 19, 2011
// Instructor: Dr. Ajay K. Katangur
//
//********************************************************************
// *The program creates a software pipe and then forks a child.
// *The child reads from standard input and writes to the pipe.
// *The parent reads what the child has written to the pipe and outputs it to standard output.
// *When the child detects end-of-file on standard input, it closes the pipe for writing.
// *The parent then detects end-of-file on the pipe, closes the pipe for reading
//        (which destroys the pipe), and exits.
// *Execute the ipcs command to check that everything was properly destroyed.

//#define DEBUG  //for debug purposes
#include <cstdio>
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

union semun
{
        int val;
        struct semid_ds *buf;
        ushort  *array;
        struct seminfo *__buf;
}arg;

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

    p = pipe_open();
    //create the pipe
    //create a semaphore
    //get a shared memory segment
    //pipe_t *pipe = pipe_open();

    //fork child and parent
    cout << "About to fork\n";
    pid = fork();
    if (!pid) /* Child */
        child(p);
    else /* Parent */
        parent(p);

    return 0;
}

void child(pipe_t *p)
{
    int bytesCopied;
    int size;
    int stringLength = 0;
    int loopNum=0;
    char str[200];

    cout << "child: starting loop\n";
    while(1)
    {
        if((loopNum % 5) == 0)
            cout << "Looped: " << loopNum << endl;
        //gets(str);
        cout << "testing...\n";
        //getline(cin, str);
        //cin >> str;
        //
        cout.flush();
        cin.get(str, 200);

        cout << "child: should be paused " << str << "\n";
        //stringLength = temp.length();
        cout << "creating buffer of size " << stringLength+1 << endl;
        //char * buf = new char[temp.length()+1];
        cout << "going to loop and copy to buffer\n";
        //for(int i=0; i < temp.length(); i++)
        //for(int i=0; i < 99; i++)
        //{ buf[i] = temp[i]; }
        cout << "writing to pipe\n";
        //bytesCopied = pipe_write(p, buf, temp.length());
        bytesCopied = pipe_write(p, str, 100);
		cout << "bytesCopied = " << bytesCopied << "\n";
        if(bytesCopied)
        {
            cout << "wrote " << bytesCopied << endl;
            break;
        }
        else
            cout << "no bytes copied " << endl;

        loopNum++;

    }

    sleep(5);
    cout <<"child: closing pipe\n";
    pipe_close(p, O_WRONLY );
}

void parent(pipe_t *p)
{
    char display[200];
    int bytesRead;
//sleep(10);
    cout << "parent: starting loop\n";
    while(1)
    {
        cout << "parent: going to read\n";
        bytesRead = pipe_read(p, display, 199);
        cout <<"parent: should be finsihed reading\n";
        if(!bytesRead)
            break;
        cout << display << "\n";
    }

    pipe_close(p, O_RDONLY );
}

pipe_t *pipe_open()
{
    pipe_t * p; //temporary pipe struct
    int shmid; // temp shared mem segment id
    int semid; //temp semaphore id
    int checkForError;
    int * temp;

    try
    {

        //Create a shared memory segment to hold a pipe_t data structure by calling shmget. Use a key of IPC_PRIVATE and owner read/write permissions.
        shmid = shmget(IPC_PRIVATE,_POSIX_PIPE_BUF,0600|IPC_CREAT);
        if(shmid == -1)
            throw 110;

        //Attach the segment by calling shmat. Cast the return value of shmat to a pipe_t * and assign it to a local variable p.
        temp = (int*) shmat(shmid, (void*) 0,0);
        if(temp == (int*) -1)
            throw 115;
        else
            p = (pipe_t*) temp;

        //Set p->shmid to the ID of the shared memory segment returned by the shmget.
        (*p).shmid = shmid;

        //Set p->data_size and p->end_of_file to 0.
        (*p).data_size=0;
        (*p).end_of_file=0;

        //Create a semaphore set containing two elements by calling semget with IPC_PRIVATE key and owner read, write, execute permissions.
        semid = semget(IPC_PRIVATE, 2, 0700);
        if(semid == -1)
            throw 127;
        cout << "Semid: " << semid << endl;

        //Initialize both semaphore elements to 1, and put the resulting semaphore ID value in p->semid.
        //union semun {
			//int val;
		//}arg;
        arg.val=1;
        checkForError = semctl(semid, 0, SETVAL, arg);
		cout<<"pipe_open: checkForError = " << checkForError << "\n";
        if(checkForError == -1)
            throw 135;

        checkForError = semctl(semid, 1, SETVAL, arg);
		cout<<"pipe_open: checkForError = " << checkForError << "\n";
        if(checkForError == -1)
            throw 138;

        (*p).semid = semid;

        //If all the calls were successful, return p.
        return p;
    }
    // If an error occurs, deallocate all resources, set errno, and return a NULL pointer.
    catch (int exitCode)
    {
        cout << "EXCEPTION: PIPE_OPEN caught: " << exitCode << endl;
        if(exitCode > 110)
            shmctl(shmid, IPC_RMID, NULL);

        if(exitCode > 127)
            semctl(semid,0,IPC_RMID,0);

        errno = exitCode;
        return NULL;
    }

}

int pipe_read(pipe_t *p, char *buf, int bytes)
{
    cout << "pipe_read: starting function\n";
    int bytesRead=0;
    int temp;

    cout << "pipe_read: going into try\n";
    try
    {
        //Perform semop on p->semid to atomically decrement semaphore element zero, and test semaphore element one for 0.
        //Element zero provides mutual exclusion. Element one is only 0 if there is something in the buffer.
        cout << "pipe_read: making sembuf\n";
        sembuf *buffer = new sembuf;
        (*buffer).sem_num = 1; //
        (*buffer).sem_op = 0;  //decrements to 0
        (*buffer).sem_flg = 0;

        cout << "pipe_read: performing semop\n";
        semop((*p).semid, buffer, 1);
        cout << "pipe_read: finished semop\n";

        cout << "pipe_read: getting value for element one\n";
        //getting value for element one
        int semnum=1;
        temp = semctl((*p).semid,semnum,GETVAL);
		cout<<"temp = " << temp << "\n";
		sleep(4);
        if(temp == -1)
            throw 183;

        //test for 0 and continue until it is 0
        while(temp != 0)
        {
            cout << "pipe_read: looping\n";
            temp = semctl((*p).semid,semnum,GETVAL);
            if( temp == -1)
                throw 190;
        }


        //If p->data_size is greater than 0 do the following.
        if((*p).data_size > 0)
        {
            //Copy at most bytes bytes of information starting at position p->current_start of the software pipe into buf.
            //Take into account the number of bytes in the pipe.
            for(int i=0; i < bytes; i++)
            {
                if(i < (*p).data_size)
                {
                    buf[i] = (*(char*)(*p).current_start + i);
                    bytesRead++;
                }
                else
                    break;
            }

            //Update the p->current_start and p->data_size members of the pipe data structure.
            (*p).current_start = (*p).data;
            (*p).data_size = 0;

            //If successful, set the return value to the number of bytes actually read.
            return bytesRead;

        }

        //Otherwise, if p->data_size is 0 and p->end_of_file is true, set the return value to 0 to indicate end-of-file.
        else if((*p).data_size == 0 && (*p).end_of_file)
        { return 0; }//indicate eof


        //Perform another semop operation to release access to the pipe. Increment element zero.
        //If no more data is in the pipe, also increment element one unless p->end_of_file is true. Perform these operations atomically by a single semop call.
        (*buffer).sem_num=0;
        (*buffer).sem_op=1;
        temp = semop((*p).semid, buffer, 1);
        if(temp == -1)
            throw 230;

    }
    //If an error occurs, return –1 with errno set.
    catch(int exitCode)
    {
        errno = exitCode;
        return -1;
    }

}

int pipe_write(pipe_t *p, char *buf, int bytes)
{
    int temp;
    int i;

    try
    {
        //Perform a semop on p->semid to atomically decrement both semaphore elements zero and one.
        u_short args[2] = {0,0};
        temp = semctl((*p).semid,0,SETALL,args); //decrements to 0
        if(temp == -1)
            throw 255;

        //Copy at most _POSIX_PIPE_BUF bytes from buf into the pipe buffer.
        for(i=0; i < _POSIX_PIPE_BUF; i++)
        { (*p).data[i] = buf[i]; }

        //Set p->data_size to the number of bytes actually copied, and set p->current_start to 0.
        (*p).data_size = i;
        (*p).current_start = 0;

        //Perform another semop call to atomically increment semaphore element zero of the semaphore set.
        sembuf *buffer = new sembuf;
        (*buffer).sem_num=0;
        (*buffer).sem_op=1;
        temp = semop((*p).semid, buffer, 1);
        if(temp == -1)
            throw 270;

        //If successful, return the number of bytes copied.
        return i;
    }
    //If an error occurs, return –1 with errno set.
    catch(int exitCode)
    {
        errno = exitCode;
        return -1;
    }

}

int pipe_close(pipe_t *p, int how)
{
    int temp;
    int semid_temp;
    int checkForError;
    int shmid_temp;


    try
    {
        //Use the semop function to atomically decrement element zero of p->semid. If the semop fails, return –1 with errno set.
        sembuf *buffer = new sembuf;
        (*buffer).sem_num = 0; //
        (*buffer).sem_op = 0;  //decrements to 0
        temp = semop((*p).semid, buffer, 1);
        if(temp == -1)
            throw 300;

        //If how & O_WRONLY is true, do the following.
        if(how == O_WRONLY)
        {
            //Set p->end_of_file to true.
            (*p).end_of_file = true;

            //Copy p->semid into a local variable, semid_temp.
            semid_temp = (*p).semid;

            //Perform a semctl to set element one of p->semid to 0.
        union semun {
			int val;
		}arg;
        arg.val=1;
            checkForError = semctl(semid_temp, 1, SETVAL, arg);
            if(checkForError == -1)
                throw 315;

            //Perform a shmdt to detach p.
            checkForError = shmdt(shmat((*p).shmid,0,0));
            if(checkForError == -1)
                throw 322;

            //Perform a semop to atomically increment element zero of semid_temp.
            sembuf *buffer = new sembuf;
            (*buffer).sem_num=0;
            (*buffer).sem_op=1;
            temp = semop((*p).semid, buffer, 1);
            if(temp == -1)
                throw 328;

            //If any of the semop, semctl, or shmdt calls fail, return –1 immediately with errno set.
        }

        //If how & O_RDONLY is true, do the following.
        if(how == O_RDONLY)
        {
            //Perform a semctl to remove the semaphore p->semid. (If the writer is waiting on the semaphore set, its semop returns an error when this happens.)
            semctl((*p).semid, 0, IPC_RMID);

            //Copy p->shmid into a local variable, shmid_temp.
            shmid_temp = (*p).shmid;

            //Call shmdt to detach p.
            checkForError = shmdt(shmat((*p).shmid,0,0));
            if(checkForError == -1)
                throw 353;

            //Call shmctl to deallocate the shared memory segment identified by shmid_temp.
            shmctl(shmid_temp, IPC_RMID, NULL);
            if(checkForError == -1)
                throw 358;
        }

    }
    //If any of the semctl, shmdt, or shmctl calls fail, return –1 immediately with errno set.
    catch(int exitCode)
    {
        errno = exitCode;
        return -1;

    }
}
