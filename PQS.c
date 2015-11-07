#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>  //intptr_t
#include <time.h>
#include <string.h>

//the number of customers
int num;
//convar used by customers to signal the clerk
pthread_cond_t clerkConvar = PTHREAD_COND_INITIALIZER;
//mutex used to protect the customer attributes array
pthread_mutex_t custArrayLock;
//mutex used to signal the clerk is a garbage mutex
pthread_mutex_t clerkLock;
//mutex for the customer time decrementation process
pthread_mutex_t decrementLock;
//mutex for recieving signals from clerk inside of customer
pthread_mutex_t custSigLock;

/* create thread argument struct for thr_func() */
typedef struct _customer{
	int id;
	int priority;
	int arrivalTime; 
  int serviceTime;
  pthread_cond_t customerConvar;
  int arrived;
  int running;
  int waiting;
  int finished; 
}customer;

/* customer function */
void *customer_function(void *ptr){
	customer * cPtr = (customer *)ptr;
  //wait until arrival time has passed
	//usleep(cPtr->arrivalTime*100000);
 sleep(5); 
  //lock the mutex on attribute reads and writes
//  pthread_mutex_lock(&custArrayLock);
//  //set the arrived state for this customer to 1
  //cPtr->arrived = 1;
//  //set the waiting state for this customer to 1
  //cPtr->waiting = 1;
//  //state that the customer has arrived
  printf("customer %2d arrives: arrival time (%d), service time (%d), priority (%2d)\n", cPtr->id, cPtr->arrivalTime, cPtr->serviceTime, cPtr->priority);
//  //unlock the customer array lock
//  pthread_mutex_unlock(&custArrayLock);
  //signal the clerk that this thread has arrived.
  pthread_cond_signal(&clerkConvar);
  //wait on this threads convar
  //pthread_mutex_lock(&custSigLock);
  pthread_cond_wait(&cPtr->customerConvar, &custSigLock);
//  //TEST: print out that we recieved a message from the clerk
//  printf("Thread recieved message from clerk to begin running");
//  //immediately unlock garbage clerkLock
//  pthread_mutex_unlock(&clerkLock);
  //wait for a signal from the clerk saying that its turn has come
  //run until signalled to stop by the clerk or run out of time
    //if signalled to stop, got to above step where we were waiting
    //if run out of time, signal clerk that I am done
  //thread kill with wait to make sure prints happen
  sleep(1);
  pthread_exit(NULL);
}

/* clerk function */
void *clerk_function(void* cA){
  customer * customerArray = (customer *)cA;
  //customerArray[i].id, 
  //customerArray[i].arrivalTime, 
  //customerArray[i].serviceTime, 
  //customerArray[i].priority);
  printf("hello");
  //int processesCompleted = 0;
  //loop until all customers are finished
  //while(processesCompleted < num){
   //waits (sleep, or convar, or mutex) until a customer signals arrival. Most likely a convar.
   //pthread_cond_wait(&clerkConvar, &clerkLock);
   printf("hello there");
   //A customer has arrived so now we need to check its properties
   //got through all the customer and check for the single one that has arrived
//   int signallingCustomerID = 0;
//   int runningCustomerID = 0;
//   int i;
//   for(i = 0; i < num; i++){
//     if(customerArray[i].running == 1){
//       runningCustomerID = customerArray[i].id;
//     }
//     if(customerArray[i].arrived == 1){
//       signallingCustomerID = customerArray[i].id;
//     }
//   }
//   printf("Here is the signalling id: %d", signallingCustomerID);
   //case 1: there is no running customer. Run the signalling customer

     //pthread_cond_signal(&customerArray[signallingCustomerID].customerConvar);
   
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
    //set customer running bool to false;
    //if another customer arrives, then go back to (A)
  
   //processesCompleted++;
  //}
  //thread kill with wait to make sure that prints happen
  sleep(1);
  pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
printf("main done");
	int i, rc;
 
 //variables needed to read in the customers.txt file
 char ch;
 char line[15];
 FILE *fp;

 //ERROR: too many arguments passed in
   if(argc != 2){
    perror("Error: Wrong number of arguments.\n");
    exit(EXIT_FAILURE);
   }  
    
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
	pthread_t thrdArray[num + 1]; //thread array with extra for the clerk (this is so we can get main to wait on all threads including clerk)
  
  //initialize the customer mutex
  if(pthread_mutex_init(&custArrayLock, NULL) != 0){
    printf("\nmutex init failed\n");
    return 1;
  }
  
  //initialize the clerk mutex
  if(pthread_mutex_init(&clerkLock, NULL) != 0){
    printf("\nmutex init failed\n");
    return 1;
  }
  
  //initialize the decrement mutex
  if(pthread_mutex_init(&decrementLock, NULL) != 0){
    printf("\nmutex init failed\n");
    return 1;
  }
  
  //initialize the decrement mutex
  if(pthread_mutex_init(&custSigLock, NULL) != 0){
    printf("\nmutex init failed\n");
    return 1;
  }
  
  //get each line of the input file and create a thread based on it.
  //initialize all of the threads with their properties 
  //lock the mutex during the init process
  //pthread_mutex_lock(&custArrayLock);
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
    cusArray[i].customerConvar = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    cusArray[i].arrived = 0;
    cusArray[i].running = 0;
    cusArray[i].waiting = 0;
    cusArray[i].finished = 0;
	  if ((rc = pthread_create(&thrdArray[i], NULL, customer_function, &cusArray[i]))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
	  }
  }

    //create the clerk thread and pass in the clerks attributes and data structures
  if ((rc = pthread_create(&thrdArray[num - 1], NULL, clerk_function, cusArray))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
  }
  
    //unlock the init mutex for custArray
	//pthread_mutex_unlock(&custArrayLock);
  //close the file
  fclose(fp);
  
	// block until all threads complete 
  int j;
  for (j = 0; j < num + 1; ++j) {
		pthread_join(thrdArray[j], NULL);
	}
 
  //sleep();
	return EXIT_SUCCESS;
}