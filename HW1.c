#include <stdio.h>
#include <stdlib.h>

char fileName[20]; // file we open and read from
char newFileName[20]; // file we create and write to
char str[25]; // buffered string being read in from file

int main(){
	
	printf("Enter file name\n");
	gets(fileName);
	
	FILE *inputFile; //A pointer to a file
	inputFile = fopen(fileName, "r");
	
	printf("Enter new file name\n");
	gets(newFileName);
	
	FILE *outputFile;
	outputFile = fopen(newFileName, "w"); 

	while(fscanf(inputFile, "%s", str) != EOF){ //While I can read a complete word in from the file (IE, while file ain't even empty)
		fputs(str, outputFile);
		fputs(" ", outputFile); // so the new file includes spacing 
	}
	
	fclose(inputFile);
	fclose(outputFile);
	
	return 0;
}
