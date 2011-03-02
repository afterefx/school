//*********************************************************************
// Christopher Carlisle
// Systems Programming
// Programming Homework #2: Files and Directory Monitor
// March 2, 2011
// Instructor: Dr. Ajay K. Katangur
//
//********************************************************************
#include <iostream>
#include <sys/stat.h>
#include <string>
using namespace std;

//********************************************************************
//
// cliArgs Class
//
// Parse the command line
//
//
// Public Variables
// ----------------
// argc         int         number of arguments on command line
// **args       char        char array of the command line arguments
//
//
// Private Variables
// -----------------
// *fileNames   string      fileNames associated with args
// timeDelay    int         store the delay time
// scanCount    int         stores the number of times to scan
// numFiles     int         stores the number of files that were named on the cli
// timeSet      bool        flag to see if time has already been set
// scanSet      bool        flag to see if scan count has already been set
//
//
// Public Functions
// --------------------
//          cliArgs(int argc, char **argv)  constructor to get everything in place
// void     debug()                         debug output
// int      getScanCount()                  returns number of scans to perform
// int      getTimeDelay()                  returns time delay between scans in seconds
// int      getNumFiles()                   returns number of files from cli
// string   getFilename(int num)            returns filename for given index num
//          ~cliArgs()                      destructor
//
// Private Functions
// ---------------
// n/a
//
//*******************************************************************
class cliArgs
{
    private:
        string *fileNames; //fileNames associated with args
        int timeDelay; //store the delay time
        int scanCount; //stores the number of times to scan
        int numFiles; //stores the number of files that were named on the cli
        bool timeSet; //flag to see if time has already been set
        bool scanSet; //flag to see if scan count has already been set

    public:
        cliArgs(int argc, char **argv); //constructor to get everything in place
        void debug(); //debug output
        int getScanCount();
        int getTimeDelay();
        int getNumFiles();
        string getFilename(int num);
        ~cliArgs();

};

//********************************************************************
//
// Constructor Function
//
// Reads arguments from the command line. If option store it
// in appropriate variable if it has not already been set. If a file
// add to list of files to be scanned
//
//
// Return Value
// ------------
// n/a
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
// temp         string      temporary string to store a substr
// numOptions   int         number of options given
// bob          string      used to construct string to throw
// i            int         iterator
//
//*******************************************************************
cliArgs::cliArgs(int argc, char **argv)
{
    int numOptions=0;
    string temp; //used to temporarily store a substr
    numFiles = argc-1;
    timeDelay = 10;
    scanCount = 0;
    timeSet = false;
    scanSet = false;

    ///////////////////////////////////////////////
    //check to make sure there are enough arguments
    ///////////////////////////////////////////////
    if(argc < 2) {
        //throw an error message
        cout << "monitor\nusage: monitor [-t delay] [-l count] {fileName}+\n";
        exit(-1);
    }
    else {
        //allocate memory
        fileNames = new string[argc];
    }

    ///////////////////////////////////////////////
    //start working with strings
    ///////////////////////////////////////////////
    for( int i=1; i < argc; i++) { //go through each argument from cli
        temp = argv[i]; //store cstring into string object

        if(temp[0] == '-') { //if it is an option change settings

            //check for the appropriate options of either t or l
            if(temp[1] != 't' && temp[1] != 'l') { //throw error if not t or l
                string bob = "monitor: illegal option -- " + temp.substr(1,1) + "\nusage: monitor [-t delay] [-l count] {fileName}+";
                throw bob;
            }

            //store arguments
            else {

                //Setting time delay
                if( temp.substr(0,2) == "-t" && !timeSet) //store option with a dash at the beginning
                {
                    if(atoi(temp.substr(2).c_str()))
                         timeDelay = atoi(temp.substr(2).c_str()); //store the value of the option in a separate variable
                    else //throw error
                        throw (string) "monitor: illegal value for option -- t\nusage: monitor [-t delay] [-l count] {fileName}+";

                     timeSet = true; //set flag to show time has been set by user
                     numFiles--; //reduce the number of file names b/c one argument was an option
                     numOptions++; //increase the number of options that there were
                }
                else if( temp.substr(0,2) == "-t" && timeSet) //store option with a dash at the beginning
                    throw (string) "monitor: Delay can only be set once\nusage: monitor [-t delay] [-l count] {fileName}+";

                //Setting number of scans
                if( temp.substr(0,2) == "-l" && !scanSet)
                {
                    if(atoi(temp.substr(2).c_str()))
                        scanCount = atoi(temp.substr(2).c_str()); //store the value of the option in a separate variable
                    else
                        throw (string) "monitor: illegal value for option -- l\nusage: monitor [-t delay] [-l count] {fileName}+";

                    scanSet = true;
                    numFiles--;
                    numOptions++;
                }
                else if( temp.substr(0,2) == "-l" && scanSet)
                    throw (string) "monitor: Scan can only be set once\nusage: monitor [-t delay] [-l count] {fileName}+";
            }
        }
        else { //file names and folder names arg and value are ==
            fileNames[i-1-numOptions] = temp;
        }
    }

    //if no filenames are given throw an error
    if(numFiles == 0)
            throw (string) "monitor: No files specified.\n\nusage: monitor [-t delay] [-l count] {fileName}+";

}

//********************************************************************
//
// getTimeDelay Function
//
// Returns the time delay for the instantiated object
//
//
// Return Value
// ------------
// int
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
// n/a
//
//*******************************************************************
int cliArgs::getTimeDelay()
{ return timeDelay; }

//********************************************************************
//
// getScanCount Function
//
// Returns the scan count for the instantiated object
//
//
// Return Value
// ------------
// int
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
// n/a
//
//*******************************************************************
int cliArgs::getScanCount()
{ return scanCount; }

//********************************************************************
//
// debug Function
//
// Outputs debug information for cli arguments passed in
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
// i        int         iterator
//
//*******************************************************************
void cliArgs::debug()
{
    cout << "NumFiles: " << numFiles << endl;
    cout << "TimeDelay: " << timeDelay << endl;
    cout << "ScanCount: " << scanCount << endl;
    cout << "TimeSet: " << ((timeSet) ? "true" : "false") << endl;
    cout << "ScanSet: " << ((scanSet) ? "true" : "false") << endl;
    for(int i=0; i < numFiles; i++) {
        cout << "Filename " << i << " has value " << fileNames[i] << endl;
    }
}

//********************************************************************
//
// getNumFiles Function
//
// Returns number of files passed in on command line
//
//
// Return Value
// ------------
// int
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
// n/a
//
//*******************************************************************
int cliArgs::getNumFiles()
{ return numFiles; }

//********************************************************************
//
// getFilename Function
//
// Returns the filename for the index number given
//
//
// Return Value
// ------------
// string
//
// Value Parameters
// ----------------
// num      int         index number to retrieve
//
// Reference Parameters
// --------------------
// n/a
//
// Local Variables
// ---------------
// error        string      string to construct and throw in error
//
//*******************************************************************
string cliArgs::getFilename(int num)
{
    //check to see that index is less than number of files given
    if(num < numFiles)
        return fileNames[num];
    else //throw an error
    {
        string error;
        error = printf("Error: File %d does not exist.", num);
        throw error;
    }
}

//********************************************************************
//
// Destructor Function
//
// Deallocate memory
//
//
// Return Value
// ------------
// n/a
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
// n/a
//
//*******************************************************************
cliArgs::~cliArgs()
{ delete fileNames; }
