#include "structure.h"

void up(int semaphore)		//FUNCTION THAT IMPLEMENTS THE up
{
   	struct sembuf up[1]={0,1,0};	//up OPERATION

   	if(semop(semaphore,&up[0],1)==-1)//up the semaphore
   	{  
		printf("semop: semop failed\n"); //if semop FAILS
   	}
}

void down(int semaphore)	//FUNCTION THAT IMPLEMENTS THE down
{	   
	struct sembuf down[1]={0,-1,0}; //down OPERATION

   	if(semop(semaphore,&down[0],1)==-1)//down the semaphore
   	{		
		printf("semop: semop failed\n"); //if semop FAILS
   	}
}

int myhash(char s[STRSIZE])	//FUNCTION THAT IMPLEMENTS THE checksum OF A MESSAGE
{				//THE FUNCTION TAKES THE MESSAGE AND RETURNS AN INTEGER WHOSE VALUE IS THE ASCII 
	int sum=0;		//NUMBER OF ALL THE CHARACTERS THAT EXIST IN THIS MESSAGE 
	while(*s != '\0')
	{
		sum=sum+(*s);
		s++;
	}
	return sum;
}


