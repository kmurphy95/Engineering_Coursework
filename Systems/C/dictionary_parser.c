/*

Kathy Murphy
ECE 2220
Program 5
3/23/2016
		Purpose: Create a program that will read in a file that 
		contains the contents of a dictionary. Then, after reading
        	that file, the program will display various information about
		the contents of the dictionary. This will be accomplished
        	using string functions, and the output will be displayed in
        	stdout.

		Known Bugs: Attempting to free "dictionaryContent" in the 
			    main program causes an "invalid pointer" error.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void leftLong(char* dictionary); //Display the longest word typed with LH
void rightLong(char* dictionary);//Display the longest word typed with RH
void firstRows(char* dictionary);//Display longest word typed with 1st 3 R
void dousFind(char* dictionary);//Display all words ending in "dous" 
void palindrome(char* dictionary);//Display all palindromes
void fiveVowels(char* dictionary);//Display words with 5 vowels in order
void leftPercent(char* dictionary);//Display % time left hand used to spell

int main (void) {
 
 FILE* fp; //File pointer to the dictionary file
 char* dictionaryContent; //Character pointer to the dictionary
 int count;



 fp = fopen("Dictionary.txt","r"); 
 
 if(fp==NULL) {

	printf("The file was invalid.");
	return(0);
 }

 
 fseek(fp,0,SEEK_END);
 count = ftell(fp);

 rewind(fp);

 dictionaryContent = malloc(count+1);
 fread(dictionaryContent,count,1,fp);
 *(dictionaryContent+count) = '\0';

 leftLong(dictionaryContent);
  
 free(dictionaryContent); 
 fclose(fp);

 return(0); 
}

//Displays the longest word typed only with the characters on the left hand 
//side of the keyboard "QWERTASDFGZXCV"
void leftLong(char* dictionary) {

 char* leftH; //Pointer to all lower-case left hand letters
 char* leftU; //Pointer to all upper-case left hand letters
 char* dictCpy;
 char* temp;
 char* longest;
 char* hitList;
 int maxL;
 int count;

 dictCpy = (char*)malloc(strlen(dictionary));
 leftH = (char*)malloc(15);
 leftU = (char*)malloc(15);
 temp = (char*)malloc(32);
 hitList  = (char*)malloc(strlen(dictionary));
 longest = (char*)malloc(32);

 strcpy(dictCpy,dictionary);
 leftH = "qwertasdfgzxcv\0";
 leftU = "QWERTASDFGZXCV\0";
 maxL = 0;

 while(strlen(dictCpy) != 0) {

	
	count = strchr(dictCpy,'\n') - dictCpy;
	strncpy(temp,dictCpy,count);
	temp[count] = '\0';	
	
	if((strspn(temp,leftH) + strspn(temp,leftU)) == count) {


		strcpy(hitList+(strlen(hitList)),temp);
		strcpy(hitList+(strlen(hitList)),"\n");

		if(count > maxL) {

			maxL = count;
		}
	}
		
	dictCpy = dictCpy + (count+1);

 }
 
 while(strlen(hitList) != 0){

	count = strchr(hitList,'\n') - hitList;
	strncpy(temp,hitList,count);
	temp[count] = '\0';

	if(strlen(temp) == maxL) {

		strcpy(longest+(strlen(longest)),temp);
		strcpy(longest+(strlen(longest)),"\n");
	}

	hitList = hitList+count+1;
 }

 printf("The longest word(s) typed with the left hand only is:\n\n");
 printf("%s\n\n",longest);

}

//Displays the longest word typed only with the characters on the right hand 
//side of the keyboard "YUIOPHJKLBNM"
void rightLong(char* dictionary) {

 
 
}

//Displays the longest word typed only with the first three rows on the keyboard
void firstRows(char* dictionary) {

 
}

//Displays all words that end with "dous"
void dousFind(char* dictionary) {

 char* dousList; //dous

 dousList = (char*)malloc(5);

 dousList = "dous";

}

//Displays all words that are spelled the same way forward as they are 
//in reverse
void palindrome(char* dictionary) {

 char* backward; //Pointer to the reverse of a word


   
}

//Displays the words that have the sequence "aeiou" in them somewhere
void fiveVowels(char* dictionary) {

 char* vowels; //aeiou

 vowels = (char*)malloc(6);

 vowels = "aeiou";


 free(vowels);
}

//Displays the percentage of the time the left hand is used to type all words
void leftPercent(char* dictionary) {

 char* leftH; //All left characters

 leftH = (char*)malloc(15);
 

 leftH = "qwertasdfgzxcv";

 
 free(leftH);
}


 
