#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ftw.h>

char **x; 
int y;	   								

static int display(const char *fpath,const struct stat *sb,int tflag,struct FTW *ftwbuf){

    char *ext;
    
    if(strrchr(fpath,'.')!=NULL){   					//Checking wheather the pointer is in directory
	
    	ext = strrchr(fpath,'.');					//Checking for the extension given as the argument 
    	ext = ext + 1;							

    	if(y>1){  							//y defines the number of extensions provided as the argument 
   		for(int i=1;i<=y-1;i++){				//Loop through the extensions provided

   			if(!strcmp(ext,x[i])) 				//Check for the matching file extensions 
    				printf("%s\n", fpath);
    		}
    	}
    	else
    		printf("%s\n", fpath);  				//Print files irrespective of the extensions 
    		
    }
    
    return 0;
}

int main(int argc,char *argv[])
{
    x = argv; 								//Assigning the aruguments globally
    y = argc;
    
    int flags = FTW_PHYS | FTW_CHDIR | FTW_MOUNT;  			// nftw requires flags to be set to parse the directory tree
    
    static char *dirPath;    
    									//Assigned if the desired path is provided in the argument
    if (argc > 1) {
    	
    	if ( !strcmp( argv[argc-2],"-t" )) {  			//To accept the directory path provided -t must be used 

    		dirPath=argv[argc-1];	   				
    		y-=2; 							//Removing the last 2 arguments as they are not file extensions
    	}
    	else
    		dirPath=".";   					//DEFAULT directory is current directory 
    }
    
    else{
 	   	dirPath=".";    					//DEFAULT directory is current directory
    }
    
    if (nftw(dirPath, display, 20, flags) == -1) {    		//nftw is called with required directory path and the funtion to print the files along with depth and flags 
    									//nftw returns -1 when the tree has reached its end 
        perror("nftw");						
        exit(EXIT_FAILURE);
    }
 
    exit(EXIT_SUCCESS);
}
