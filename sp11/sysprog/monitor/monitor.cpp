//*********************************************************************
// Christopher Carlisle
// Systems Programming
// Programming Homework #2: Files and Directory Monitor
// March 2, 2011
// Instructor: Dr. Ajay K. Katangur
//
//********************************************************************

//#define DEBUG  //for debug purposes
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <list>
#include "cliArgs.h"
using namespace std;

struct statinfo
{
    string fileName; // File Name
    struct stat * prevStatus; //previous scan information from stat
    struct stat * status;       // Information from stat()
    bool newEntry; //flag for new entries
};

//function prototypes
void loop(cliArgs*);
void processDirectory(string,int);
void processFile(string);
void deleteMapKeys();
#ifdef DEBUG
void showContents();
#endif

//global vars
map<string, statinfo*> files; //map of files currently being tracked
list<string> deleteList; //list of files to delete from map at end of scan
bool firstScan = true; //first scan flag

//********************************************************************
//
// Main Function
//
// Get arguments from the command line and then start up the loop to 
// scan
//
//
// Return Value
// ------------
// void
//
// Value Parameters
// ----------------
// argc         int         number of arguments on command line
// **argv       char        char array of the command line arguments
//
// Reference Parameters
// --------------------
// n/a
//
// Local Variables
// ---------------
// *args        cliArgs     command line argmuent object
//
//*******************************************************************
int main(int argc, char **argv)
{
    cliArgs *args;

    //take care of the command line arguments
    try {
        args = new cliArgs(argc, argv);
    }
    catch(string e) {
        cout << e << endl;
        return 1;
    }

    #ifdef DEBUG
    args->debug();//debug output
    #endif

    //analyze files and folders
    loop(args);

    return 0;
}

//********************************************************************
//
// Loop Function
//
// This function first receives the files and options from the
// cliArg object. It then scans through and adds each file provided
//
//
// Return Value
// ------------
// void
//
// Value Parameters
// ----------------
// *args        cliArgs     command line argmuent object
//
// Reference Parameters
// --------------------
// n/a
//
// Local Variables
// ---------------
// numFiles     int                                 Number of files to receive from cliArg object
// delay        int                                 Number of seconds to wait between each scan
// scanCount    int                                 Number of scans to perform. If 0 then infinite
// scan         int                                 Current scan number, used for conditional testing
// counter      int                                 Number of scans performed, used for output
// lastPos      int                                 Last position of character in string
// notInfinite  bool                                Defines if we are looping infinitely
// tempChar     int                                 Temporary char value
// *current     statinfo                            Pointer to the current statinfo struct being handled
// it2          map<string, statinfo*>::iterator    iterator to go through files in the files map
//
//*******************************************************************
void loop(cliArgs *args)
{
    int numFiles = args->getNumFiles();
    int delay = args->getTimeDelay();
    int scanCount = args->getScanCount();
    int scan=0;
    int counter=1;
    int lastPos;
    bool notInfinite = (scanCount) ? true : false;
    char tempChar;
    statinfo *current;

    //if scanCount comes back zero for
    //inifinite loop set to one so it will be infinite
    if(!scanCount)
        scanCount = 1;

    //go through each scan
    while( scan < scanCount)
    {
        #ifdef DEBUG
        showContents();
        #endif

        //output for user to show which scan we are on
        cout << "Scan " << counter << ":\n";

        ///////////////////////////
        //on first scan add files
        ///////////////////////////
        if(firstScan)
        {
            #ifdef DEBUG
            cout << "inside first scan if\n";
            #endif
            //loop ovoer each file received on command line
            for(int i=0; i < numFiles; i++)
            {
                current = new statinfo; //get a new pointer
                (*current).status = new struct stat; //allocate a stat struct for our struct
                (*current).fileName = args->getFilename(i); //get a filename

                //check for a / as last character and remove it if it is there
                lastPos = (*current).fileName.length()-1; //get last character position
                tempChar = (*current).fileName[lastPos]; // get last character
                if(tempChar == '/') //if / remove it
                    (*current).fileName = (*current).fileName.substr(0,lastPos);


                //get info for current file
                if(stat((*current).fileName.c_str(), (*current).status))
                {
                    //output error
                    cout << "monitor: Error " + (*current).fileName + " is causing problems\n";
                    continue;
                }

                //add pointer to map for filename as key
                files[(*current).fileName]=current;

                //we have a new file so say that it will be added
                (*current).newEntry=true;

                //check to see if current filename is directory
                if(S_ISDIR((*current).status->st_mode))
                    processDirectory((*current).fileName,0); //if directory process contents
                else
                    processFile((*current).fileName); //if file process file
            }

        }
        else
        {
            //iterate through each file and folder
            map<string, statinfo*>::iterator it2;
            for ( it2=files.begin() ; it2 != files.end(); it2++ )
            {
                // if folder processDirectory
                current = (*it2).second; //get the pointer from iterator

                //check to see if it's a directory
                if(S_ISDIR((*current).status->st_mode))
                    processDirectory((*current).fileName,0); //process directory contents
                else
                    processFile((*current).fileName); //process file
            }
        }


        //if not set for infinite count then increment scan counter
        if(notInfinite)
            scan++;

        firstScan = false;//true only on first loop
        deleteMapKeys(); //remove any keys that have been deleted
        sleep(delay);//delay between scans
        counter++; //increment scan counter for user output
        cout << "\n"; //put a newline to output
    }

}

//********************************************************************
//
// deleteMapKeys Function
//
// Opens the deleteList list and while there are still keys in there
// it receives the key, deletes it from the files map and pops off the
// last key in the list. Purpose is to delete any files from the map
// that do not exist anymore
//
//
// Return Value
// ------------
// void
//
// Value Parameters
// ----------------
// n/a
//
// Reference Parameters
// --------------------
// n/a
//
// Local Variables
// ---------------
// key          string      String to hold name of current key
//
//*******************************************************************
void deleteMapKeys()
{
    string key;

    //while the list is not empty delete keys
    while (!deleteList.empty())
    {
        key =deleteList.back(); //get the bakc of the list
        files.erase(key); //erase the key
        deleteList.pop_back(); //pop the back of the list off
    }

}

//********************************************************************
//
// Process Directory Function
//
// Processes the contents of the directory. For each file in the
// directory it prepends the directory name, asks stat for info on the
// file and then sends it to be processed as a file or folder based on
// what stat tells it.
//
//
// Return Value
// ------------
// void
//
// Value Parameters
// ----------------
// key              string  This is the foldername
// recursionCount   int     This is the number of recursive calls deep that we
//                          are for this function
//
// Reference Parameters
// --------------------
// n/a
//
// Local Variables
// ---------------
// *fd          DIR                                     file descriptor
// *dirp        struct dirent                           directory pointer thing
// temp         string                                  temporary string
// lastPos      int                                     position of last character in string
// tempChar     char                                    temporary char
// keyExists    bool                                    bool to show if key already exists in the map
// *current     statinfo                                pointer to statinfo struct for current file
// it           map<string, statinfo*>::iterator        iterator for the file map
//
//*******************************************************************
void processDirectory(string key, int recursionCount)
{

    DIR *fd; //file descriptor
    struct dirent *dirp; //directory pointer thing
    string temp; //temporary string
    int lastPos; //position of last character in string
    char tempChar; //temporary char
    bool keyExists=false; //bool to show if key already exists in the map

    //if the key is not the current folder or a folder higher or if it's
    //the first execution of this function for a directory without recursion
    // then go ahead
    if((key != ".." && key != "." ) || recursionCount == 0)
    {
        #ifdef DEBUG
        cout << "checking contents of " << key << "\n";
        #endif

        //open the directory
        fd = opendir(key.c_str());
        if(fd == NULL)
            cout << "Error opening " << key << endl;

        //loop over directories contents
        while ((dirp = readdir(fd)) != NULL)
        {
            temp = string(dirp->d_name); //get string for the filename of current file

            //make sure that it's not the foldername or parent or current
            //directory
            if(temp != ".." && temp != "." && temp != key)
                temp =  key + "/" + string(dirp->d_name);

            //figure out if filename is a folder or file
            statinfo *current; //declare a pointer
            current = new statinfo; //get a new statinfo struct
            (*current).fileName = temp; //store the filename
            (*current).status = new struct stat; //allocate a stat struct for our struct

            //check for / at end of folder name and
            //remove if need be
            lastPos = (*current).fileName.length()-1;
            tempChar = (*current).fileName[lastPos];
            if(tempChar == '/') //remove extra slash at end if needed
                (*current).fileName = (*current).fileName.substr(0,lastPos);

            //get file information and output error for non-zero number
            if(stat((*current).fileName.c_str(), (*current).status))
            {
                cout << "monitor: Error " + (*current).fileName + " is causing problems\n";
                continue;//continue to the next file
            }

            //check to see if the key already exists in the map
            map<string, statinfo*>::iterator it;
            for ( it=files.begin() ; it != files.end(); it++ )
            {
                if(it== files.find(temp))
                    keyExists = true;
            }

            //if the key does not exists and filename is not .. or . then add it
            if(!keyExists && (*current).fileName != ".." && (*current).fileName !=".")
            {
                //add pointer to map for filename as key
                files[(*current).fileName]=current;
                (*current).newEntry=true; //set that it's a new entry
            }
            keyExists = false; //reset key exists

            //check to see if current filename is directory
            if(S_ISDIR((*current).status->st_mode))
            {
                recursionCount++; //increase recursion count
                processDirectory((*current).fileName,recursionCount); //process the contents of the directory
            }
            else
                processFile((*current).fileName); //process the file

        }

    }
}

//********************************************************************
//
// Process File Function
//
// Processes the file checking to see if it still exists. If it does
// still exist then check to see if it's new. If it's new show that
// it's been added to the file map and add to file map. If it's not
// check the modified times to see if they have been changed since
// the last scan. If the times are different output that they have
// been changed. Move the current modified time to the previous
// to prepare for the next scan.
//
//
// Return Value
// ------------
// void
//
// Value Parameters
// ----------------
// key              string  This is the filename
//
// Reference Parameters
// --------------------
// n/a
//
// Local Variables
// ---------------
// *current     statinfo    pointer to statinfo struct for current file
//
//*******************************************************************
void processFile(string key)
{
    #ifdef DEBUG
    cout << "checking " << key << "\n";
    #endif

    statinfo *current;
    current = files[key]; //get the current key pointer

    //get this cycle information
    if(stat((*current).fileName.c_str(), (*current).status))
    {
        cout << "DELETED    - " + key + "\n";
        #ifdef DEBUG
        cout << (int) (stat((*current).fileName.c_str(), (*current).status)) << endl;
        #endif
        deleteList.push_back(key);
        return;
    }


    //if previoustime == 0 new file
    if( firstScan ||(*current).newEntry)
    {
        (*current).prevStatus = (*current).status;
        cout << "ADDED    - " + key + "\n";
        (*current).newEntry = false;
        //set last cycle and modification time
    }

    //compare to previous time
    //if previoustime is diff then current changed
    if( ((*current).prevStatus->st_mtime != (*current).status->st_mtime) && !firstScan)
        cout << "CHANGED    - " + key + "\n";

    (*current).prevStatus = (*current).status;
    (*current).status = new struct stat; //allocate a stat struct for our struct
}


#ifdef DEBUG
//********************************************************************
//
// Show Contents Function
//
// Show the contents of the map for debug purposes
//
// Return Value
// ------------
// void
//
// Value Parameters
// ----------------
// n/a
//
// Reference Parameters
// --------------------
// n/a
//
// Local Variables
// ---------------
//  it  map<string, statinfo*>::iterator    iterator for the files map
//
//*******************************************************************
void showContents()
{
    map<string, statinfo*>::iterator it;
    int count=0;
                cout << "============================\n";
                cout << "        Map Contents\n";
                cout << "============================\n";
              for ( it=files.begin() ; it != files.end(); it++ )
              {
                    cout << count << ".) " << (*it).first << "\n";
                    count++;

              }
                cout << "============================\n";
}
#endif
