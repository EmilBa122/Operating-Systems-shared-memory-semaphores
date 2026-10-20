#include "structure.h"

int main()
{
	shared* shm;	//shared memory
	int semp2enc2, semenc2p2, p2finished;	//semaphores

	int shmid;
	
	shmid=shmget(SHMKEY,sizeof(shared),PERMS|IPC_CREAT);	//CREATE THE shared memory.
	if(shmid<0)
	{
		printf("shmget: shmget failed\n");//if get FAILS END THE PROGRAM
  		return -1;
	}
	
	shm=shmat(shmid,NULL,0); //ATTACH ON THE shared memory        
	if(shm==(shared*)-1)
	{
		printf("error: shmat failed...\n");//if shmat FAILS END THE PROGRAM
		return -1;
	}

	semenc2p2=semget(SEMKEY+4,1,PERMS|IPC_CREAT); //CREATE THE semaphores
   	if(semenc2p2==-1)
   	{
		printf("semget: semget failed\n");//if get FAILS END THE PROGRAM
		return -1;
   	}
   	
   	semp2enc2=semget(SEMKEY+5,1,PERMS|IPC_CREAT); //CREATE THE semaphores
   	if(semp2enc2==-1)
   	{
		printf("semget: semget failed\n");//if get FAILS END THE PROGRAM
		return -1;
   	}
   	  	
   	p2finished=semget(SEMKEY+10,1,PERMS|IPC_CREAT); //CREATE THE semaphores
   	if(p2finished==-1)
   	{
		printf("semget: semget failed\n");//if get FAILS END THE PROGRAM
		return -1;
   	}
   	
   	while(1)
   	{
		//I read from enc2
		down(semenc2p2);
		printf("TO MHNYMA APO THN P1->P2 EINAI :%s\n",shm->enc2p2);
		if(strcmp(shm->enc2p2,"TERM")==0)//WE SEE IF WE GOT THE TERM FROM THE ENC2
		{
			up(p2finished);//I wake up the other processes that have not finished 
			return 0;
		}		
				
		printf("DWSE MHNYMA STHN P2: \n");
	
		fgets(shm->enc2p2, STRSIZE, stdin);
		int l=strlen(shm->enc2p2);//THIS IS DONE SO THAT WE DO NOT HAVE THE CHARACTER OF THE enter WITH US 
		shm->enc2p2[l-1] = '\0';
		
		if(strcmp(shm->enc2p2,"TERM")==0)//WE SEE IF IN THE P2 THE MESSAGE IS THE TERM
		{
			up(p2finished);//I wake up the other processes that have not finished 
			up(semp2enc2);//I wake up the enc2 
			return 0;
		}
		//I wake up the enc2
		up(semp2enc2);
	}	
	return 0;
}


