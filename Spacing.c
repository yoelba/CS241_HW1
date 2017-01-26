// this only runs once, it should use a loop until EOF in main()
char[200] spacing(int length, char[200] line){

        if(sizeof(line) == length){
                // do nothing, move to next line
        } else {
                // iterate through line, adding spaces
                while(sizeof(line) != length){
                        /* read through characters until first 
                        space, then add space */
                        int c;
                        while((c = getc(line)) != " "){
                                // do nothing, wait until space
                        } 
                        putchar(" "); // now add the space
                }
        }
        return line;
} 


int main(){ //Let's test
	char testLine[200] = "dingus dongus"; // 7 character line as input 
	testLine = spacing(10, testLine); //Add spaces till it hits 10 chars total
	printf("%s", testLine);
	return 0;
}
