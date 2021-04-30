/*

    Code written by:
            Kartikay Goel           180101033
            Falak Chhikara     		180101023

    Assignment 1
    CS 544: Topics in Networks

    format of taking input -> ./a.out -top topology_file -conn connections_file -rt routingtablefile -ft forwardingtablefile -path pathsfile -flag hop|dist -p 0|1
    
    To run this program type the following the terminal:
        g++ Q1.cpp
        ./a.out -top top14.doc -conn NSFNET_100.doc -rt routing.txt -ft forwarding.txt -path path.txt -flag hop -p 0
*/
#include <bits/stdc++.h>

using namespace std;

int LABEL = 100;
class Node{
    public:
        int key,linkCost,label;
        double capacity,connSum;
        vector<vector<int>> paths;
        vector<int> paths_cost;

    void setKey(int key){
        this->key = key;
    }
    
    void setlinkCost(int linkCost){
        this->linkCost = linkCost;
    }
    
    void setlabel(int label){
        this->label = label;
    }

    void setcapacity(double capacity){
        this->capacity = capacity;
    }
    
    void setconnSum(double connSum){
        this->connSum = connSum;
    }
    
    void setpaths(vector<vector<int>>& paths){
        this->paths = paths;
    }

    void setpaths_cost(vector<int>& paths_cost){
        this->paths_cost = paths_cost;
    }

    // -----------

    int getkey( ){
        return this->key ;
    }
    
    int getlinkCost( ){
        return this->linkCost ;
    }
    
    int getlabel( ){
        return this->label ; 
    }

    double getcapacity( ){
        return this->capacity;
    }
    
    double getconnSum( ){
        return this->connSum ;
    }
    
    vector<vector<int>> getpaths(){
        return this->paths ;
    }

    vector<int> getpaths_cost(){
        return this->paths_cost ;
    }
    
};

class conn{
    public:
        int id,src,dest;
        // double b[3]; // b_min, b_avg, b_max
        double b_min, b_avg, b_max;

    void setid(int id){
        this->id = id;
    }
    void setsrc(int src){
        this->src = src;
    }
    void setdest(int dest){
        this->dest = dest;
    }
    void setb_min(double b_min){
        this->b_min = b_min;
    }
    void setb_avg(double b_avg){
        this->b_avg = b_avg;
    }
    void setb_max(double b_max){
        this->b_max = b_max;
    }
    // --

    int getid( ){
        return this->id;
    }
    int getsrc( ){
        return this->src;
    }
    int getdest( ){
        return this->dest ;
    }
    double getb_min( ){
        return this->b_min;
    }
    double getb_avg( ){
        return this->b_avg;
    }
    double getb_max(){
        return this->b_max;
    }
};

vector<pair<int,vector<int>>> label_list; // connection_id, label_list
vector<vector<Node>> adj;
vector<set<vector<int>>> fw; // for each node we have a set, whose each entry is a vector of 4 numbers
vector<conn> requests;
vector<pair<int,int>> successful_req; // connection_id, path_num(0 or 1)

void printVector(vector<int> v){
    for (auto i : v) {
        cout<<i<<" ";
    }
    cout<<endl;
}

bool isEqual(string a, string b)
{
	return a==b;
}
int readIntFromFile(ifstream *File){
    char str[32];
    *File >> str;
    int j=0;
    for(;!(str[j]>='0' && str[j]<='9');j++){
    }
    return atoi(str+j);
}
void read_topologyFile(string fname){
    
    ifstream File;
    File.open(fname.c_str());

    int N = readIntFromFile(&File);
    int E = readIntFromFile(&File);
    int i = 0;
    int j = 0;
    while(i<N){
        set<vector<int>> fw_temp;
        fw.push_back(fw_temp);
        vector<vector<int>> paths_temp;
        vector<int> paths_cost_temp;
        vector<Node> temp(N);
        j = 0;
        while(j<N)
        {
            temp[j].setKey(-1);
            temp[j].setlabel(-1);
            temp[j].setlinkCost(-1);
            temp[j].setcapacity(-1.0);
            temp[j].setconnSum(-1.0);
            temp[j].setpaths(paths_temp);
            temp[j].setpaths_cost(paths_cost_temp);
            j++;
        }
        i++;
        adj.push_back(temp);
    }

    i = 0;
    while(i<E){
        i++;
        int a= readIntFromFile(&File);
        int b= readIntFromFile(&File);
        int cost = readIntFromFile(&File);
        int cap = readIntFromFile(&File);

        adj[a][b].setKey(b);
        adj[a][b].setlinkCost(cost);
        adj[a][b].setcapacity(cap+0.0);
        adj[a][b].setlabel(LABEL);
        adj[b][a].setKey(a);
        adj[b][a].setcapacity(cap+0.0);
        adj[b][a].setlinkCost(cost);
        adj[b][a].setlabel(LABEL);
        LABEL++;
        adj[a][b].setconnSum(0.0);
        adj[b][a].setconnSum(0.0);
    }
    File.close();
}
void read_connectionFile(string fname){
    ifstream File;
    int R;
    File.open(fname.c_str());
    R =readIntFromFile(&File);
    int src,dest;
    int b_min, b_avg, b_max;
    int i = 0;
    while(i<R){
        src = readIntFromFile(&File);
        dest = readIntFromFile(&File);
        b_min = readIntFromFile(&File);
        b_avg = readIntFromFile(&File);
        b_max = readIntFromFile(&File);
        conn temp;
        temp.setsrc(src);
        temp.setdest(dest);
        temp.setid(i);
        temp.setb_min(b_min);
        temp.setb_avg(b_avg);
        temp.setb_max(b_max);
        requests.push_back(temp);
        i++;
    }
    File.close();
}


double calc_bequiv(int i)
{
    return min(requests[i].getb_max(), requests[i].getb_avg() + (requests[i].getb_max()-requests[i].getb_min())/4.0);
}

void update_forwading_table(int conn_id, int path_id){
    vector<int> labels; // label_list 
    int src = requests[conn_id].getsrc();
    int dest = requests[conn_id].getdest();
    vector<int> path = adj[src][dest].paths[path_id]; // path
    int prev;
    int curr = src;
    int nexxt = path[0];
    int j=0;
    while(j<path.size())
    {
    	prev = curr;
        curr = path[j];
        labels.push_back(adj[prev][curr].getlabel());
        j++;
    }
    curr = src;
    label_list.push_back({conn_id,labels});
    
    // keeping the interface id = node id
    fw[src].insert({-1,-1,path[0],adj[src][path[0]].getlabel()}); //  (blank,blank, interface_out, label_out)
    int i = 0;
    while(i<path.size()-1)
    {
        prev = curr;
        curr = path[i];
        int nq = path[i+1];
        nexxt = nq;
        fw[curr].insert({prev,adj[prev][curr].label,nexxt,adj[curr][nexxt].label});
        i++;
    }
    int indx = path.size()-2;
    fw[dest].insert({path[indx],adj[path[indx]][dest].label, -1,-1}); // (interface_in, label_in, blank,blank)
}

void process_connections(string optimistic){
	int n = requests.size();
    bool flag=false;
    if(isEqual(optimistic, "0")){
       flag = true;
    }
    int i = 0;
    for (auto request : requests){ // for each connection
        int src=request.src, dest = request.dest,b_equiv = request.getb_max(); // src, dest, b_equiv(for pessimistic)
        if(flag){ // if optimistic then b_equiv
            b_equiv = calc_bequiv(i);
        }
        int prev,curr=src;
        int k=0;
        while(k<adj[src][dest].paths.size()){ // for the paths available between src and dest
            vector<int> path = adj[src][dest].paths[k]; // path choosen now check criteria
            bool f=true;
            for(auto p : path){ // for every link on the path
                prev = curr;
                curr = p;
                // connSum = sum of all admitted connection through this link
                int tmp1 = adj[prev][curr].getcapacity();
                int tmp2 = adj[prev][curr].getconnSum();
                if(b_equiv >  tmp1 - tmp2){
                    f = false;
                    break;
                }
            }
            if(f){ // if all links satisfy the criteria
                curr=src;
                for(auto p : path){ // update connSum for each link
                    prev = curr;
                    curr = p;
                    adj[prev][curr].connSum += b_equiv;
                }
                successful_req.push_back({i,k}); // update successfull requests array
                update_forwading_table(i,k);
                break;
            }
            k++;
        }
        i++;
    }
}
int minDist(vector<int> dist, vector<bool> sptSet){
    int j=0;
    int n = dist.size();
    int minn = INT_MAX;
    int index;
    for(int i=0; i<dist.size(); i++){
        if(sptSet[j]==false && dist[i]<=minn){
            minn = dist[i];
            index = j;
        }
        j++;
    }
    return index;

}
void findPath(vector<int> parent, vector<int> &path, int j){
    if(parent[j]!=-1){
        findPath(parent, path, parent[j]);
    }
    else return;
    path.push_back(j);
}
void findPathforAll(vector<int> dist, vector<int> parent, int src){
    int n=dist.size();
    vector<int> path;
    int i = 0;
    for(int j=0; j<dist.size(); j++){
        if(i!=src)
        {
            path.clear();
            findPath(parent,path,i);
            adj[src][i].paths.push_back(path);
            adj[src][i].paths_cost.push_back(dist[j]);
        }
        i++;
    }
}
void dijkstra(int src, int dest, bool is_single_dest, bool is_hop){
    int n = adj.size();
    vector<int> dist;
    for(int i=0;i<n;i++)
    {
        dist.push_back(INT_MAX);
    }
    vector<bool> sptSet;
    for(int i=0;i<n;i++)
    {
        sptSet.push_back(false);
    }
    vector<int> parent;
    for(int i=0;i<n;i++)
    {
        parent.push_back(-1);
    }
    dist[src]=0;
    int i = 0;
    for(;i<n;i++){
        if(i==n-1){
            break;
        }
        int u = minDist(dist,sptSet);
        sptSet[u]=true;
        int v = 0;
        while(v<n){
            int cost = 1;
            if(is_hop == false){
                cost = adj[u][v].linkCost;
            }
            if(!sptSet[v]){
                if(adj[u][v].key!=-1)
                {
                    if(!(dist[u] >= dist[v] - cost)){
                        dist[v] = dist[u] + cost;
                        parent[v] = u;
                    }
                }
                
            }
            v++;
        }
    }
    vector<int> path;
    if(!(is_single_dest))
    {
        findPathforAll(dist,parent,src);
    }
    else
    {
    	path.clear();
        findPath(parent, path, dest);
        adj[src][dest].paths_cost.push_back(dist[dest]);
        adj[src][dest].paths.push_back(path);
    }
    
}
void findSecondShortestPath(int src, int dest, bool is_hop){
    vector<vector<int>> temp_paths = adj[src][dest].getpaths();
    vector<int> s_path = temp_paths[0];
    int prev = -1,curr=src;
    for(auto path: s_path){
        prev = curr;
        curr = path;
        adj[prev][curr].key = -1;
        adj[curr][prev].key = -1;
    }
    dijkstra(src,dest,true, is_hop);
    prev = -1;
    curr=src;

    int i=0;
    while(i<s_path.size())
    {
    	prev = curr;
        curr = s_path[i];
        adj[prev][curr].key = curr;
        adj[curr][prev].key = prev;
        i++;
    }        

}
void routing(bool is_hop){
    int n = adj.size();
    int i=0;
    int j=0;
    while(i<n){
        dijkstra(i, -1, false, is_hop);
        j=0;
        while(j<n){
            if(i==j){
                j++;
                continue;
            }
            findSecondShortestPath(i,j, is_hop);
            j++;

        }
        i++;
        
    }
}
void writeRoutingTaleFile(string routingTableFile){
    ofstream File;
    File.open(routingTableFile.c_str());

    int n=adj.size();
    int i = 0;
    for(auto adj_ : adj){
        File << "Src = " << i << endl;
        File << "------------\n";
        File << "Dest\tCost\tPath\n\n";
        int j = 0;
        while(j<n){
            if(i==j){
                j++;
                continue;
            }
            File << j;
            for(int k=0;k<adj_[j].paths.size();k++){
                File <<"\t\t"<<adj_[j].paths_cost[k]<<"\t\t"<<i;
                for(int i1=0;i1<adj_[j].paths[k].size();i1++){
                    File <<"-"<< adj_[j].paths[k][i1];
                }
                File << endl;
            }
            File << endl;
            j++;
        
        }
        File <<endl;
        i++;
    }
    File.close();

}
void writeForwadingTableFile(string forwadingTableFile){
    ofstream File;
    File.open(forwadingTableFile.c_str());
    int n=fw.size(), i = 0;

    for(auto f: fw){
        File<<"Node id : " << i<<endl;
        File << "---------------\n";
        File << "Interface_in\tLabel_in\t\tInterface_out\tLabel_out\n";
        for(auto itr = f.begin();itr!=f.end();itr++){
            vector<int> temp = *itr;
            if(temp[0]<0){
                File << "----\t\t\t";
            }
            else{
                File << temp[0] << "\t\t\t\t";
            }
            if(temp[1]<0){
                File << "----\t\t\t";
            }
            else
            {
                File << temp[1] << "\t\t\t\t";
            }
            if(temp[2]<0)
            {
                File << "----\t\t\t";
            }
            else
            {
                File << temp[2] << "\t\t\t\t";
            }
            if(!(temp[3]>=0))
            {
                File << "----\n";
            }
            else{
                File << temp[3]<<endl;
            }
        }
        File<<endl;
        i++;
    }
}

void writeOutputFile(string pathFile){
    ofstream File;
	File.open(pathFile.c_str());

    int n=successful_req.size();
    File <<"Connection_Id\tSource\tDestination\t\tLabel_List\t\t\t\t\t\tPath_Cost\n";
    int conn_id, src, dest,path_cost;
    vector<int> labels;
    int i = 0;
    for(auto successful_r: successful_req){
        conn_id = successful_r.first, src=requests[conn_id].src, dest = requests[conn_id].dest;
        if(label_list[i].first != conn_id){
            cout<<"Incoreect connection id\n";
            break;
        }
        labels = label_list[i].second;
        path_cost = adj[src][dest].paths_cost[successful_r.second];
        File << conn_id<<"\t\t\t\t";
        File << src<<"\t\t";
        File << dest<<"\t\t\t\t";
        int temp = 34;
        for(auto label: labels){
            string label_temp = to_string(label);
            File<<label<<" ";
            temp = temp - label_temp.size() - 1;
        }
        int j = 0;
        while(j<temp){
            File <<" ";
            j++;
        }
        File << path_cost<<endl;
        i++;
    }
    File.close();
}

int main(int argc, char *argv[])
{
    string topologyFile, connectionFile, routingTableFile, forwadingTableFile, pathFile, optimistic, flag;
    
    int i=0;
    while(i < argc-1)
    {
    	if(!strcmp(argv[i],"-top"))
    	{
            topologyFile.assign(argv[i+1]);
        }
        if(!strcmp(argv[i],"-conn"))
        {
            connectionFile.assign(argv[i+1]);
        }
        if(!strcmp(argv[i],"-rt"))
        {
            routingTableFile.assign(argv[i+1]);
        }
        if(!strcmp(argv[i],"-ft"))
        {
            forwadingTableFile.assign(argv[i+1]);
        }
        if(!strcmp(argv[i],"-path"))
        {
            pathFile.assign(argv[i+1]);
        }
        if(!strcmp(argv[i],"-p"))
        {
            optimistic.assign(argv[i+1]);  
        }
        if(!strcmp(argv[i],"-flag"))
        {
            flag.assign(argv[i+1]);  
        }

        i++;
    }

    bool is_hop = true;
    if(flag=="hop"){
        is_hop=false;
    }
    read_topologyFile(topologyFile);
    routing(!is_hop);
    writeRoutingTaleFile(routingTableFile);
    read_connectionFile(connectionFile);
    process_connections(optimistic);
    writeForwadingTableFile(forwadingTableFile);
    writeOutputFile(pathFile);

    cout<<"Total Connections requests: " << requests.size()<<endl;
    cout<<"Successful Connections: " << successful_req.size()<<endl;
    return 0;
}
