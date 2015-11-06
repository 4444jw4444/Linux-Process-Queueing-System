#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>  //intptr_t
#include <time.h>
#include <string.h>

/* create thread argument struct for thr_func() */
typedef struct _customer{
	int id;
	int priority;
	int arrivalTime; 
  int serviceTime; 
}customer;

/* thread function */
void *print_function(void *ptr){
	customer * cPtr = (customer *)ptr;
	sleep(cPtr->arrivalTime);
  printf("Customer %2d has arrived at %2d seconds\n", cPtr->id, cPtr->arrivalTime);
  usleep(cPtr->serviceTime);
  printf("Customer %2d has been serviced for %2d decisenconds\n", cPtr->id, cPtr->serviceTime);
}

int main(int argc, char* argv[])
{
	int num; //customer number
	int i, rc, randNo;
 
 //variables needed to read in the customers.txt file
 char ch;
 char line[15];
 FILE *fp;

 //ERROR: too many arguments passed in
 //  if(sizeof(argv)/sizeof(char)!= 1){
 //   perror("Error: Wrong number of arguments.\n");
 //   exit(EXIT_FAILURE);
 //  }  
 
 //try to read the file
  fp = fopen(argv[1],"r"); // read mode
  
 ////ERROR: file unreadable 
  if( fp == NULL )
  {
     perror("Error while opening the file.\n");
     exit(EXIT_FAILURE);
  }

  //read the number of customers 
  num = fgetc(fp) - '0';
  fgetc(fp);
  //create the arrays of threads and customers based on num
  customer cusArray[num]; //customer array
	pthread_t thrdArray[num]; //thread array
  
  //show the content of the file
  printf("The contents of %s file are :\n", argv[1]);
  printf("%d\n",num);
  
  //get each line of the input file and create a thread based on it. 
  for (i = 0; fgets(line, sizeof(line), fp); ++i) {
//    int * attributes;
//    attributes = createAttributes(line);
//    cusArray[i].id = line[0] - '0';
//	  cusArray[i].arrivalTime = line[2] - '0';
//    cusArray[i].serviceTime = (line[4] - '0')*10;
//    cusArray[i].priority = line[6] - '0';  
    char *pChr = strtok(line, ":,");
    int j;
    for(j = 0; pChr != NULL; j++){
      if(j == 0){
        cusArray[i].id = atoi(pChr);
      }
      if(j == 1){
        cusArray[i].arrivalTime = atoi(pChr);
      }
      if(j == 2){
        cusArray[i].serviceTime = atoi(pChr);
      }
      if(j == 3){
        cusArray[i].priority = atoi(pChr);
      }
      pChr = strtok(NULL, ":,");
    }
	  if ((rc = pthread_create(&thrdArray[i], NULL, print_function, &cusArray[i]))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
	  }
  }

  //close the file
  fclose(fp);
	
	// block until all threads complete 
//	for (i = 0; i < num; ++i) {
//		pthread_join(thrdArray[i], NULL);
//	}
	sleep(10);//wait for customers leaving
	return EXIT_SUCCESS;
}

////takes in a line from the file and returns an array of 4 numbers,
////each of which is an attribute for a thread
//int * createAttributes(char line[15]){
//      printf("Line to create attributes from: %s\n", line);
//      const char colon[2] = ":";
//      const char comma[2] = ",";
//      char *idAndRemainder;
//      char *attrs;
//      static int attributes[4] = {0,0,0,0};
//      printf("Hello");
//      //pull out the id
//      idAndRemainder = strtok(line, colon);
//      
//      //save the id in the attributes array
//      attributes[0] = atoi(idAndRemainder);
//      //move past the id
//      idAndRemainder = strtok(NULL, colon);
//      
//      //split the non-id attributes by the comma
//      attrs = strtok(idAndRemainder, comma);
//      //save the arrival time in attributes array
//      attributes[1] = atoi(attrs);
//      //move to the service time
//      attrs = strtok(NULL, comma);
//      //save the service time in attributes array
//      attributes[2] = atoi(attrs);
//      //move to the priority
//      attrs = strtok(NULL, comma);
//      //save the priority in the attributes array
//      attributes[3] = atoi(attrs);
//           
//      return attributes;
//}