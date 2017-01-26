#include <string.h>
#include <stdio.h>
#include <stdlib.h>



//USE A LINKED LIST :)


int spaceCounter(char *currentLine){ //Pass in a character array terminated by a 'endline' character. Returns the number of spaces in the line.
        int spcCtr = 0;
        for(int i = 0; i < strlen(currentLine); i++){
                if(currentLine[i] == ' '){
                        spcCtr++;
                        printf("I got a space! ");
                }
        }
        return spcCtr;
}


int main(){
	FILE *inputFile; //A pointer to a file
	//inputFile = fopen("hollow.txt", "r"); //ALLOW USER TO INPUT FILE NAME IN FUTURE!
	inputFile = fopen("beeMovie.txt", "r");	
	
	FILE *outputFile; //Another pointer to another file
	outputFile = fopen("formattedText.txt", "w"); //ALLOW USER TO INPUT FILE NAME IN FUTURE!
	
	int lineLength = 20; // a l l o w   u s e r   i n p u t
	
	char currentWord[200]; //an array of characters. 
	char currentLine[200]; //Another array of characters
	memset(currentLine, 0, 200);
	
	while(fscanf(inputFile, "%s", currentWord) != EOF){ //While I can read a complete word in from the file (IE, while file ain't even empty)
		//fputs(currentLine, outputFile); //Put the word we read into the character array MyString
		if(strlen(currentLine)+strlen(currentWord) <= (lineLength - 1)){
			if(strlen(currentLine) != 0){
				strcat(currentLine, " " );
			}
			strcat(currentLine, currentWord); //Add the word to the current line if it's short enough.
		}else{
			strcat(currentLine, "\n"); //Append endline character, the final capstone on the contruction of each line!
			strcat(currentLine, NULL); 
			
			printf(currentLine); // d e b u g
			printf(strlen(currentLine)); //strlen is clearly not acting right :(
				
			for(int i = 0; i < strlen(currentLine); i++){ // d e b u g
				char c = currentLine[i];
				printf( c );	
			}

			div_t divInfo; //Gotta get me some quotient and remainder you know
			int spcCnt = spaceCounter(currentLine);
			divInfo = div( (lineLength - strlen(currentLine)), spcCnt);
			int i = 0;
			int remainders = divInfo.rem;
			while(currentLine[i] != '\n'){
				if(currentLine[i] == ' '){
					for(int j = 0; j<=(divInfo.quot/spcCnt)+remainders; j++){
						fputc(' ', outputFile);
					}
				}else{
					fputc(currentLine[i], outputFile);
				}
				i++;
			}
			
			memset(currentLine, 0, 200); //clear the line, for it must be reborn anew as the next line, phoenix-like!
			strcat(currentLine, currentWord); //and thus, we immediately begin the contruction of this next line.
		}
	}
	fputs(currentLine, outputFile);
	fclose(inputFile);
	fclose(outputFile);
	return 0;
}
