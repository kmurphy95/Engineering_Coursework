/*

Katherine Murphy
ECE 2220
Program 3
2-16-2016
	
	Purpose: Create an error detection program. The program will read in a
		 two digit ASCII input from the user and then translate that 
		 phrase into a code word by adding 5 error preventing parity
		 bits after converting the user's input to binary values. 
		 The program will then print back to the screen the 
		 user's input and the modified codeword with the parity bits
		 added.

		Known bugs: The final parity bits are out of order for some
			    reason. This is likely a counting mistake when
		    	    bit shifting the final code word into place with
			    room left for the parity bits to be 'or'd in later. 


*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main (void) {
 
 unsigned char keyPhrase[10] = "TERMINATE"; //Phrase used to exit the program
 unsigned char userInput[10]; //Array to store what user enters when prompted
 unsigned char userChars[2]; //First two characters from userinput 
 int pBit[5]; //Parity bits 1,2,4,8, and 16
 int i; //Array index variable
 int count; //counting variable
 int codeWordFirst; //Integer value to store the information word
 int codeWordFinal; //Final binary codeWord to print to screen at end
 int tempWord; //Temp variable for making the code word from the info word
 int mask; //Bitmasking variable
 int infoBits[16]; //Array to hold the binary value of userChars
 int tempTotal; //Integer to add sum of infoBits for calculating parity bits
 int parityWord; //binary representation of the parity values in their indices
 
 //Loop iterates until the user enters the exit word
 while(strcmp(keyPhrase,userInput) != 0) {
	
	codeWordFirst = 0x00000000;
	codeWordFinal = 0x00000000;
	//Prompts the user to enter a string exactly two characters long
	do {
 		printf("Enter a two character ASCII phrase or TERMINATE ");
		printf("to end the program: "); 
 		fgets(userInput,sizeof(userInput),stdin);
 		sscanf(userInput,"%s",userInput);

	} while(strlen(userInput)<2 || (strlen(userInput)>2 &&(strcmp(userInput,keyPhrase)!=0)));
 
	if(strcmp(userInput,keyPhrase) == 0) {

		break;
	}


	//Constructs the two character phrase from input
 	for(i=0;i<2;i++) {

		userChars[i] = userInput[i];
	}
 	printf("\n\n");

	//The following few lines are used to display the user's initial info
	//in char, binary and hexidecimal formats
	mask = 0x80; //1000000 in binary

	printf("\t\t\t %c\t  %c\n",userChars[1],userChars[0]);
	printf("0x    00\t00\t%X\t %X\n",userChars[1],userChars[0]);
 	printf("-------- -------- ");	
	for(i=1;i>=0;i--) {

		mask = 0x80;
		while(mask>0) {

			printf("%d", (userChars[i] & mask) > 0);
			mask = mask >> 1;
		}
		printf(" ");
	}
	printf("\n");

	//Puts the binary values of userChar into codeWordFirst
	codeWordFirst = codeWordFirst | userChars[1];
	codeWordFirst = codeWordFirst << 8;
	codeWordFirst = codeWordFirst | userChars[0];

	//Creates a 16bit array of the values in codeWordFirst
	mask = 0x8000;
	i=15;
	while(mask>0) {

		infoBits[i]= (codeWordFirst & mask) > 0; 
		i--;
		mask = mask >> 1;
	}

	printf("\n\n");

	//For loop to calculate parity bits and store them based on the 
	//criteria set for each bit in the assignment
	for(i=0;i<5;i++) {

		tempTotal = 0;

		if(i==0) {
				
			tempTotal += infoBits[0]+infoBits[1]+infoBits[3];
			tempTotal += infoBits[5]+infoBits[6]+infoBits[8];
			tempTotal += infoBits[10]+infoBits[11]+infoBits[13];
			tempTotal += infoBits[15];
		}
		else if(i==1) {

                        tempTotal += infoBits[0]+infoBits[2]+infoBits[3];
                        tempTotal += infoBits[5]+infoBits[6]+infoBits[9];
                        tempTotal += infoBits[10]+infoBits[12]+infoBits[13];
		}
		else if(i==2) {
                    
		        tempTotal += infoBits[1]+infoBits[2]+infoBits[3];
                        tempTotal += infoBits[7]+infoBits[8]+infoBits[9];
                        tempTotal += infoBits[10]+infoBits[14]+infoBits[15];
		}
		else if(i==3) {

                        tempTotal += infoBits[4]+infoBits[5]+infoBits[6];
                        tempTotal += infoBits[7]+infoBits[8]+infoBits[9];
                        tempTotal += infoBits[10];
		}
		else {

                        tempTotal += infoBits[11]+infoBits[12]+infoBits[13];
                        tempTotal += infoBits[14]+infoBits[15];
		}

		//If the sum of the bits was an even number, pBit[i] = 0 
		//otherwise pBit[i] = 1
		if(tempTotal % 2 == 0) {

			pBit[i] = 0;
		}
		else {

			pBit[i] = 1;
		}
		printf("%d",pBit[i]);
	}


	printf("\n\n");

	//Displays Parity bits to user
	printf("The parity bits are ");
	
	for(i=0;i<5;i++) {

		printf("%d, ", pBit[i]);
	}
	printf("respectively for P1, P2, P4, P8 and P16.\n\n");		
	
	codeWordFinal |= (0x1F << 11)& codeWordFirst;
	codeWordFinal <<= 5;
	codeWordFinal |= (0x7F << 8)&codeWordFirst;
	codeWordFinal |= (0x7 << 4)&codeWordFirst;
	codeWordFinal |= (0x1 << 2)&codeWordFirst;
	
	mask=0x100000;
	while(mask>0){

		printf("%d",(codeWordFinal&mask) >0);
		mask >>=1;
	}
	printf("\n\n");

	//Constructs a binary code word containing parity values in their 
	//correct indices so that they may be easily inserted into the final
	//code word.
	parityWord = pBit[4];
	parityWord <<= 8; 
	parityWord += pBit[3];
	parityWord <<= 4;
	parityWord += pBit[2];
	parityWord <<= 2;
	parityWord += pBit[1];
	parityWord <<= 1;
	parityWord += pBit[0];


	printf("\n\n");

	codeWordFinal |= parityWord; //Inserts Parity bits to form final phrase

	printf("The final code word is displayed below: \n");
	printf("-------- ---");
	mask=0x100000; //10000000000000000 in binary
	count = 0;

	while(mask>0){

		if(count == 5 || count == 13) {

			printf(" ");
		}

		printf("%d", (codeWordFinal & mask) >0);
		mask >>= 1;
		count++; 
	}

	printf("\n\n");

	printf("In Hexidecimal:\n");
	printf("%08X\n\n",codeWordFinal);	
 }

 return(0);

}
