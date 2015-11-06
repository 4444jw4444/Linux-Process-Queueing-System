# PQS
This is a Process Queueing System. 
In this system we simulate a clerk and their customers through the use of mutexs, convars, and threads. 

To run the program you must have a file containing customers with unique ids, arrival times, service times, and priorities
Once the file is read in, it is used to create a thread for every customer.
The clerk thread then performs scheduling on these customers in a highest priority first manner.
This means that if a low priority customer is being serviced and a high priority customer arrives, 
then the high priority customer gets serviced immediately and the low priority customer has to wait. 

To follow the progress of the clerk and customers, the program prints out event details as they occur.
At end of the program running, the the main thread will exit after all customers will have arrived and then been served. 
