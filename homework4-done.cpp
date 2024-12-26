#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>
using namespace std;
void fcfs(vector<vector<int>> array_of_process,int num_process) {
    vector<int> finish_time(num_process);
    finish_time[0] =array_of_process[0][0] + array_of_process[0][1];
    cout<<"Gantt chart: "<<array_of_process[0][0]<<" process(0) "<<finish_time[0]<<" |" ;
	for( int i=1 ; i<num_process ; i++ ){
	 finish_time[i]=finish_time[i-1] + array_of_process[i][1];
	 cout<<finish_time[i-1]<<" process("<<i<<"):"<<finish_time[i]<<" |";
	}
	cout <<endl;
	for(int i=0;i<num_process;i++){
	    cout<<"finis time to process("<<i<<"):"<<finish_time[i]<<endl;
	}
	vector<int> TAT_time(num_process);
	TAT_time[0]=array_of_process[0][1]-array_of_process[0][0];
	for (int i=1;i<num_process;i++){
	TAT_time[i]=finish_time[i]-array_of_process[i][0];
	}
	for(int i=0;i<num_process;i++){
	cout<<"The TAT :"<<"process"<<"("<<i<<"):"<<TAT_time[i]<<endl;
	}
	vector<int> WT_time(num_process);
	WT_time[0]=TAT_time[0]-array_of_process[0][1];
	for (int i=0;i<num_process;i++){
	    WT_time[i]=TAT_time[i]-array_of_process[0][1];
	}
	for(int i=0;i<num_process;i++){
	    cout <<"Wating time process("<<i<<"):"<<WT_time[i]<<endl;
	}
	cout <<"Note : the Cs =0 , SO The CPU Utilization in FCFS =100%  "<<endl;
}
int SRT_FUNCTION(vector<vector<int>>& array_process, int current_time, int num_process) {
    //in the first we put the min_remaining_time as large as possiple to ba able to compare with athor processes and get the smallest one 
    int min_remaining_time = INT_MAX;
    // this Variable i will use it to get the index to smallest burst time process  
    int process_index = -1;

    // this loop will loop on all process 
    for (int i = 0; i < num_process; ++i) {
        // we check that the process its curruntly arrive (حتى اقارنها)and in the same time it should be the smallest 
        //remaining_burst time and the last logical and its for sure that the process doesn't end (هل بكون وقت التنفيذ مساوي لصفر او لأ )
        if (array_process[i][0] <= current_time && array_process[i][1] < min_remaining_time && array_process[i][1] > 0) {
            min_remaining_time = array_process[i][1];
            process_index = i;
        }
    }
    // سوف نقوم بارجاع رقم الخانة التي سوف تنطبق عليه الشروط السابقة (قبل عملية الloop)
    return process_index;
}

struct to_Each_Process {
    int number_P;              
    int arrival_time;     
    int burst_time;      
    int remain_time;  
    int finish_time;     
    int waiting_time;     
    int turnaround_time;};
void RR_Function(vector<vector<int>> &array_process, int quantum, int num_process){
        vector<to_Each_Process> array_of_process(num_process); 
        // divid and store the information to each process to calc. TAT and Wt and the finch time 
        for (int i = 0; i < num_process; i++) {
        array_of_process[i].number_P = i + 1;
        array_of_process[i].arrival_time = array_process[i][0];
        array_of_process[i].burst_time = array_process[i][1];
        array_of_process[i].remain_time = array_process[i][1]; 
        }

        queue<to_Each_Process*> process_line; 
        // will help me to know the time which is latter help me to decied wither the process will countio after the quantum end 
        int current_time = 0;
        // I put all the process in the quaue to save the order "as FIFo but we because we also care about the quantum so we use the queue"
        for (int i = 0; i < num_process; i++) {
        process_line.push(&array_of_process[i]);
       }
        while (!process_line.empty()) {
        to_Each_Process* current_P = process_line.front();
        
        process_line.pop();  
        // the first condition when the quantum is enough to the process burst time
        if (current_P->remain_time <= quantum) {
            current_time += current_P->remain_time;
            current_P->finish_time = current_time;
            current_P->remain_time = 0; 
        }
        else {
            current_P->remain_time -= quantum;
            current_time += quantum;
            // this very important to put the process another time in the queue because it does not finish yet 
            process_line.push(current_P); 
        }
        }
        for (int i = 0; i < num_process; i++) {
        array_of_process[i].turnaround_time = array_of_process[i].finish_time - array_of_process[i].arrival_time;
        array_of_process[i].waiting_time = array_of_process[i].turnaround_time - array_of_process[i].burst_time;
        }
        // print the Gantt chart 
        cout << "Gantt chart: ";
    for (const to_Each_Process &p : array_of_process) {
        if(p.number_P==0)
        cout<< p.arrival_time <<" process(" << p.number_P <<") "<< p.finish_time<<" | ";
       cout << p.finish_time << " process(" << p.number_P << ") " << p.burst_time << " | ";

    }
     for (const to_Each_Process &p : array_of_process) {
        cout<<endl;
        cout<<"process "<<p.number_P<<" finish time : "<<p.finish_time;
     }
     for (const to_Each_Process &p : array_of_process) {
        cout<<endl;
        cout<<"process "<<p.number_P<<" Waiting time: "<<p.waiting_time;
     }
     for (const to_Each_Process &p : array_of_process) {
        cout<<endl;
        cout<<"process "<<p.number_P<<" Turnaround time : "<<p.turnaround_time;
     }
}
+int main() {
    // first step was make the code able to take the data from the file 
    ifstream input("Inputfile.txt");
    // I do this to check that the file not empty.
    if (!input.is_open()) {
        cout << "The file is not open" << endl;
        return 1;
    }
    //determine the number of processes
    int num_process;
    input >> num_process; // to determine the number of processes
    // difine the 2 dimintion vector and then store both
    //(arriveial time "the first value" and burust time"the second value" )in this using the for loop 
    vector<vector<int>> array_process(num_process, vector<int>(2, 0));
    for (int i = 0; i < num_process; ++i) {
        for (int j = 0; j < 2; ++j) {
            input >> array_process[i][j]; // to take the information about the process and put it on the vector
        }
    }

    //these variable couled used in some "algorithm" cases.
    int current_time = 0;
    int idle_time = 0;
     num_process = array_process.size();
    vector<int> finish_time(num_process, -1);
    vector<int> waiting_time(num_process, 0);
    vector<int> turnaround_time(num_process, 0);
    vector<int> remaining_burst(num_process);

    // Copy burst times to a separate vector for manipulation
    for (int i = 0; i < num_process; ++i) {
        remaining_burst[i] = array_process[i][1];
    }
    ofstream out;
    out.open("SRT.txt");
    vector<int> gantt_chart;
    cout << "Enter one of the following numbers: 1 for FCFS, 2 for SRT, 3 for RR." << endl;
    int num_case;
    cin >> num_case;
    switch (num_case) {
    case 1:
    sort(array_process.begin(), array_process.end(),
    [](const vector<int>& a, const vector<int>& b) {
    return a[0] < b[0];   // Compare based on the first element in the vector
     });
    fcfs(array_process, num_process);
    break;
    case 2:{
    int idle_time = 0;
    int last_process_indexis = -1;
    float max_finish_time = -1;
    // in each unit time it will do one unit of the burust time and than it will copare with the second processe
	//if the second processe its burst_time is less then the current than it will print the remaining burst time in the file 
    // Calc. the idle_time for the first process
     idle_time = array_process[0][0] - 0;
    // the running for SRT and in this while loop we should update the unit of time 
    while (true) {
    // in this function we search about the smallest process to perform the Decreasing for its burst time unit 
    int current_process = SRT_FUNCTION(array_process, current_time, num_process);
    // this to check that the process end 
    if (current_process == -1) {
    break;
    }
    // the update to the burst time and put the process on the gantt_chart vector to save the order 
    //of the process execution.
    array_process[current_process][1]--; 
    gantt_chart.push_back(current_process); 
    // this will check that the process end and the finish_time is not send to the finish_time vector
    bool process_found=false;
   for(int i=0;i<num_process;i++){
   if (array_process[current_process][1] == 0 && finish_time[current_process] == -1) {
    finish_time[current_process] = current_time + 1;
     process_found = true;
    }
    }
    // this for check if there no process then will we have idel_time
     if (!process_found) {
                idle_time++; 
            }
    //update the variable that is resbonsabil about the time 
    current_time++;
    }
    // Calc. TAT time and Wt 
    for (int i = 0; i < num_process; ++i) {
    turnaround_time[i] = finish_time[i] - array_process[i][0];  
    waiting_time[i] = turnaround_time[i] - remaining_burst[i];
    // to determine the Process which end last one 
    for (int i = 0; i < num_process; ++i) {
    if (finish_time[i] > max_finish_time) {
        max_finish_time = finish_time[i];
        last_process_indexis = i;
    }
}

    } 
    //print the Gantt chart
    out << "Gantt Chart:";
    for (int time_slot : gantt_chart) {
    out << "Process"<< time_slot + 1  << " | ";
    }
    out << endl;
    //print the finish times.
    for (int i = 0; i < num_process; ++i) {
    out<<" finish time to Process( "<<i+1 <<")" << finish_time[i] <<endl;
    }
    //print the TAT time .
    for (int i = 0; i < num_process; ++i) {
    out << " TAT to Process("<<i+1<<")" << turnaround_time[i] <<endl;
    }
    //print the waiting time.
    for (int i = 0; i < num_process; ++i) {
    out << "WT to Process( "<<i+1<<")" << waiting_time[i] << endl;
    }
    // calc. the cpu Utilization
   float total_time_needed= max_finish_time ;
   float cpu_utilization =((total_time_needed - idle_time) / ( total_time_needed))*100;
    cout<<"The Cpu Utilization for SRT:"<<cpu_utilization<<"%"<<endl;
    out.close() ;
    break ;
    }
    case 3:
            // For Round Robin (RR) scheduling
            int quantum;
            sort(array_process.begin(), array_process.end(),
                [](const vector<int>& a, const vector<int>& b) {
                    return a[0] < b[0];   // Compare based on the first element in the vector
                });
            cout << "Enter the quantum: ";
            cin >> quantum;
           RR_Function(array_process, quantum, num_process);
            break;

        default:
            cout << "Invalid choice!" << endl;
            break;
    }

    input.close();
    return 0;
}