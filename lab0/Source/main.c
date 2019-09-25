/***********************************
            Includes
***********************************/
#include "main.h"
#include <sys/resource.h> 
 #include <sys/time.h> 
 #include <unistd.h> 
/**********************************
        Main Function
**********************************/
int main(int argc, char *argv[])
{
    struct rlimit rl;
    // First get the limit on memory 
   getrlimit (RLIMIT_AS, &rl); 
  
   printf("\n Default value is : %lld\n", (long long int)rl.rlim_cur); 
   printf("\n Default value is : %lld\n", (long long int)rl.rlim_max); 

   // Change the limit 
   rl.rlim_cur = 500000000; 
   rl.rlim_max = 500000000; 
  
   // Now call setrlimit() to set the  
   // changed value. 
   setrlimit (RLIMIT_AS, &rl); 
  
  
  setrlimit (RLIMIT_DATA, &rl); 
   // Again get the limit and check 
  // getrlimit (RLIMIT_AS, &rl); 

   //getrlimit (RLIMIT_AS, &rl); 

   //RLIMIT_DATA
  
 //  printf("\n Default value now is : %lld\n", (long long int)rl.rlim_cur); 
  

    int c;
	FILE *input, *output;
    char * line = NULL;	
    size_t len = 0;
    uint32_t total_elts = 0;
    char outputfilename[20];
    char inputfilename[20];
    int8_t arg_filename = 0;
    char buffer[20];
    int32_t *input_array;

    if(argv[1]== NULL) 
    {
        printf("Please provide arguments\n");
        printf("./mysort [Sourcefilename] -o [outputfilename]\nSource filename mandatory\n");
        exit(1);

    }

    //long option for command line arguments
    static struct option longopts[] =
    {
        {"name", no_argument, NULL, 'n'},
        {NULL, 0, NULL, 0}
    };

    //parse the input file if given as command line argument
    if(strcmp(argv[1],"-o") == 0 )
    {
        //if not provided, go with the default input file
        printf("./mysort [Sourcefilename] -o [outputfilename]\nSource filename mandatory\n");
        exit(1);
    }
    else
    {
        strcpy(inputfilename,argv[1]);
    }

    //parsing command line arguments
    while ((c = getopt_long (argc, argv, "no:", longopts, NULL)) != -1)
    {
        switch (c)
        {
            case 'n':
                printf("\nSteve Antony Xavier Kennedy\n");
                exit(1);
                break;
            case 'o':
                strcpy(outputfilename,optarg);
                arg_filename = 1;
                break;
            default:
                printf("No arguments provided\n");
                exit(1);
        }
    }

    printf ("inputfilename = %s\n",inputfilename);

	input = fopen(inputfilename,"r");
	if(input == NULL)
	{
		printf("Couldn't open file as it doesn't exists\n");
		exit(1);
	}

	struct stat st;
    if(stat(inputfilename, &st) != 0) 
    {
        return 0;
    }

	//find the number of integers in the input file
	while((getline(&line, &len, input)) != -1)
	{
        total_elts++;

    }

    //reset the file pointer to the begining
    fseek(input, 0, SEEK_SET);

    //create an input array to store contents from the file
    input_array = malloc(total_elts * sizeof(int32_t));

    if((input_array == NULL) )
    {
    	printf("Couldn't malloc\n");
		return -1;
    }
    int32_t index = 0;

    //store the elements in an integer array
    while((getline(&line, &len, input)) != -1)
	{
        input_array[index] = atoi(line);
        index++;
    }
    fclose(input);

    printf("**********Unsorted array*****************\n");
    for(uint32_t i = 0; i < total_elts; i++)
    {
    	
		printf("%d\n",input_array[i]);
	}

	printf("Total elements %d\n",total_elts);

    //calls merge sort algorithm
	merge_sort(input_array,0,total_elts-1,total_elts);

	printf("**********Sorted array*****************\n");

    //if output file name is provided as command line argument
    if(arg_filename == 1)
    {
        output = fopen(outputfilename,"w");
        if(output == NULL)
        {
            printf("Couldn't open file\n");
            return -1;
        }
    }

	for(uint32_t j=0; j< total_elts; j++)
	{   
        //if output file is provided, store the sorted integers in the output file
        if(arg_filename == 1)
        {
            memset(buffer,'\0',sizeof(buffer));
            sprintf(buffer,"%d\n",input_array[j]);
            fwrite(buffer,1, strlen(buffer),output);
        }
        else
        {
            //print the sorted integers on the terminal if output file is not provided
            printf("%d\n", input_array[j]);
        }
		
	}

    if(arg_filename == 1)
    {
        printf("Sorted and Stored in %s \n",outputfilename);
        fclose(output);
    }  
    free(input_array);
    free(temp);
}


