/*
Kathy Murphy
ECE 2220
Program 7
April 24th, 2016

	Purpose: Create a program that spawns 4 child processes, three to 
		 represent submarines reporting to a base, and a fourth to 
		 act as an interface for the user to enter commands. The user
		 will have various control over the submarines, which are to 
		 travel out a distance from base and then return to base after
		 finishing a mission. 

		 Known Bugs:

		 Assumptions:


*/

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

//This structure represents a submarine that exists in child processes 1-3 
typedef struct SUBMARINE {

 int serialNum; //Process ID of parent - sub's for current sub
 int fuel; //Value from 0 to 5000 to represent gallons of fuel remaining
 int missles; //Value from 0 to 10 to represent number of missles onboard
 int distance; //Number of miles away from base
 int direction; //Direction that the sub is heading (1 for mission, 0 for base)
}Submarine;

void updateTime(char* military, char* sysTime);

int main (void) {

 char* cTime; //Value that displays the time of the system as hh:mm:ss
 char* sysTime; //String that represents asctime(TIME_INFO)
 int parentID; //Parent's process ID
 time_t tInfo; //Information read in using the time() function
 struct tm* tStats; //Info from header in time.h
 int seeds[3]; //Three random numbers from the time seeded rand() 
 int i; //Loop variable

 cTime = (char*)malloc(9);
 sysTime = (char*)malloc(24);
 tStats = (struct tm*)malloc(sizeof(struct tm));
 cTime[8] = '\0';
 srand(time(NULL)); //Random numbers are different each time after execution
 time(&tInfo);
 tStats = localtime(&tInfo); 
 strcpy(sysTime,asctime(tStats));
 strncpy(cTime,asctime(tStats)+11,8);

 for(i=0;i<3;i++) {

	seeds[i] = rand() + 1;
	printf("Seed is %d\n",seeds[i]);
 }   

 parentID = getpid(); //Store parent ID so subs can be labeled by number 
 
 printf("\n\nMission Start Time and Date: %s\n\n", sysTime);
 
 //Spawn all child processes
 if(fork()!=0) { //Spawn Sub 1
	if(fork()!=0) { //Spawn Sub 2
		if(fork()!=0) { //Spawn Sub 3
			if(fork()!=0) { //Spawn Communication Terminal
			
				/* Base is Here*/
				wait();
				wait();
				wait();
				wait();

				printf("I waited for everyone to finish\n");
				return(0);


			}
			else { //Communication Terminal 

				printf("This is the input, %d\n", getpid());
				exit(getpid());
			}
		}
		else { //Sub 3

			srand(seeds[2]);
		}
	}
	else { //Sub 2
		
		srand(seeds[1]);
	}
 }
 else { //Sub 1

	srand(seeds[0]); 
 }
 
 Submarine cSub; //Create a submarine that represents the current process
 sleep(2);
 //Initialize the current sub
 cSub.serialNum = (int)getpid() - parentID; //Initialize sub number 
 cSub.fuel = (rand() % 4001) + 1000;
 cSub.missles = (rand() % 5) + 6;
 cSub.direction = 1;
 cSub.distance = 0;

 //Update Time info
 time(&tInfo);
 tStats = localtime(&tInfo); 
 strcpy(sysTime, asctime(tStats));
 strncpy(cTime, asctime(tStats)+11, 8);

 printf("I am sub #%d and the time is %s\n", cSub.serialNum, cTime);
 printf("I have %d missles and %d gallons of fuel\n\n",cSub.missles,cSub.fuel);
 exit(getpid());

}


