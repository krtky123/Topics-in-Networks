/*
    Code written by:
        Falak Chhikara      180101023
        Kartikay Goel       180101033
    Assignment 1
    CS 544: Topics in Networks
    To run this program type the following the terminal:
        g++ partB.cpp
        ./a.out
 */

#include <bits/stdc++.h>
#include<algorithm>



using namespace std;

typedef class node {
  public:
    double time;
    char event_type;
    node *next;

  void settime(double time){
    this->time = time;
  }

  void setevent_type(char event_type){
    this->event_type = event_type;
  }

  void setnext(node *next){
    this->next = next;
  }

  // ---

  double gettime(){
    return this->time ;
  }

  char getevent_type(){
    return this->event_type ;
  }

  node* getnext(){
    return this->next ;
  }

} EventNode;

double SIMULATION_TIME, ARRIVAL_TIME, SERVICE_TIME;  // Time for which simulation is run  // Mean workers arrival time  // Mean workers service time
int SERVERS;                           // Total number of servers

/* Function used for generating Random Variables that follows exponential distribution with a mean  rate given as argument by using inverse method */
double generate_expRV(double mean)
{
    double z = ((double) rand() / RAND_MAX);        // Uniform random number from 0 to 1

    // Generating a uniform Random Variable (0 < z < 1)
    while ((z == 0) || (z == 1)){
      z = ((double) rand() / RAND_MAX);
    };

    // Return exponentially generated Random Variable
    return(-mean * log(z));
}

/* Creating a new Event Node and allocating memory to it */
EventNode* NewNode(char event,double time){
    EventNode * newnode = NULL;
    newnode = new EventNode();
    newnode->settime(time);
    newnode->setevent_type(event);
    newnode->setnext(NULL);
    return newnode;
}
/*   */

void InsertEvent(EventNode ** head,char event,double time)
{
  EventNode* CurrentNode, *newEventNode;
  CurrentNode = *head;
  newEventNode = NULL;
  if(CurrentNode == NULL) {
    newEventNode=NewNode(event,time);
    *head = newEventNode;
    return;
  }
  for(;CurrentNode != NULL;) {
      /* Event that need to be added before the first event */
      newEventNode = NewNode(event,time);
      if(CurrentNode->gettime() > time) {
        if(CurrentNode == *head){
          
          newEventNode->setnext(CurrentNode);
          *head = newEventNode;
          return;
        }
      }
      else{
      /* Event that need to be added after the last event */
        newEventNode->setnext(CurrentNode->getnext());
        if(CurrentNode->gettime() <= time) {
          if(CurrentNode->getnext() == NULL){
            CurrentNode->setnext(newEventNode);
            return;
          }
        }
        /* Event that need to be added in between two other events */
        if(CurrentNode->gettime() < time) {
          if(CurrentNode->getnext()->gettime()>= time){
            CurrentNode->setnext(newEventNode);
            return;
          }
        }
      }
      // Iterate to next event
      CurrentNode = CurrentNode->getnext();
  } 
  
}

bool freeMemort(EventNode** head){
  try{
    free(*head);
    return true;
  }catch(...)
  {
    return false;
  }
}

/* Function used for removing element from the front of the queue */
void RemoveEvent(EventNode** head){
  if (*head == NULL) {
    // Error: Empty queue, header pointer is null
    return ;
  }else{
    EventNode* NextNode = (*head)->getnext();
    bool flag = freeMemort(head);              //Deallocate Memory
    if(flag==false){
      cout<<"Error in deallocation";
      return;
    }
    // Update header pointer
    *head = NextNode;
    return;
  }
  
}


void RemoveBufferEvent(EventNode** head,EventNode** buffer_head,double CurrentTime,double* WaitingTime,double* ResponseTime,double* BusyTime){
  if (*buffer_head == NULL) {
    // Error: Empty Buffer, buffer header pointer is null
        return ;
  }
  EventNode* NextNode =(*buffer_head)->getnext();
  // Create and insert popped event as a departure event in service queue
  double ServTime = generate_expRV(SERVICE_TIME);
  double netTime = CurrentTime;
  netTime += ServTime;
  double waiting;
  double buffer_head_val = (*buffer_head)->gettime();
  if(CurrentTime-buffer_head_val>=0.0){
    waiting = CurrentTime-buffer_head_val;
  }else{
    waiting = 0.0;
  }
  *WaitingTime = *WaitingTime + waiting;
  *ResponseTime = (*ResponseTime + waiting );
  *ResponseTime = *ResponseTime + ServTime;
  *BusyTime = *BusyTime + ServTime;
  // Adding to queue
  InsertEvent((head),'D',netTime);
  free(*buffer_head);
  *buffer_head = NextNode;
}

int server,DeparturesCompleted=0;                    
double PresentTime,netTime,BusyTime,ServTime,WaitingTime,ResponseTime,sim_time_input,lamda_input,meu_input;
double SIMTIME = 1000000.0;         // General Simulation time
int LAMBDA = 100;            // General Arrival Rate
int MEU = 200;         // General Service Rate

EventNode* head;                           // Header pointer for servicing queue
EventNode* buffer_head;                    // Header pointer for buffer queue


int main( int argc, char *argv[])
{

    server=DeparturesCompleted=0;                 // Server Line Active           // Total no. of completed departures
    PresentTime=netTime=BusyTime=ServTime=WaitingTime=ResponseTime=0.0; // Current time in simulation // Departure time for event
    // Time for which server is busy
    // Service time generated by exponential distribution
    // Average Waiting Time
    // Average Response Time
    head = buffer_head = NULL;                           // Header pointer for servicing queue
     // Header pointer for buffer queue

/* ---------------------------------------Simulation Begins-------------------------------------------     */
   // Since there are two officers that are checking workers
    SERVERS = 2;
    sim_time_input = SIMTIME;
    lamda_input = LAMBDA;
    meu_input = MEU;
    cout << "Enter input for simulating a testcase\n";
    cout << "Simulation Time :";
    cin >> sim_time_input;
    cout << "lambda_2 :";
    cin >> lamda_input;
    cout << "mue_2 :";
    cin >> meu_input;
    ARRIVAL_TIME=1;
    ARRIVAL_TIME /= lamda_input;

    SERVICE_TIME=1;
    SERVICE_TIME /= meu_input;
    SIMULATION_TIME=sim_time_input;

   double PresentTimeTemp = PresentTime + generate_expRV(ARRIVAL_TIME);
   InsertEvent(&head,'A',PresentTimeTemp);
    while(1) {
      char curr_event=head->getevent_type();
      if(curr_event=='A')
      {
        PresentTime = head->gettime();
        RemoveEvent(&head);
        if(server>=SERVERS){
          // if servers are busy then add the arrivals to the buffer queue
          PresentTimeTemp = PresentTime;
          InsertEvent(&buffer_head,'A',PresentTimeTemp);
          // Keep creating arrivals until the present time exceeeds the simulation time
          if(PresentTime < SIMULATION_TIME){
            PresentTime += generate_expRV(ARRIVAL_TIME);
            InsertEvent(&head,'A',PresentTime);
          }
        }
        else{
          
          // Generate Departure event for current arrival
          ServTime = generate_expRV(SERVICE_TIME);
          BusyTime = BusyTime + ServTime;
          ResponseTime = ResponseTime + ServTime;
          netTime = PresentTime+ServTime;
          InsertEvent(&head,'D',netTime);
          server++; // Make the server busy
          // Keep creating arrivals until the present time exceeeds the simulation time
          if(PresentTime < SIMULATION_TIME){
            double temp = PresentTime + generate_expRV(ARRIVAL_TIME);
            InsertEvent(&head,'A',temp);
          }
        }

        
      }
      else if(curr_event=='D')
      {
        DeparturesCompleted = DeparturesCompleted + 1;
        PresentTime = head->gettime();
        RemoveEvent(&head);
        if(SIMULATION_TIME < PresentTime){
          break;
        }
        bool flag = true;
        if(PresentTime <= SIMULATION_TIME){
          if(buffer_head!=NULL){                 // Handle buffered events
            flag = false;
            RemoveBufferEvent(&head,&buffer_head,PresentTime,&WaitingTime,&ResponseTime,&BusyTime);
          }
        }
        if(PresentTime <= SIMULATION_TIME){
          if(flag){
            if(server>=1){
              server--;
            }else{
              server = 0;
            }
                // server=max(0,server-1);         // Servers becomes idle
          }
        }
      }

    }
    // Output results asked in question
    FILE * outputPartB= fopen("outputPartB.txt","w");
    if(outputPartB==NULL){
        cout<<"Error: File could not be created";
        return 0;
    }
/* Stability Condition */
  if(lamda_input < meu_input){
    // Storing output in file
    fprintf(outputPartB,"----------------------------Results from M/M/2 simulation----------------------------\n\n");
    fprintf(outputPartB," Following are the inputs provided :\n");
    fprintf(outputPartB," Total Simulation Time               = %3.4f sec\n",PresentTime);
    fprintf(outputPartB," Arrival Rate (LAMBDA)               = %lf sec\n",lamda_input);
    fprintf(outputPartB," Service Rate (MEU)                  = %lf sec\n",meu_input);
    fprintf(outputPartB,"-------------------------------------------------------------------------------------\n\n");
    fprintf(outputPartB," Following are the output generated :\n");
    fprintf(outputPartB," Total Number of workers checked                                       = %u workers\n\n",DeparturesCompleted);
    fprintf(outputPartB," Average number of workers getting checked                             = %lf workers\n", ResponseTime/PresentTime);
    fprintf(outputPartB," Average response time for workers in getting checked                  = %lf sec\n", ResponseTime/DeparturesCompleted);
    fprintf(outputPartB," Average time for which a worker has to wait until getting checked     = %lf sec\n", WaitingTime/DeparturesCompleted);
    fprintf(outputPartB," Average number of workers waiting in the queue before each officer    = %lf workers\n", (ResponseTime-BusyTime)/PresentTime);
    fprintf(outputPartB,"-------------------------------------------------------------------------------------\n\n");

}
else{
    printf("UNSTABLE: Arrival Rate is greater than Service Rate, Please try other test case\n");
}
    fclose(outputPartB);

    return 0;
}


