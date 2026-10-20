# Operating Systems shared memory semaphores

## Assignment Description
Two-Layer Message Passing Between Five Processes (P1, P2, ENC1, ENC2, CHAN) with 
Checksum Calculation using Shared Memory and Semaphores in C


**PROGRAM DESIGN IDEA :**

**INSTRUCTIONS FOR USE**

**THIS STRUCTURE CAN BE EXECUTED THROUGH 2 terminals WHERE IN THE FIRST ARE
WRITTEN THE MESSAGES THAT WILL BE SENT FROM THE P1 AND IN THE SECOND terminal
MESSAGES ARE SENT FROM THE P2 .THE P1 MUST ALWAYS SEND FIRST,WHEN THE P1 SENDS A
MESSAGE THE USER MUST WAIT UNTIL THE MESSAGE APPEARS IN THE terminal OF THE P2
WHERE EXACTLY AFTER THE P2 CAN SEND TO THE P1, THAT IS THE TURN IS ALTERNATING
AND IT IS NOT POSSIBLE FOR THE P1 AND THE P2 TO SEND MANY MESSAGES AT THE SAME
TIME.**

**THEREFORE FOR THE EXECUTION :**

1. **Open a Linux terminal**
2. **Write make e1**
3. **Write make e2**

   **(We assume that the first terminal will be for the P1 so open another
   terminal for the P2)**
4. **In the first terminal (P1) write ./e1**
5. **In the second terminal (P2) write ./e2**

**DESIGN CHOICES**

**IN THE PROGRAM THE shared memory ON WHICH THE PROCESSES WILL WRITE AND WILL
READ IS A struct (WITH DECLARATION shared). IN THIS struct WE HAVE 4 ARRAYS OF
SIZE STRSIZE** (STRSIZE is the maximum number of characters that the user can
type) **AND TWO INTEGER NUMBERS. EACH ARRAY REFERS TO THE TWO PROCESSES THAT
COMMUNICATE THAT IS THE ARRAY p1enc1 REFERS TO THE MESSAGES BETWEEN THE p1 AND
enc1,** the array enc1chan refers to the messages between the enc1 and chan, the
array chanenc2 refers to the messages between chan and enc2 and the array enc2p2
refers to the messages between enc2 and p2. **THE TWO INTEGER NUMBERS REPRESENT
THE checksum THAT IS CALCULATED IN THE enc1 AND enc2 WITH NAMES p1p2hash AND
p2p1hash RESPECTIVELY. FOR THE CORRECT COMMUNICATION BETWEEN THESE 5 PROCESSES I
HAVE USED 9 semaphores, FROM THESE I TAKE EIGHT SEMAPHORES AND I DIVIDE THEM
INTO GROUPS OF TWO SO THAT THE CORRECT FLOW OF MESSAGES BETWEEN THE 5 PROCESSES
IS CONTROLLED. THE NINTH SEMAPHORE IS USED WHEN THE MESSAGE TERM ENTERS THE
STRUCTURE WHERE IT GIVES THE "SIGNAL" FOR THE DEALLOCATION OF ALL THIS STRUCTURE
(p2finshed).**

IMPLEMENTATION OF THE EXERCISE

In the file strucure.h the shared memory on which the processes will write and
will read is declared (with declaration shared). Also the function up(int
semaphore) is declared which implements the up of a semaphore and
correspondingly we have the function down(int semaphore) which implements the
down of a semaphore. Finally we have the declaration of the function myhash(char
s[STRSIZE]) which implements the checksum of the message. The file structure.c
contains the implementations of all the functions that have been mentioned
before. In the file processes.c initially we include the structure.h then we
declare a pointer shm which will point to the shared memory of the exercise and
9 semaphores. Next we create and attach the shared memory and of course we
create the 9 semaphores and we initialize all of them to the value zero. **IN
THE MAIN PART OF THE FILE WE MAKE THE FOLLOWING ASSUMPTION THAT THE P1 WILL BE
THE FATHER WHICH WILL CREATE THE OTHER 3 PROCESSES : ENC1, CHAN, ENC2 (THROUGH
THE USE OF THE fork()) SO HE IS THE ONE RESPONSIBLE WHO DECIDES.** The first
child will be the ENC1 which initially will read from the P1 and will calculate
the value of the checksum of the message and will write it to the chan. If the
message from the P1 was the TERM then before it finishes it will wake up the
chan otherwise it wakes up the chan and waits to read from the chan, then it
checks if the message from the chan is the TERM then before it finishes it wakes
up the P1 otherwise it wakes up the P1 and waits to read from the P1. I follow
similar logic for the implementation of the second process (chan) and of the
third (ENC2). The last is the P1 itself which at the beginning asks the user to
give a message in order to write it to the enc1 and wakes it up it then sees if
the message was equal to TERM if yes it waits until the message arrives at the
p2 otherwise it waits to read from the enc1. If the message from or to the enc1
is equal to TERM then initially it waits for the return of the rest because it
is the father and then we deallocate everything that is the shared memory and
all the 9 semaphores. The process P2 is in the file p2.c which initially simply
attaches all this structure but does not have the responsibility of the
creation. Then it reads from the enc2 checks if the message from the enc2 is
TERM if yes then it gives the "signal" for the deallocation of all this
structure otherwise it asks the user for a message and writes it to the enc2 and
again checks if the message was equal to TERM and then wakes up the enc2 and
depending on the message waits to read from the enc2 or terminates.

**<u>HAS NOT BEEN IMPLEMENTED</u> the ability in the process CHAN to replace
characters of the message with others randomly based on a probability, the
ability of checking the checksum and of the retransmission of the message in
case of alteration by the processes ENC1 and ENC2.**
