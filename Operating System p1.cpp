#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

struct Process {
	int pid; //process id
	int AT ; //arrival time
	int BurstT;    //burst time
	int time_left; //for round robin
	int cs = 1;
	int wt; //waiting time
	int Ta_time; // turnaround time
	int f_time;  //finish time
};




int total_wt = 0, total_Ta_time = 0, total_burst = 0;
Process process[5];
int msize, psize, q, cs;
double avg_turnaround = 0, avg_waiting = 0;
int currentTime;
int n1;
double cpu_utilization = 0;

//function to read the data from file
void readFile() {

	ifstream in; 
	in.open("processes.txt");  
	if (!in.is_open()) {
		cout << "cannt find the file " << endl;   
		exit(EXIT_FAILURE);
	}
	//read file data 

	while (in.good()) {
		in >> n1;
		in >> msize;
		in >> psize;
		in >> q;
		in >> cs;
		for (int i = 0; i < n1; i++) {
			in >> process[i].pid;
			in >> process[i].AT ;
			in >> process[i].BurstT;

		}
	}
	in.close();

}
// gant chart for FCFS , SJF , RR
void GanttChart() {

	currentTime = 0;
	cout << "\n-------------------\n";
	cout << "Gantt Chart\n";
	cout << "-------------------\n";
	cout << "\n";
	for (int i = 0; i < n1; i++)
	{
		cout << "(" << currentTime << ")|==P" << process[i].pid << "==|";
		//to calculate current time after each process finished 
    currentTime += process[i].BurstT + cs; 	}
	currentTime = currentTime-cs;
	cout << "(" << currentTime << ")\n";

}



void findWaitingTime(Process proc[], int n,
	int wt[])
{
	// waiting time for first process is 0
	wt[0] = 0;

	// calculat waiting time
	for (int i = 1; i < n; i++)
		wt[i] = proc[i - 1].BurstT + wt[i - 1] + proc[1].cs;
}

// calculate turn around time
void findTurnAroundTime(Process proc[], int n,
	int wt[], int tat[])
{
	// calculat turnaround time 
	for (int i = 0; i < n; i++)
		tat[i] = proc[i].BurstT + wt[i] + proc[1].cs;
}

//Function to calculate average time
void findavgTime(Process proc[], int n)
{
	int wt[10], tat[10], total_wt = 0, total_tat = 0;

	// waiting time for all processes
	findWaitingTime(proc, n, wt);

	// turn around time for all processes
	findTurnAroundTime(proc, n, wt, tat);

	//Display processes with all info
	cout << "\nProcesses  " << " Burst time  "<< " Waiting time  " << " Turn around time\n";

	// total WT and total TAT

	double sum = 0;
	for (int i = 0; i < n1; i++)
	{
		total_wt = total_wt + wt[i];
		total_tat = total_tat + tat[i];
		sum = sum + proc[i].BurstT;
		cout << "   " << proc[i].pid << "\t\t"
			<< proc[i].BurstT << "\t    " << wt[i]
			<< "\t\t  " << tat[i] << endl;
	}

	cout << "\nAverage waiting time = "
		<< (float)total_wt / (float)n;
	cout << "\nAverage turn around time = "
		<< (float)total_tat / (float)n << endl;

	cout << "cpu utlization = " << (sum * 1.0 / (sum + 12)) * 100 << "%" << endl;
}


void sorty() {
	bool swapped;
	do { //Rearrange processes upwards depending on arrival time
		swapped = false;
		for (int i = 0; i < n1 - 1; i++) {
			if (process[i].AT  > process[i + 1].AT ) {
				swap(process[i], process[i + 1]);
				swapped = true;
			}
		}
	} while (swapped);
}

//function to calculate & display the Shared Optimization Criteria in SJF & FCFS
void getInfo() {
	GanttChart(); //call GanttChat function to display it
	total_burst = 0; total_wt = 0; total_Ta_time = 0; //reset totals
	process[0].f_time = process[0].BurstT;
	process[0].wt = 0;
	process[0].Ta_time = process[0].f_time - process[0].AT ;
	for (int i = 1; i < n1; i++) { //to calculate waiting time & turnaround time & finish time for each process 
		total_burst += process[i - 1].BurstT;
		process[i].f_time = process[i - 1].f_time + cs + process[i].BurstT;
		process[i].wt = process[i - 1].f_time + cs - process[i].AT ;
		process[i].Ta_time = process[i].f_time - process[i].AT ;
	}
	process[4].f_time -= cs;
	for (int i = 0; i < n1; i++) { //to calculate the totals
		total_wt += process[i].wt;
		total_Ta_time += process[i].Ta_time;
	}
	total_burst += process[n1 - 1].BurstT;
	cpu_utilization = float(total_burst * 100.0 / currentTime); //to calculate cpu utilization
															  //To display Information for each process in table
	cout << "Process\t|TurnAround Time|Waiting Time\t|finish Time\n";
	cout << "\n";
	for (int i = 0; i < n1; i++)
	{
		cout << "P(" << process[i].pid << ")\t|\t" << process[i].Ta_time << "\t|\t" << process[i].wt << "\t|\t" << process[i].f_time << "\n";

	}
	//t display the Averages
	cout << "Average Wating Time: =" << (double)total_wt / (n1 * 1.0) << endl;
	cout << "Average Turnaround  Time: =" << (double)total_Ta_time / (n1 * 1.0) << endl;
	cout << "Cpu Utilization " << cpu_utilization << " %" << endl;
	return;
}//function for FCFS
void FCFS() {
	cout << "First Come First is \n";
	sorty();	 //callsorty function to rearange processes
	getInfo();// call getInfo function to display Gantt Chart & other information  

}
//function for SJF (nonpreemptive)
void SJF() {
	cout << "SJF (nonpreemptive)\n";
	sorty();     //callsorty function to rearange processes
	int check = 0;      // check is counter to check if the process has arrived
	for (int i = 0; i < n1 - 1; i++)
	{
		check += process[i].BurstT;       //inserting arrival time of each process to be looped ..
		for (int k = i + 1; k < n1 - 1; k++)
		{
			if (check >= process[k + 1].AT  && process[k].BurstT > process[k + 1].BurstT)//sorting by burst time and
			{                                                               // if process arrived
				swap(process[k], process[k + 1]);
			}
		}
	}
	getInfo();
}
//function for Round Robin Scheduling with q time quantum 
void RR() {
	sorty();
	cout << "Round Robin\n";
	cout << " \n";
	int totalProcessTime, remain_processes, flag = 0;
	remain_processes = n1; //to decide how many processes remain (not finish)
	totalProcessTime = 0;
	int i;
	cout << "Gantt Chart\n";  //to display Gantt Chart 
	for (i = 0; i < n1; i++) {//to set each processes left time
		process[i].time_left = process[i].BurstT;
	}
	for (currentTime = 0, i = 0; remain_processes != 0;)
	{
		cout << "(" << currentTime << ")|==P" << process[i].pid << "==|";
		if (process[i].time_left <= q && process[i].time_left > 0)
		{// If burst time is smaller than or equal quantoum 
			currentTime += process[i].time_left + cs;
			process[i].time_left = 0;
			flag = 1;
		}
		else if (process[i].time_left > 0)
		{// If burst time is greater than quantoum
			process[i].time_left -= q;
			currentTime += q + cs;
		}
		if (process[i].time_left == 0 && flag == 1)
		{  //if we finish process 
			remain_processes--; //decremente number of processes
			process[i].f_time = currentTime - cs;
			process[i].Ta_time = currentTime - process[i].AT ; //to calculate turn around time
			process[i].wt = currentTime - process[i].AT ; //waiting time			

			flag = 0;
		}
		if (i == n1 - 1)
			i = 0; //if finish all processes
		else if (process[i + 1].AT  <= currentTime)
			i++;
		else
			i = 0;

	}
	total_wt = 0;
	total_Ta_time = 0;

	for (int i = 0; i < 5; i++) {
		process[i].Ta_time = process[i].wt + process[i].BurstT;
//to calculate Tptal WT 
	total_wt += process[i].wt;
//to claculate total TAT
		total_Ta_time += process[i].Ta_time; 

	}
	cout << "(" << currentTime << ")" << endl;
	cout << "Process\t|TurnAround Time| Waiting Time  |  finish Time\n";
	total_burst = 0;

	for (int i = 0; i < n1; i++)
	{
		//to set process turn around time
		cout << "P(" << process[i].pid << ")\t|\t" << process[i].Ta_time << "\t|\t" << process[i].wt << "\t|\t" << process[i].f_time << "\n";
		total_burst += process[i].BurstT;
	}
	cout << "--------------------------------------------------------\n\n";
	//to show Average times
	cout<<"\n";
	cout << "Avg Waiting time = " << (double)total_wt * 1.0 / n1 << endl;
	cout << "Avg Turnaround time = " << (double)total_Ta_time / n1 << endl;
	cpu_utilization = float(total_burst * 100.0 / (currentTime));
	cout << "Cpu Utilization " << cpu_utilization << " %\n";
}



int main() {
	cout << endl << endl;
	cout << "----- AYAT ABUALLAN 191060 -----" << endl << endl;

	readFile();
	cout << " your file information : " << endl;
	cout << "Process\t| Arrival Time  | Burst Time \n";
	for (int i = 0; i < n1; i++)
	{
		cout << "P(" << process[i].pid << ")\t|\t" << process[i].AT  << "\t|\t" << process[i].BurstT << "\n";
	}


	while (1) {
		cout << endl << endl << "   select choice  \n"<<" 1 - First Come First serves \n"<<
			" 2 - Shortest job first \n"<<" 3 - Round Robin \n";
		int choice;
		cin >> choice;
		switch (choice)
		{
		case 1:
			FCFS();
			break;
		case 2:
			SJF();
			break;

		case 3:
			RR();
			break;
		}
	}

	system("pause");
	return 0;
}
