#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHMKEY (key_t)4444	//KEYS THAT ARE USED FOR THE ALLOCATION OF THE MEMORY
#define SEMKEY (key_t)8888
#define PERMS 0777 		//PERMISSIONS 

#define STRSIZE 40		//THE MAXIMUM SIZE THAT A MESSAGE CAN HAVE 

void up(int semaphore);	//FUNCTION THAT IMPLEMENTS THE up

void down(int semaphore);	//FUNCTION THAT IMPLEMENTS THE down

int myhash(char s[STRSIZE]);	//FUNCTION THAT IMPLEMENTS THE checksum OF A MESSAGE

typedef struct shared		//THE shared memory OF THE PROGRAM 
{
	char p1enc1[STRSIZE];	//THE shared data BETWEEN THE PROCESS P1 AND ENC1 
	int p1p2hash;		//THE checksum OF THE MESSAGE WHEN IT IS IN THE PROCESS ENC1
	char enc1chan[STRSIZE];//THE shared data BETWEEN THE PROCESS ENC1 AND CHAN
	char chanenc2[STRSIZE];//THE shared data BETWEEN THE PROCESS CHAN AND ENC2
	char enc2p2[STRSIZE];	//THE shared data BETWEEN THE PROCESS ENC2 AND P2
	int p2p1hash;		//THE checksum OF THE MESSAGE WHEN IT IS IN THE PROCESS ENC2
}shared;

