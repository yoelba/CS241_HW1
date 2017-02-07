/*
Kristina Kolibab and Yoel Ben-Avraham
Assignment 1 - CS241
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void formatter( FILE *, FILE *, int );

/* GLOBALS  */
char fileName[30];
char newFileName[30];

int spaceCounter(char *currentLine){ //Pass in a character array terminated by a 'endline' character. Returns the number of spaces in the line.
        int spcCtr = 0;
        for(int i = 0; i < strlen(currentLine); i++){
                if(currentLine[i] == ' '){
                        spcCtr++;
                }
        }
        return spcCtr;
}

int main( int argc, char *argv[] ){

        FILE *ifp, *ofp;
        clock_t begin_time, end_time;
        double time_used;
        
        if(argc < 4){
                fprintf(stderr, "Not enough arguments\n");
                exit(1);
        }
        if(!(ifp = fopen(argv[1], "r"))){
                fprintf(stderr, "Cannot open file %s\n");
        }
        if(!(ofp = fopen(argv[2], "w"))){
                fprintf(stderr, "Cannot open file %s\n");
        }

        begin_time = clock();
        formatter( ifp, ofp, atoi(argv[3]));
        end_time = clock();
        time_used = (double)(end_time - begin_time)/CLOCKS_PER_SEC;
        fprintf(stderr, "Time usage = %17.13f\n", time_used);

        fclose(ifp);
        fclose(ofp);

}

void formatter( FILE * inputFile, FILE *outputFile, int lineLength ){

        char currentWord[200]; //an array of characters. 
        char currentLine[200]; //Another array of characters
        memset(currentLine, 0, 200);

        while(fscanf(inputFile, "%s", currentWord) != EOF){ //While I can read a complete word in from the file (IE, while file ain't even empty)

                if(strlen(currentLine)+strlen(currentWord) <= (lineLength-1)){
                        if(strlen(currentLine) != 0){
                                strcat(currentLine, " " );
                        }
                        strcat(currentLine, currentWord); //Add the word to the current line if it's short enough.
                }else{
                        strcat(currentLine, "\n"); //Append endline character, the final capstone on the contruction of each line! 
                        
                        div_t divInfo; //Gotta get me some quotient and remainder you know
                        int spcCnt = spaceCounter(currentLine);
                        if(spcCnt != 0){
                                divInfo = div( (lineLength - (strlen(currentLine)-1)), spcCnt); //Substract 1 from the strlen to not count the \n char
                        } else {
                                divInfo = div(0,1);
                        }
                        int i = 0;
                        int remainders = divInfo.rem;
                        int remAdd;
                        if(remainders > 0){
                                remAdd = 1;
                        } else {
                                remAdd = 0;
                        }

                        while(currentLine[i] != '\n'){
                                if(currentLine[i] == ' '){
                                        for(int j = 0; j<=divInfo.quot+remAdd; j++){
                                                fputs(" ", outputFile);
                                                printf(" ");
                                        }
                                        if(remainders>0){
                                                remainders--;
                                                if(remainders == 0) remAdd = 0;
                                        }
                                }else{
                                        printf("%c", currentLine[i]);
                                        fputc(currentLine[i], outputFile);
                                }
                                i++;
                        }
                        fputs("\n", outputFile);
                        printf("\n");


                        memset(currentLine, 0, 200); //clear the line, for it must be reborn anew as the next line, phoenix-like!
                        strcat(currentLine, currentWord); //and thus, we immediately begin the contruction of this next line.
                }
        }
}
