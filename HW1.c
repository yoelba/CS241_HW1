#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *inputFile; //A pointer to a file
	inputFile = fopen("hollow.txt", "r"); //ALLOW USER TO INPUT FILE NAME IN FUTURE!
	
	FILE *outputFile;
	outputFile = fopen("formattedText.txt", "w"); //ALLOW USER TO INPUT FILE NAME IN FUTURE!
	
	char myString[100]; //an array of characters. 

	while(fscanf(inputFile, "%s", myString) != EOF){ //While I can read a complete word in from the file (IE, while file ain't even empty)
		fputs(myString, outputFile);
	}
	
	fclose(inputFile);
	fclose(outputFile);
	
	return 0;
}
