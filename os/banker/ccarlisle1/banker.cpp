/********************************************************************
||Name:       Christopher Carlisle
||COURSE:     COSC 3346
||*****************************************************************/
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/************************************************
|| banker struct
|| Purpose: makes it easier to pass values and matricies
************************************************/
struct banker
{
	int numProc;//  number of processes: n
	int resourceTypes;//  number of resource types: m
	int **allocMatrix;//  An n x m allocation matrix
	int **maxMatrix;//  An n x m max matrix
	int *availVector;//  A 1 x m available vector
	int procNumber; //process making request
	int *reqVector;//  A i : 1 x m request vector
	int **needMatrix; //An n x m need matrix
	bool beforeSafeState; //Flag for safe state. 
	bool afterSafeState; //Flag for safe state. 
	bool requestGranted; //Flag for if request can be granted
	int *updateAvail; //updated available vector
};

//Function prototype
void getFileName(char []); //get filename
void openVerifyFile(fstream &, char[]); //verify file works
banker loadData(fstream &);
banker compute(banker);
void print(banker);


int main(int argc, char *argv[])
{
	//Declarations
	banker state;
	fstream fsInputFile;

	//Get, open and verify file
	openVerifyFile(fsInputFile, argv[1]);

	//Grab data
	state = loadData(fsInputFile);

	//Compute matricies
	state = compute(state);
	
	print(state);
	
}

/************************************************
|| compute()
|| Purpose: Runs bankers algorithm and checks for 
||  	   safe state and then makes a request
|| 	   checks again and then returns a banker
||  	   struct.
************************************************/
banker compute(banker state)
{
	state.beforeSafeState=0;
	state.afterSafeState=1;
	state.requestGranted=0;
	//Declarations
	int workingMatrix[state.numProc][state.resourceTypes];
	int workingVector[state.resourceTypes];
	int countCompleted=0;
	bool found=1;
	bool checkLastCol=1; //determines if last column should be checked
	bool procFound=0;
	int numZeroCols=0;
	int finished[state.numProc];

	for(int i=0; i < state.numProc; i++)
		finished[i] = 0;

	//Find need matrix
	state.needMatrix = new int*[state.numProc];
	for(int i=0; i < state.numProc; i++)
		state.needMatrix[i] = new int[state.resourceTypes];
		
	for(int r=0; r < state.numProc; r++)
	{
		for(int c=0; c < state.resourceTypes; c++)
		{
			state.needMatrix[r][c] = state.maxMatrix[r][c] - state.allocMatrix[r][c];
		}
	}

	//before safe state
	//copy need matrix to working matrix
	for(int r=0; r < state.numProc; r++)
	{
		for(int c=0; c < state.resourceTypes; c++)
		{
			workingMatrix[r][c] = state.needMatrix[r][c];
		}
	}

	//copy available vector to working vector
	for(int i=0; i < state.resourceTypes; i++)
	{
		workingVector[i] = state.availVector[i];

	}

	//compute
       	while (countCompleted != state.numProc && found == 1)
       	{
	       found = 0;
	       
	       //go through each process that hasn't
	       //been freed yet
	       for(int i=0; i < state.numProc; i++)
	       { 
		       procFound=0;
		       checkLastCol=1;
		       numZeroCols=0;

		       //check columns for current process
		       for(int c=0; c < state.resourceTypes-1; c++)
		       {

			       if(workingMatrix[i][c] > workingVector[c])
			       {
				       checkLastCol=0;
				       break;
			       }
		       }

		       //check the last column and if true then 
		       //we have found a process that will work
		       if(checkLastCol && 
		          workingMatrix[i][state.resourceTypes-1] <
			  workingVector[state.resourceTypes-1] &&
			  finished[i]==0)
		       {
			       procFound=1;
			       finished[i]=1;
		       }

		       
		       //move resources around if current process
		       //will work
		       if(procFound)
		       {
			       //A = A + Cp  add resources back in
			       for(int col=0; col < state.resourceTypes; col++)
				       workingVector[col] += state.allocMatrix [i][col];
			       /* p can obtain all it needs.         */
			       /* assume it does so, terminates, and */
			       /* releases what it already has.      */
			       //P = P − {p} remove process
			       for(int col=0; col < state.resourceTypes; col++)
				       workingMatrix[i][col]=0;
			       found = 1;
			       countCompleted++;
		       }
	       }
	       if (!found)
		       break;	    
	}

	if(countCompleted == state.numProc)
	{
		state.beforeSafeState=1; //safe state before request
	}
	
	if(state.beforeSafeState)
	{
		//after request safe state
		//copy need matrix to working matrix
		for(int r=0; r < state.numProc; r++)
		{
			for(int c=0; c < state.resourceTypes; c++)
			{
				workingMatrix[r][c] = state.needMatrix[r][c];
			}
		}

		//copy available vector to working vector
		for(int i=0; i < state.resourceTypes; i++)
		{
			workingVector[i] = state.availVector[i];

		}

		//check request
		state.requestGranted=1;
		for(int i=0; i < state.resourceTypes; i++)
		{
			if(state.reqVector[i] > workingVector[i] ||
					state.reqVector[i] >
					state.needMatrix[state.procNumber][i]) 
			{
				state.requestGranted=0;
				break;
			}
		}

		//make request
		if(state.requestGranted)
		{
			//add resources back to working vector
			for(int i=0; i < state.resourceTypes; i++)
			{
				state.updateAvail[i] = state.availVector[i] - state.reqVector[i];
				cout << state.updateAvail[i] << "\n";
			}

			for(int i=0; i < state.resourceTypes; i++)
				workingVector[i] += state.allocMatrix [state.procNumber][i];

			//set process number for request to 0 in working matrix
			for(int i=0; i < state.resourceTypes; i++)
				workingMatrix[state.procNumber][i] = 0;


		}

		//compute
		while (countCompleted != state.numProc && found == 1 &&
				state.requestGranted)
		{
		       found = 0;
		       
		       //go through each process that hasn't
		       //been freed yet
		       for(int i=0; i < state.numProc; i++)
		       { 
			       procFound=0;
			       checkLastCol=1;
			       numZeroCols=0;

			       //check columns for current process
			       for(int c=0; c < state.resourceTypes-1; c++)
			       {
				       if(workingMatrix[i][c] == 0)
					       numZeroCols++;

				       if(workingMatrix[i][c] > workingVector[c])
				       {
					       checkLastCol=0;
					       break;
				       }
			       }

			       //check the last column and if true then 
			       //we have found a process that will work
			       if(checkLastCol && 
				  workingMatrix[i][state.resourceTypes-1] <
				  workingVector[state.resourceTypes-1]
				  && !(numZeroCols == state.resourceTypes-1 
				  && workingMatrix[i][state.resourceTypes-1] == 0))
				       procFound=1;

			       
			       //move resources around if current process
			       //will work
			       if(procFound)
			       {
				       //A = A + Cp  add resources back in
				       for(int col=0; col < state.resourceTypes; col++)
					       workingVector[col] += state.allocMatrix [i][col];
				       /* p can obtain all it needs.         */
				       /* assume it does so, terminates, and */
				       /* releases what it already has.      */
				       //P = P − {p} remove process
				       for(int col=0; col < state.resourceTypes; col++)
					       workingMatrix[i][col]=0;
				       found = 1;
				       countCompleted++;
			       }
		       }
		       if (!found)
		       {
			       state.requestGranted=0;
			       break;	    
		       }
		}
	}
	else
		state.afterSafeState = 0;

	return state;

}

/************************************************
|| loadData()
|| Purpose: loads data from the file
************************************************/
banker loadData(fstream & fsInputFile)
{
	//Declarations
	banker state;
	string tempString;

	//number of processes
	getline(fsInputFile,tempString);
	state.numProc = atoi(tempString.data());

	//skip a line
	getline(fsInputFile,tempString);

	//number of resource types
	getline(fsInputFile,tempString);
	state.resourceTypes = atoi(tempString.data());

	//skip a line 
	getline(fsInputFile,tempString);

	//load allocation matrix
	state.allocMatrix = new int*[state.numProc]; //allocate memory for matrix
	for(int i=0; i < state.numProc; i++)
		state.allocMatrix[i] = new int[state.resourceTypes];
	
	for(int r=0; r < state.numProc; r++)
	{
		//grab each column except last using ' ' as delimiter
		for(int c=0; c < state.resourceTypes-1; c++)
		{
			getline(fsInputFile, tempString, ' ');
			state.allocMatrix[r][c] = atoi(tempString.data());
		}

		//grab the last column and then move to next row
		getline(fsInputFile, tempString);
		state.allocMatrix[r][state.resourceTypes-1] = atoi(tempString.data());
	}

	//skip a line
	getline(fsInputFile,tempString);

	//load max allocation matrix
	state.maxMatrix = new int*[state.numProc]; //allocate memory for matrix
	for(int i=0; i < state.numProc; i++)
		state.maxMatrix[i] = new int[state.resourceTypes]; 

	for(int r=0; r < state.numProc; r++)
	{
		//grab each column except last using ' ' as delimiter
		for(int c=0; c < state.resourceTypes-1; c++)
		{
			getline(fsInputFile, tempString, ' ');
			state.maxMatrix[r][c] = atoi(tempString.data());
		}

		//grab the last column and then move to next row
		getline(fsInputFile, tempString);
		state.maxMatrix[r][state.resourceTypes-1] = atoi(tempString.data());
	}
	
	//skip a line
	getline(fsInputFile, tempString);

	//load available vector
	state.availVector = new int[state.resourceTypes]; //allocate memory for vector
	for(int i=0; i < state.resourceTypes-1; i++) //grab all but the last column
	{
		getline(fsInputFile, tempString, ' ');
		state.availVector[i] = atoi(tempString.data());
	}
	getline(fsInputFile, tempString); //get last column
	state.availVector[state.resourceTypes-1] = atoi(tempString.data());

	//updated available vector
	state.updateAvail = new int[state.resourceTypes]; //allocate memory for vector
	for(int i=0; i < state.resourceTypes; i++)
		state.updateAvail[i] = state.availVector[i];

	//skip a line
	getline(fsInputFile, tempString);


	//load process requesting
	getline(fsInputFile, tempString, ':');
	state.procNumber = atoi(tempString.data());

	//load request vector
	state.reqVector = new int[state.resourceTypes]; //allocate memory for vector
	for(int i=0; i < state.resourceTypes-1; i++) //grab all but the last column
	{
		getline(fsInputFile, tempString, ' ');
		state.reqVector[i] = atoi(tempString.data());
	}
	getline(fsInputFile, tempString); //get last column
	state.reqVector[state.resourceTypes-1] = atoi(tempString.data());


	return state;
}

/************************************************
|| openVerifyFile()
|| Purpose: open filestream
************************************************/
void openVerifyFile(fstream & fsFileName, char cstrFileNameIn[])
{
	fsFileName.open(cstrFileNameIn); //file is opened
	if(!fsFileName)
	{
		cout << "Bad file name, exiting... \n";
		exit(-1); //exits program
	}
}

/************************************************
|| print()
|| Purpose: prints out results
************************************************/
void print(banker state)
{
	char letter = 'A';
	
	//Number of processes
	cout << "There are " <<  state.numProc << " processes in the system.\n\n";

	//Number of resources
	cout << "There are " <<  state.resourceTypes << " resource types.\n\n";

	//Allocation matrix
	cout << "The Allocation Matrix is...\n";
	cout << "   ";
	for(int i=0; i < state.resourceTypes; i++)
		cout << (char) (letter+i) << " ";

	cout << "\n";
	for(int r=0; r < state.numProc ; r++)
	{
	       	cout << r << ": ";
		for(int c=0; c < state.resourceTypes; c++)
		{
			cout << state.allocMatrix[r][c] << " ";

		}
		cout << endl;
	}
	cout << endl;

	//Max matrix
	cout << "The Max Matrix is...\n";
	cout << "   ";
	for(int i=0; i < state.resourceTypes; i++)
		cout << (char) (letter+i) << " ";

	cout << "\n";
	for(int r=0; r < state.numProc ; r++)
	{
	       	cout << r << ": ";
		for(int c=0; c < state.resourceTypes; c++)
		{
			cout << state.maxMatrix[r][c] << " ";

		}
		cout << endl;
	}
	cout << endl;

	//Need matrix
	cout << "The Need Matrix is...\n";
	cout << "   ";
	for(int i=0; i < state.resourceTypes; i++)
		cout << (char) (letter+i) << " ";

	cout << "\n";
	for(int r=0; r < state.numProc ; r++)
	{
	       	cout << r << ": ";
		for(int c=0; c < state.resourceTypes; c++)
		{
			cout << state.needMatrix[r][c] << " ";

		}
		cout << endl;
	}
	cout << endl;
	
	//Available vector
	cout << "The Available Vector is...\n";
	for(int i=0; i < state.resourceTypes; i++)
		cout << (char) (letter+i) << " ";

	cout << "\n";
	for(int c=0; c < state.resourceTypes; c++)
	{
		cout << state.availVector[c] << " ";

	}
	cout << endl << endl;

	//State
	if(state.beforeSafeState)
		cout << "THE SYSTEM IS IN A SAFE STATE!\n\n";
	else
		cout << "THE SYSTEM IS NOT IN A SAFE STATE!\n\n"; 

	//Request vecotr
	cout << " The Request Vector is...\n";
	cout << "  ";
	for(int i=0; i < state.resourceTypes; i++)
		cout << (char) (letter+i) << " ";

	cout << "\n";
	cout << state.procNumber << ":";
	for(int c=0; c < state.resourceTypes; c++)
	{
		cout << state.reqVector[c] << " ";

	}
	cout << endl << endl;

	if(state.afterSafeState)
		cout << "THE REQUEST CAN BE GRANTED!\n\n";
	else
		cout << "THE REQUEST CANNOT BE GRANTED!\n\n";

	//Available vector
	if(state.afterSafeState)
	{
		cout << "The Available Vector is...\n";
		for(int i=0; i < state.resourceTypes; i++)
			cout << (char) (letter+i) << " ";

		cout << "\n";
		for(int c=0; c < state.resourceTypes; c++)
		{
			cout << state.updateAvail[c] << " ";
		}
		cout << endl << endl;
	}
}
