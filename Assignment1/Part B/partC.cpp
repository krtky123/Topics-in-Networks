/*
    Code written by:
        Kartikay Goel           180101033
        Falak Chhikara          180101023

    Assignment 1
    CS 544: Topics in Networks
    To run this program type the following the terminal:
        g++ partC.cpp
        ./a.out
 */
#include <bits/stdc++.h>

using namespace std;



// To generate exponentially distributed RV
double expntl(double mean)
{
    double z=0.0;
    // Get a RV (0 < z < 1)
    do{
        z = (double) rand();
        z/=RAND_MAX;
    }
    while ((z == 0) || (z == 1));

    double res = mean*log(z);
    return(-1.0*res);
}

int main(int argc, char **argv)
{
    double simulationTimeInput=100000.0;
    double lambdaInput=100.0;
    double muInput=1000.0;
    int capacity = 6; 		//No of chairs that each counter can have
    cout << "Enter input for simulating a testcase\n";
    cout << "\nlambda (in workers/sec):";
    cin >> lambdaInput;
    cout << "\nmu (in workers/sec):";
    cin >> muInput;
    cout << "\n";
    if(lambdaInput > muInput){
        printf("Arrival rate is greater than Service rate. Please try another testcase.\n\n");
        return 0;
    }

    double departureTime = 1 / muInput;  // Mean service time
    double totalTime = simulationTimeInput;        // Total time for Simulation
    double arrivalTime = 1 / lambdaInput;      // Mean arrival time
    
      
    double time1 = 0.0;        // Current time in simulation
    double time2 = 0.0;     // Current time in simulation
    double nextarrivalTime1 = 0.0;  // Time for next arrival
    double nextarrivalTime2 = 0.0;  // Time for next arrival
    double nextdepartureTime1 = totalTime;    // Time for next departure
    double nextdepartureTime2 = totalTime;    // Time for next departure

    unsigned int n1=0;  // Actual number of Workers in the system
    unsigned int n2=0;  // Actual number of Workers in the system       

    double departures1 = 0.0;   // Total number of Workers served
    double departures2 = 0.0;   // Total number of Workers served
   
    double busyTime1 = 0.0;  // Total busy time
    double busyTime2 = 0.0;    // Total busy time 
    double NoOfWorkers1 = 0.0;  // Area of number of Workers*time in system
    double NoOfWorkers2 = 0.0;  // Area of number of Workers*time in system
    double lastEventTime1 = time1;    // "last event time"
    double lastEventTime2 = time2;    // "last event time" 
    double lastBusyTime1 = 0.0; // "last start of busy time"
    double lastBusyTime2 = 0.0; // "last start of busy time"
    double Throughput1, Throughput2; // Throughput rate
    double Utilization1, Utilization2;      // Utilization  
    double NoOfWorkers_checked1, NoOfWorkers_checked2;      // Average number of Workers in system    
    double responseTime1, responseTime2; // Average Response time  
    double waitingTime1 = 0.0; //Average Waiting time
    double waitingTime2 = 0.0; //Average Waiting time        
    double WorkerInQueue1 = 0.0; // Average number of workers in queue
    double WorkerInQueue2 = 0.0; // Average number of workers in queue
    int i;
    // Simulation loop
    while (time1 < totalTime && time2 < totalTime)
    {
        // Arrival occurred      
        if (nextarrivalTime1 < nextdepartureTime1)
        {
            time1 = nextarrivalTime1;
            if(n1<capacity){
            NoOfWorkers1 = NoOfWorkers1 + n1 * (time1 - lastEventTime1);  // Update area under "NoOfWorkers" curve
            n1++;    // Workers in system increase
            
            if (n1 == 1) // System is full, only have 1 server
            {
                lastBusyTime1 = time1;    // Set "last start of busy time"
                nextdepartureTime1 = time1 + expntl(departureTime);
                waitingTime1 -= nextdepartureTime1 - time1;
            }

        	}
        	lastEventTime1 = time1;   // "last event time" for next event
            nextarrivalTime1 = time1 + expntl(arrivalTime);
        }
        // Departure occurred
        else
        {
            time1 = nextdepartureTime1;
            NoOfWorkers1 = NoOfWorkers1 + n1* (time1- lastEventTime1); // Update area under "NoOfWorkers" curve
            n1--;    // Workers in system decrease
            lastEventTime1 = time1;   // "last event time" for next event
            departures1++;           // Increment number of completions
            if (n1 > 0)
            {
                nextdepartureTime1 = time1 + expntl(departureTime);
                waitingTime1 -= nextdepartureTime1 - time1;
            }
            else
            {
                nextdepartureTime1 = totalTime;
                // Update busy time sum when no Workers
                busyTime1 = busyTime1 + time1 - lastBusyTime1;
            }
        }


        // Arrival occurred      
        if (nextarrivalTime2 < nextdepartureTime2)
        {
            time2 = nextarrivalTime2;
            if(n2<capacity){
            NoOfWorkers2 = NoOfWorkers2 + n2 * (time2 - lastEventTime2);  // Update area under "NoOfWorkers" curve
            n2++;    // Workers in system increase
            
            if (n2 == 1) // System is full, only have 2 server
            {
                lastBusyTime2 = time2;    // Set "last start of busy time"
                nextdepartureTime2 = time2 + expntl(departureTime);
                waitingTime2 -= nextdepartureTime2 - time2;
            }

            }
            lastEventTime2 = time2;   // "last event time" for next event
            nextarrivalTime2 = time2 + expntl(arrivalTime);
        }
        // Departure occurred
        else
        {
            time2 = nextdepartureTime2;
            NoOfWorkers2 = NoOfWorkers2 + n2* (time2- lastEventTime2); // Update area under "NoOfWorkers" curve
            n2--;    // Workers in system decrease
            lastEventTime2 = time2;   // "last event time" for next event
            departures2++;           // Increment number of completions
            if (n2 > 0)
            {
                nextdepartureTime2 = time2 + expntl(departureTime);
                waitingTime2 -= nextdepartureTime2 - time2;
            }
            else
            {
                nextdepartureTime2 = totalTime;
                // Update busy time sum when no Workers
                busyTime2 = busyTime2 + time2 - lastBusyTime2;
            }
        }
    }
    FILE * outputPartC= fopen("outputPartC.txt","w");
    if(outputPartC==NULL){
        cout<<"Error: File could not be created";
        return 0;
    }
    
     // Storing output in file
    fprintf(outputPartC,"<-------------------------------------------------------------> \n");
    fprintf(outputPartC,"                       Simulation Results                       \n");
    fprintf(outputPartC,"<-------------------------------------------------------------> \n");
    fprintf(outputPartC,"-    INPUTS: \n");
    fprintf(outputPartC,"-    lamba   = %.4f workers/sec\n", lambdaInput);
    fprintf(outputPartC,"-    mu      = %.4f workers/sec\n\n", muInput);
    // Compute outputs
    
    Throughput1 = departures1 / time1;  // Compute throughput rate
    Throughput2 = departures2 / time2;  // Compute throughput rate

    Utilization1 = busyTime1 / time1;    // Compute server utilization
    Utilization2 = busyTime2 / time2;    // Compute server utilization

    NoOfWorkers_checked1 = NoOfWorkers1 / time1;           // Avg number of Workers in the system
    NoOfWorkers_checked2 = NoOfWorkers2 / time2;           // Avg number of Workers in the system

    responseTime1 = NoOfWorkers_checked1 / Throughput1;              // Avg Response time
    responseTime2 = NoOfWorkers_checked2 / Throughput2;              // Avg Response time

    waitingTime1 += NoOfWorkers1;                  //Average Waiting time
    waitingTime1 /= time1;
    waitingTime1 = waitingTime1 / Throughput1;

    waitingTime2 += NoOfWorkers2;                  //Average Waiting time
    waitingTime2 /= time2;
    waitingTime2 = waitingTime2 / Throughput2;

    WorkerInQueue1 = NoOfWorkers1 - busyTime1;          // Average number of workers in queue
    WorkerInQueue1 = WorkerInQueue1 / time1;

    WorkerInQueue2 = NoOfWorkers2 - busyTime2;          // Average number of workers in queue
    WorkerInQueue2 = WorkerInQueue2 / time2;
    
     // Storing output in file
    fprintf(outputPartC,"                Results for M/M/1 simulation for Queue %d         \n", 1);
    fprintf(outputPartC,"<-------------------------------------------------------------> \n");
    fprintf(outputPartC,"-  OUTPUTS: \n");
    fprintf(outputPartC,"-    Average number of workers getting checked                                 = %f workers \n", NoOfWorkers_checked1);
    fprintf(outputPartC,"-    Average response time for workers in getting checked.                     = %f sec \n", responseTime1);
    fprintf(outputPartC,"-    Average time for which a worker has to wait until getting checked.        = %f sec \n", waitingTime1);
    fprintf(outputPartC,"-    Average number of workers waiting in the queue before each officer.       = %f workers \n", WorkerInQueue1);
    fprintf(outputPartC,"<-------------------------------------------------------------> \n");

    
     // Storing output in file
    fprintf(outputPartC,"                Results for M/M/1 simulation for Queue %d         \n", 2);
    fprintf(outputPartC,"<-------------------------------------------------------------> \n");
    fprintf(outputPartC,"-  OUTPUTS: \n");
    fprintf(outputPartC,"-    Average number of workers getting checked                                 = %f workers \n", NoOfWorkers_checked2);
    fprintf(outputPartC,"-    Average response time for workers in getting checked.                     = %f sec \n", responseTime2);
    fprintf(outputPartC,"-    Average time for which a worker has to wait until getting checked.        = %f sec \n", waitingTime2);
    fprintf(outputPartC,"-    Average number of workers waiting in the queue before each officer.       = %f workers \n", WorkerInQueue2);
    fprintf(outputPartC,"<-------------------------------------------------------------> \n");

    fclose(outputPartC);
    return 0;
}

