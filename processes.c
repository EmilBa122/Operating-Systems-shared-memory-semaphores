#include "structure.h"

int main()
{
	//shared memory
	shared* shm;
	//semaphores
	int semp1enc1, semenc1chan, semchanenc2, semenc2p2;
	int semp2enc2, semenc2chan, semchanenc1, semenc1p1;
	int p2finished;
	
	/////////////////////CREATE AND ATTACH ON THE shared memory///////////////////////////////
	int shmid;
	
	shmid=shmget(SHMKEY,sizeof(shared),PERMS|IPC_CREAT); //CREATE THE shared memory
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
	
	////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	////////////////////CREATE THE semaphores AND INITILIZE THEM//////////////////////////
	semp1enc1=semget(SEMKEY+1,1,PERMS|IPC_CREAT); //CREATE THE semaphores
   	if(semp1enc1 == -1)
   	{
		printf("semget: semget failed\n");//if get FAILS END THE PROGRAM
		return -1;
   	}

	semenc1chan=semget(SEMKEY+2,1,PERMS|IPC_CREAT); //CREATE THE semaphores
   	if(semenc1chan == -1)
   	{
		printf("semget: semget failed\n");//if get FAILS END THE PROGRAM
		return -1;
   	}

	semchanenc2=semget(SEMKEY+3,1,PERMS|IPC_CREAT); //CREATE THE semaphores
   	if(semchanenc2 == -1)
   	{
		printf("semget: semget failed\n");//if get FAILS END THE PROGRAM
		return -1;
   	}
	
	semenc2p2=semget(SEMKEY+4,1,PERMS|IPC_CREAT); //CREATE THE semaphores
   	if(semenc2p2 == -1)
   	{
		printf("semget: semget failed\n");//if get FAILS END THE PROGRAM
		return -1;
   	}
   	
   	p2finished=semget(SEMKEY+10,1,PERMS|IPC_CREAT); //CREATE THE semaphores
   	if(p2finished == -1)
   	{
		printf("semget: semget failed\n");//if get FAILS END THE PROGRAM
		return -1;
   	}
	
	//INITIALIZATION OF THE semaphores 
	int u=semctl(semp1enc1,0,SETVAL,0); 
   	if(u == -1)
   	{ 
		printf("semctl: semctl failed\n"); 
  	 	return -1; 
   	}

	u=semctl(semenc1chan,0,SETVAL,0); 
   	if(u == -1) 
   	{ 
		printf("semctl: semctl failed\n"); 
  	 	return -1; 
   	}

	u=semctl(semchanenc2,0,SETVAL,0); 
   	if(u == -1) 
   	{ 
		printf("semctl: semctl failed\n"); 
  	 	return -1; 
   	}	
	
	u=semctl(semenc2p2,0,SETVAL,0); 
   	if(u == -1) 
   	{ 
		printf("semctl: semctl failed\n"); 
  	 	return -1; 
   	}
   	
   	u=semctl(p2finished,0,SETVAL,0); 
   	if(u == -1) 
   	{ 
		printf("semctl: semctl failed\n"); 
  	 	return -1; 
   	}

	semp2enc2=semget(SEMKEY+5,1,PERMS|IPC_CREAT); //CREATE THE semaphores
   	if(semp2enc2 == -1)
   	{
		printf("semget: semget failed\n");//if get FAILS END THE PROGRAM
		return -1;
   	}

	semenc2chan=semget(SEMKEY+6,1,PERMS|IPC_CREAT); //CREATE THE semaphores
   	if(semenc2chan == -1)
   	{
		printf("semget: semget failed\n");//if get FAILS END THE PROGRAM
		return -1;
   	}

	semchanenc1=semget(SEMKEY+7,1,PERMS|IPC_CREAT); //CREATE THE semaphores
   	if(semchanenc1 == -1)
   	{
		printf("semget: semget failed\n");//if get FAILS END THE PROGRAM
		return -1;
   	}
	
	semenc1p1=semget(SEMKEY+8,1,PERMS|IPC_CREAT); //CREATE THE semaphores
   	if(semenc1p1 == -1)
   	{
		printf("semget: semget failed\n");//if get FAILS END THE PROGRAM
		return -1;
   	}
	
	//INITIALIZATION OF THE semaphores 
	u=semctl(semp2enc2,0,SETVAL,0); 
   	if(u == -1) 
   	{ 
		printf("semctl: semctl failed\n"); 
  	 	return -1; 
   	}

	u=semctl(semenc2chan,0,SETVAL,0); 
   	if(u == -1) 
   	{ 
		printf("semctl: semctl failed\n"); 
  	 	return -1; 
   	}

	u=semctl(semchanenc1,0,SETVAL,0); 
   	if(u == -1) 
   	{ 
		printf("semctl: semctl failed\n"); 
  	 	return -1; 
   	}	
	
	u=semctl(semenc1p1,0,SETVAL,0); 
   	if(u == -1) 
   	{ 
		printf("semctl: semctl failed\n"); 
  	 	return -1; 
   	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
	int i;
	int pid;
	
	//THE P1 WILL BE THE FATHER WHICH WILL CREATE THE OTHER 3
	//THE PROCESSES THAT ARE CREATED IN ORDER ARE THE FOLLOWING : ENC1, CHAN, ENC2 

	for(i=0; i<3; i++)
	{
		pid=fork();
		//THE CHILD PROCESS WILL DO break, WHILE THE FATHER (P1) WILL CONTINUE WITH THE CREATION OF THE REST
		if(pid==0)
		{
			break;
		}
	}
	
	//THE FIRST THAT DOES break IS THE ENC1, SO FROM THE break IT HAS i=0
	if(i==0)
	{
		while(1)
		{
			//I read from the P1
			down(semp1enc1);
			//I write to the chan
			strcpy(shm->enc1chan,shm->p1enc1);
			shm->p1p2hash=myhash(shm->p1enc1);//THE checksum OF THE MESSAGE IN THE ENC1 
			if(strcmp(shm->enc1chan,"TERM")==0)//WE SEE IF WE GOT THE TERM FROM THE P1
			{
				//I wake up the chan
				up(semenc1chan);
				return 0;
			}
			//I wake up the chan
			up(semenc1chan);
			//I wait to read from chan
			down(semchanenc1);
			strcpy(shm->p1enc1,shm->enc1chan);	
			if(strcmp(shm->enc1chan,"TERM")==0)//WE SEE IF WE GOT THE TERM FROM THE CHAN
			{
				//I wake up the P1
				up(semenc1p1);
				return 0;
			}
			//I wake up the P1
			up(semenc1p1);
		}
	}
	//THE SECOND THAT DOES break IS THE CHAN,SO FROM THE break IT HAS i=1
	if(i==1)
	{		
		while(1)
		{
			//I read from enc1
			down(semenc1chan);
			//I write to the enc2
			strcpy(shm->chanenc2,shm->enc1chan);			
			if(strcmp(shm->chanenc2,"TERM")==0)//WE SEE IF I GOT THE TERM FROM THE ENC1 
			{
				//I wake up the enc2
				up(semchanenc2);
				return 0;
			}
			//I wake up the enc2
			up(semchanenc2);
			//I wait to read from enc2
			down(semenc2chan);
			strcpy(shm->enc1chan,shm->chanenc2);
			if(strcmp(shm->chanenc2,"TERM")==0)//WE SEE IF I GOT THE TERM FROM THE ENC2
			{
				//I wake up the enc1
				up(semchanenc1);
				return 0;
			}
			//I wake up the enc1
			up(semchanenc1);
		}
	}
	//THE THIRD THAT DOES break IS THE ENC2, SO FROM THE break IT HAS i=2
	if(i==2)
	{
		while(1)
		{
			//I read from chan
			down(semchanenc2);
			//I write to the p2
			strcpy(shm->enc2p2,shm->chanenc2);
			if(strcmp(shm->enc2p2,"TERM")==0)//WE SEE IF I GOT THE TERM FROM THE CHAN
			{
				//I wake up the p2
				up(semenc2p2);
				return 0;
			}
			//I wake up the p2
			up(semenc2p2);
			//I wait to read from p2
			down(semp2enc2);
			strcpy(shm->chanenc2,shm->enc2p2);
			shm->p2p1hash=myhash(shm->enc2p2);//THE checksum OF THE MESSAGE IN THE ENC2
			if(strcmp(shm->enc2p2,"TERM")==0)//WE SEE IF I GOT THE TERM FROM THE P2
			{
				//I wake up the chan
				up(semenc2chan);
				return 0;
			}
			//I wake up the chan
			up(semenc2chan);
		}
	}
	//THE LAST IS THE P1 ITSELF, SO IT HAS i=3
	if(i==3)
	{
		while(1)
		{
			//the point where the p1 writes to the enc1 
			printf("DWSE MHNYMA STHN P1: \n");
			fgets(shm->p1enc1, STRSIZE, stdin);
			int l=strlen(shm->p1enc1);//THIS IS DONE SO THAT WE DO NOT HAVE THE CHARACTER OF THE enter WITH US
			shm->p1enc1[l-1] = '\0';
			//I wake up enc1
			up(semp1enc1);
			if(strcmp(shm->p1enc1,"TERM")==0)//WE SEE IF I GOT THE TERM FROM THE P1
			{
				down(p2finished);
				break;
			}
			//I wait to read from enc1
			down(semenc1p1);
			printf("TO MHNYMA APO THN P2->P1 EINAI :%s\n",shm->p1enc1);
			if(strcmp(shm->p1enc1,"TERM")==0)//WE SEE IF I GOT THE MESSAGE TERM FROM THE ENC1
			{
				down(p2finished);
				break;
			}
		}
		//BECAUSE THE P1 IS FATHER IT MUST WAIT FOR THE RETURN OF THE REST 
		for(i=0; i<3; i++)
		{
			wait(NULL);
		}
		//WE DEALLOCATE EVERYTHING 		
		shmctl(shmid,IPC_RMID,0);
		semctl(semp1enc1,0,IPC_RMID,0);
		semctl(semenc1chan,0,IPC_RMID,0);
		semctl(semchanenc2,0,IPC_RMID,0);
		semctl(semenc2p2,0,IPC_RMID,0);
		semctl(semp2enc2,0,IPC_RMID,0);
		semctl(semenc2chan,0,IPC_RMID,0);
		semctl(semchanenc1,0,IPC_RMID,0);
		semctl(semenc1p1,0,IPC_RMID,0);
		semctl(p2finished,0,IPC_RMID,0);	
	}
}



