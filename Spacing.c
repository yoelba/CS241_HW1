// this only runs once, it should use a loop until EOF in main()
char spacing(int length, char line){

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
