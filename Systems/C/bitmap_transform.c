/*

Kathy Murphy
ECE 2220
Program 6
4-11-2016

	Purpose: Create a program that reads in color information from a .bmp
		 image and modifies the colors by RGB values passed in as 
		 command line arguments. The program then also creates a new
		 file with the modified image information displayed.

		 Known Bugs: Numerous errors when attempting to initialize space
			     associated with a new array of pixels past the 
			     original. Even initializing at the same time yields
			     strange segmentation faults. 

			     Edge Detection Portion completely omitted.

			     Shade Pixel section is non-functioning, as there
			     exists several errors associated with allocating,
			     initializing and operating on the array which have
			     not been identified. It is known that something is
			     is wrong with the section of code that transforms
			     the command line arguments for shade offsets to 
			     integers from strings, however it has not been 
			     pinpointed.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct {

 unsigned short int Type;
 unsigned int Size;
 unsigned short int Reserved1, Reserved2;
 unsigned int Offset;
}HEADER;

typedef struct {

 unsigned int Size;
 int Width, Height;
 unsigned short int Planes;
 unsigned short int Bits;
 unsigned int Compression;
 unsigned int ImageSize;
 int xResolution, yResolution;
 unsigned int Colors;
 unsigned int ImportantColors;
}INFOHEADER;

typedef struct {

 unsigned char Red, Green, Blue;
}PIXEL;


int main (int argc, char* argv[]){


 FILE *fpIn,fpOutE,fpOutS; //File pointers
 char* fileNameIn; //Name of the file being read
 char* fileNameOutE; //Name of the file being created for edge detection
 char* fileNameOutS; //Name of the file being created for shade change
 const char Matrix[3][3] = {{0,-1,0},{-1,4,-1},{0,-1,0}}; //2nd deriv filter
 PIXEL **myPixels; //Dynamically allocated array of pixels
 PIXEL** edgePix; //Array of pixels
 PIXEL** shadePix; //Array of pixels
 unsigned char* colorBytes; //List of all color values in order for each bit 
 unsigned char* shadeBytes; //Bytes with the offset added/subtracted 
 unsigned char temp; //Value for saving the index of the color values
 int i; //Loop variable
 int j; //Loop variable 
 INFOHEADER* infoPoint; //Pointer to infoheader information
 HEADER Header; //Header information
 char rSh, bSh, gSh; //Shading offset values passed in as cmd line args
 int count;


 if(argc != 6) {

	printf("Not enough arguments entered.\n");
	return(0);
 }

 fileNameIn = (char*)malloc(strlen(argv[1])+1); 
 fileNameOutE = (char*)malloc(strlen(argv[2])+7);
 fileNameOutS = (char*)malloc(strlen(argv[2])+8);

 
 infoPoint = (INFOHEADER*)malloc(40);

 fileNameIn = argv[1];

 if(*argv[3] != 43 && *argv[3] != 45) {

	printf("Error: red offset value not preceded by sign value.\n");
	return(0);
 }
 if(*argv[4] != 43 && *argv[4] != 45){

	printf("Error: green offset value not preceded by sign value.\n");
	return(0);
 }
 if(*argv[5] != 43 && *argv[5] != 45) {

	printf("Error: blue offset value not preceded by sign value.\n");
	return(0);
 }

 rSh=0;
 gSh=0;
 bSh=0;

 for(i=5;i>2;i--) {

	j=1;
	count = 1;

	temp = *argv[i+(strlen(argv[i])-count)];
	while(temp != 43 && temp != 45) {
		
		if(i==3){
			
			rSh += ((temp - 48) * j);
		}
		else if(i==4) {

			gSh += ((temp - 48) * j);
		}
		else if(i==5) {

			bSh += ((temp - 48) * j);
		}
	
		count++;
		j = j * 10;	

		temp = *argv[i+(strlen(argv[i])-count)];
		if(temp == 45) {

			if( i==5) {

				rSh = rSh * -1;

			}
			else if(i==4) {

				gSh = gSh * -1;
			}
			else if(i==3) {

				bSh = bSh * -1;
			}
		}
	}

 }

 if(rSh > 200 || rSh <-200) {

	printf("Red value entered is too high or low.\n");
	return(0);
 }
 if(gSh > 200 || gSh < -200) {

	printf("Green value entered is too high or low.\n");
	return(0);
 }
 if(bSh>200 || bSh<-200) {

	printf("Blue value entered is too high or low.\n");
	return(0);
 }
 

 fileNameOutE = argv[2];
 fileNameOutS = argv[2];

 strcat(fileNameOutE,"(edge)\0");
 strcat(fileNameOutS,"(shade)\0");

 //Check that the user picked two file names in command line
 if(strcmp(fileNameIn,argv[2]) == 0) {

	printf("Error: Input and Output file names must be different.\n");
	return(0);
 }



 fpIn = fopen(fileNameIn, "rb");

 //Ensure that the file opened properly
 if(fpIn == NULL) {

	printf("Error: file not read correctly.\n");	
        return(0);
 }

 fread(&Header,14,1,fpIn); //Read header info
 fread(infoPoint,40,1,fpIn); //Read the infoheader stuff from our pic
 fseek(fpIn,Header.Offset,SEEK_SET); 

 //Allocate space for all of the color values
 colorBytes = (unsigned char*)malloc(infoPoint->ImageSize);
 

 //Allocate space for three images worth of pixels
 myPixels = (PIXEL**)malloc(3*infoPoint->Height);
 edgePix = (PIXEL**)malloc(3*infoPoint->Height);
 shadePix = (PIXEL**)malloc(3*infoPoint->Height);

 for(i=0;i<infoPoint->Height;i++) {

	myPixels[i] = (PIXEL*)malloc(3*infoPoint->Width);
	edgePix[i] = (PIXEL*)malloc(3*infoPoint->Width);
	shadePix[i] = (PIXEL*)malloc(3*infoPoint->Width);
 } 


 fread(colorBytes,infoPoint->ImageSize,1,fpIn); //Get a list of all color vals
 
 //Flip the last and first of each of pair of three bytes (BGR to RGB)
 for(i=0;i<infoPoint->ImageSize; i+=3) {

	temp = colorBytes[i]; //B
	colorBytes[i] = colorBytes[i+2]; //Replace B with R
	colorBytes[i+2] = temp; //Put B back where R used to be 

 }

 
 //Loop that initializes the color values for each pixel in the original image

 for(i=0,count=0;i<infoPoint->Height;i++){
	for(j=0;j<infoPoint->Width;j++){

		myPixels[i][j].Red = colorBytes[count];
 		myPixels[i][j].Green = colorBytes[count+1];
		myPixels[i][j].Blue = colorBytes[count+2];
                shadePix[i][j].Red = colorBytes[count];
                shadePix[i][j].Green = colorBytes[count+1];
                shadePix[i][j].Blue = colorBytes[count+2];

		count = count + 3;
	}
 }	


 for(i=0;i<infoPoint->Height;i++){
	for(j=0;j<infoPoint->Width;j++){

		if(shadePix[i][j].Red + rSh > 255) {

			shadePix[i][j].Red = 255;
		}
		else{

	               shadePix[i][j].Red += rSh;

		}
		if(shadePix[i][j].Red +rSh < 0) {

			shadePix[i][j].Red = 0;
		}

		else{
        	       shadePix[i][j].Red += rSh;
	
		}
		if(shadePix[i][j].Green + gSh> 255) {

			shadePix[i][j].Green = 255;
		}

		else {

	               shadePix[i][j].Green += gSh;

		}
		if(shadePix[i][j].Green + gSh< 0) {

			shadePix[i][j].Green = 0;
		}

		else {
	               shadePix[i][j].Green += gSh;

		}
		if(shadePix[i][j].Blue + bSh> 255) {

			shadePix[i][j].Blue = 255;
		}

		else {
	               shadePix[i][j].Blue += bSh;

		}
		if(shadePix[i][j].Blue + bSh< 0) {

			shadePix[i][j].Blue = 0;
		}
		else {
	               shadePix[i][j].Blue += bSh;

		}

		//Flip it back to BGR
		temp = shadePix[i][j].Blue;
		shadePix[i][j].Blue = shadePix[i][j].Red;
		shadePix[i][j].Red = temp;

	}	

 }

 return(0);

}
