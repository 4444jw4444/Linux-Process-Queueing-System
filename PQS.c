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
  pthread_cond_t convar;
  int arrived;
  int running;
  int waiting;
  int finished; 
}customer;

/* customer function */
void *customer_function(void *ptr){
	customer * cPtr = (customer *)ptr;
  //wait until arrival time has passed
	sleep(cPtr->arrivalTime);
  //state that the customer has arrived
  printf("customer %2d arrives: arrival time (%d), service time (%d), priority (%2d)\n", cPtr->id, cPtr->arrivalTime, cPtr->serviceTime, cPtr->priority);
  //signal the clerk that this thread has arrived. Pass the id of the customer.
  //wait for a signal from the clerk saying that its turn has come
  //run until signalled to stop by the clerk or run out of time
    //if signalled to stop, got to above step where we were waiting
    //if run out of time, signal clerk that I am done
  //thread kill with wait to make sure prints happen
  sleep(1);
  pthread_exit(NULL);
}

/* clerk function */
void *clerk_function(void *customerArray){
  //waits (sleep, or convar, or mutex) until a customer signals arrival with id. Most likely a convar.
  //(A): if there is no currently running customer (running bool) then immediately signal start the newly arrived process
  //if there is a customer running (as noted by a running bool), then it is the highest priority of waiting and running customers so check if the newly arrived process is higher
    //if the new customer isn't higher, then add the customer to the waiting list
      //print out that the customer was blocked
    //if the new customer is higher, then signal stop to the currently running process and putting it back into the waiting list, and signal start to the new process.
      //print out that the customer was interupted by a higher priority
    //if there is a tie of priority give the priority to
      //the one that arrived first
      //then to the one with shortest service
      //then lower id number (it is assumed that id numbers increased linearly and so customers earlier in the file have lower ids)
  //wait for the running customer to signal completion
  //set running bool to false;
  //if another customer arrives, then go back to (A)
  //thread kill with wait to make sure that prints happen
  sleep(1);
  pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	int num; //customer number
	int i, rc;
 
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
	pthread_t thrdArray[num + 1]; //thread array
  
  //get each line of the input file and create a thread based on it.
  //initialize all of the threads with their properties 
  for (i = 0; fgets(line, sizeof(line), fp); ++i) {  
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
    cusArray[i].convar = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    cusArray[i].arrived = 0;
    cusArray[i].running = 0;
    cusArray[i].waiting = 0;
    cusArray[i].finished = 0;
	  if ((rc = pthread_create(&thrdArray[i], NULL, customer_function, &cusArray[i]))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
	  }
  }
  
  //close the file
  fclose(fp);
  
  //create the clerk thread and pass in the clerks attributes and data structures
  //Clerk is joined to the customers, so only dies when they all die.
  if ((rc = pthread_create(&thrdArray[num - 1], NULL, clerk_function, cusArray))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
  }
	
	// block until all threads complete 
  int j;
  for (j = 0; j < num + 1; ++j) {
		pthread_join(thrdArray[j], NULL);
	}
  //sleep();
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