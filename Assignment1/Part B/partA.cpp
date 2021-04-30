/*
    Code written by:
        Kartikay Goel           180101033
        Falak Chhikara     		180101023

    Assignment 1
    CS 544: Topics in Networks

    To run this program type the following the terminal:
        g++ partA.cpp
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
    double simulationTimeInput;
    double lamdaInput;
    double muInput;

    simulationTimeInput = 100000.0;
    lamdaInput=100.0;
    muInput=1000.0;

    cout << "Enter input( lamba and mu )\n";
    cout << "\nlambda (in workers/sec):";
    cin >> lamdaInput;
    cout << "\nmu (in workers/sec):";
    cin >> muInput;
    cout << "\n";
    if(lamdaInput > muInput){
        printf("Arrival rate is greater than Service rate. Please try another testcase\n\n");
        return 0;
    }


    double departureTime = 1 / muInput;  		// Mean service time
    double totalTime = simulationTimeInput;   	// Total time for Simulation
    double arrivalTime = 1 / lamdaInput;      	// Mean arrival time
    

    double time_first = 0.0;         			// Current time in simulation
    double time_second = 0.0;         			// Current time in simulation

    double nextarrivalTime_first = 0.0;        // Time for next arrival
    double nextarrivalTime_second = 0.0;       // Time for next arrival

    double nextdepartureTime_first = totalTime; // Time for next departure
    double nextdepartureTime_second = totalTime; // Time for next departure

    unsigned int n_first = 0;         	 // Actual number of Workers in the system
    unsigned int n_second = 0;         	 // Actual number of Workers in the system

    unsigned int departures_first = 0; 	// Total number of Workers served
    unsigned int departures_second = 0; // Total number of Workers served

    double busyTime_first = 0.0;       	// Total busy time
    double busyTime_second = 0.0;       // Total busy time


    double NoOfWorkers_first = 0.0;              	// Area of number of Workers*time in system
    double NoOfWorkers_second = 0.0;              	// Area of number of Workers*time in system

    double lastEventTime_first = time_first; 		// "last event time"
    double lastEventTime_second = time_second; 		// "last event time"

    double lastBusyTime_first;          	// "last start of busy time"
    double lastBusyTime_second;          	// "last start of busy time"

    double Throughput_first;     		// Throughput rate
    double Throughput_second;    	 	// Throughput rate

    double Utilization_first;     	// Utilization
    double Utilization_second;     	// Utilization

    double NoOfWorkers_checked_first;     	// Average number of Workers in system
    double NoOfWorkers_checked_second;     	// Average number of Workers in system

    double Response_Time_first;     	// Average Response time
    double Response_Time_second;     	// Average Response time

    double Waiting_Time_first = 0.0;   	//Average Waiting time
    double Waiting_Time_second = 0.0;   //Average Waiting time

    double WorkerInQueue_first = 0.0;           	// Average number of workers in queue
    double WorkerInQueue_second = 0.0;           	// Average number of workers in queue

    int i;
    // Simulation loop
    while (time_first < totalTime && time_second < totalTime)
    {
        // Arrival occurred
        
        if (nextarrivalTime_first < nextdepartureTime_first)
        {
            time_first = nextarrivalTime_first;
            NoOfWorkers_first = NoOfWorkers_first + n_first * (time_first - lastEventTime_first);  // Update area under "NoOfWorkers" curve
            n_first++;    // Workers in system increase
            lastEventTime_first = time_first;   // "last event time" for next event
            int prev = i;
            nextarrivalTime_first = time_first + expntl(arrivalTime);
            if (n_first == 1) // System is full, only have 1 server
            {
                lastBusyTime_first = time_first;    // Set "last start of busy time"
                nextdepartureTime_first = time_first + expntl(departureTime);
                Waiting_Time_first -= nextdepartureTime_first - time_first;
            }
        }
        // Departure occurred
        else
        {
            time_first = nextdepartureTime_first;
            NoOfWorkers_first = NoOfWorkers_first + n_first * (time_first - lastEventTime_first); // Update area under "NoOfWorkers" curve
            n_first--;    // Workers in system decrease
            lastEventTime_first = time_first;   // "last event time" for next event
            departures_first++;           // Increment number of completions
            if (n_first > 0)
            {
                nextdepartureTime_first = time_first + expntl(departureTime);
                Waiting_Time_first -= nextdepartureTime_first - time_first;
            }
            else
            {
                nextdepartureTime_first = totalTime;
                // Update busy time sum when no Workers
                busyTime_first = busyTime_first + time_first - lastBusyTime_first;
            }
        }

        // ------------

        if (nextarrivalTime_second < nextdepartureTime_second)
        {
            time_second = nextarrivalTime_second;
            NoOfWorkers_second = NoOfWorkers_second + n_second * (time_second - lastEventTime_second);  // Update area under "NoOfWorkers" curve
            n_second++;    // Workers in system increase
            lastEventTime_second = time_second;   // "last event time" for next event
            int prev = i;
            nextarrivalTime_second = time_second + expntl(arrivalTime);
            if (n_second == 1) // System is full, only have 1 server
            {
                lastBusyTime_second = time_second;    // Set "last start of busy time"
                nextdepartureTime_second = time_second + expntl(departureTime);
                Waiting_Time_second -= nextdepartureTime_second - time_second;
            }
        }
        // Departure occurred
        else
        {
            time_second = nextdepartureTime_second;
            NoOfWorkers_second = NoOfWorkers_second + n_second * (time_second - lastEventTime_second); // Update area under "NoOfWorkers" curve
            n_second--;    // Workers in system decrease
            lastEventTime_second = time_second;   // "last event time" for next event
            departures_second++;           // Increment number of completions
            if (n_second > 0)
            {
                nextdepartureTime_second = time_second + expntl(departureTime);
                Waiting_Time_second -= nextdepartureTime_second - time_second;
            }
            else
            {
                nextdepartureTime_second = totalTime;
                // Update busy time sum when no Workers
                busyTime_second = busyTime_second + time_second - lastBusyTime_second;
            }
        }


        
    }
    FILE * outputPartA= fopen("outputPartA.txt","w");
    if(outputPartA==NULL){
        cout<<"Error: File could not be created";
        return 0;
    }
    
     // Storing output in file
    fprintf(outputPartA,"                       Simulation Results                       \n");
    fprintf(outputPartA,"<-------------------------------------------------------------> \n");
    fprintf(outputPartA,"-    Provided INPUTS: \n");
    fprintf(outputPartA,"-    lambda   = %.4f workers/sec\n", lamdaInput);
    fprintf(outputPartA,"-    mu      = %.4f workers/sec\n\n", muInput);

    // Compute outputs
    
    Throughput_first = departures_first / time_first;  // Compute throughput rate
    Utilization_first = busyTime_first / time_first;    // Compute server utilization
    NoOfWorkers_checked_first = NoOfWorkers_first / time_first;           // Avg number of Workers in the system
    Response_Time_first = NoOfWorkers_checked_first / Throughput_first;              // Avg Response time
    Waiting_Time_first += NoOfWorkers_first;
    Waiting_Time_first /= time_first;
    Waiting_Time_first = Waiting_Time_first / Throughput_first;
    WorkerInQueue_first = NoOfWorkers_first - busyTime_first;
    WorkerInQueue_first = WorkerInQueue_first / time_first;
    
    // Storing output in file
    fprintf(outputPartA,"\n<-------------------------------------------------------------> \n");
    fprintf(outputPartA,"                Results for M/M/1 simulation for Queue %d         \n", i + 1);
    fprintf(outputPartA,"<-------------------------------------------------------------> \n");
    fprintf(outputPartA,"-  OUTPUTS: \n");
    fprintf(outputPartA,"-    Average number of workers getting checked                                 = %f workers \n", NoOfWorkers_checked_first);
    fprintf(outputPartA,"-    Average response time for workers in getting checked.                     = %f sec \n", Response_Time_first);
    fprintf(outputPartA,"-    Average time for which a worker has to wait until getting checked.        = %f sec \n", Waiting_Time_first);
    fprintf(outputPartA,"-    Average number of workers waiting in the queue before each officer.       = %f workers \n", WorkerInQueue_first);
    fprintf(outputPartA,"<-------------------------------------------------------------> \n");
    
    Throughput_second = departures_second / time_second;  // Compute throughput rate
    Utilization_second = busyTime_second / time_second;    // Compute server utilization
    NoOfWorkers_checked_second = NoOfWorkers_second / time_second;           // Avg number of Workers in the system
    Response_Time_second = NoOfWorkers_checked_second / Throughput_second;              // Avg Response time
    Waiting_Time_second += NoOfWorkers_second;
    Waiting_Time_second /= time_second;
    Waiting_Time_second = Waiting_Time_second / Throughput_second;
    WorkerInQueue_second = NoOfWorkers_second - busyTime_second;
    WorkerInQueue_second = WorkerInQueue_second / time_second;
    
        // Storing output in file
    fprintf(outputPartA,"\n<-------------------------------------------------------------> \n");
    fprintf(outputPartA,"                Results for M/M/1 simulation for Queue %d         \n", 2);
    fprintf(outputPartA,"<-------------------------------------------------------------> \n");
    fprintf(outputPartA,"-  OUTPUTS: \n");
    fprintf(outputPartA,"-    Average number of workers getting checked                                 = %f workers \n", NoOfWorkers_checked_second);
    fprintf(outputPartA,"-    Average response time for workers in getting checked.                     = %f sec \n", Response_Time_second);
    fprintf(outputPartA,"-    Average time for which a worker has to wait until getting checked.        = %f sec \n", Waiting_Time_second);
    fprintf(outputPartA,"-    Average number of workers waiting in the queue before each officer.       = %f workers \n", WorkerInQueue_second);
    fprintf(outputPartA,"<-------------------------------------------------------------> \n");


    fclose(outputPartA);
    return 0;
}

