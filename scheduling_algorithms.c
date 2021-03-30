#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct Process {
 int arrival_time;
 int service_time;
 int service_time_left;
 int waiting_time;
 int id;
};

struct Process createProcess(int,int);
void fifo(struct Process[], int size);
void sjf(struct Process[], int size);
void srt(struct Process[], int size);

int main(void) {
 struct Process p[5];
 p[0] = createProcess(0,3);
 p[1] = createProcess(2,6);
 p[2] = createProcess(4,4);
 p[3] = createProcess(6,5);
 p[4] = createProcess(8,2);

 printf("FIFO style\n");
 fifo(p,5);
 printf("SJF style\n");
 sjf(p,5);
 printf("SRT style\n");
 srt(p,5);
}

struct Process createProcess(int a_time, int s_time) {
 struct Process temp;
 temp.arrival_time = a_time;
 temp.service_time = s_time;
 temp.service_time_left = s_time;
 temp.waiting_time = 0;
 return temp;
}

void fifo(struct Process p[], int size) {
  int run_order[size];
  int current_time = 0;
  //itterating through the locations in the run_order
  for(int i = 0; i<size; i++){
    int last_arrival_time = 0;
    bool first = true;
    //itterating through the processes
    for (int j = 0; j<size; j++) {
      bool breakaway = false;
      //itterating through the existing positions in the run_order to see if the current process is already queued
      for (int k = 0; k < i; k++){
        //if the current process that's being checked is already queued
        if(run_order[k] == j) {
          breakaway = true;
          break;
        }
      }
      if(breakaway)
        continue;
      //if it's the first process that's given then it's automatically pushed to the queue
      if(first){
        run_order[i] = j;
        last_arrival_time = p[j].arrival_time;
        first = false;
      } else if (p[j].arrival_time < last_arrival_time) {
        run_order[i] = j;
        last_arrival_time = p[j].arrival_time;
      }
    }
  }
  current_time = p[run_order[0]].arrival_time;
  int total_turnaround_time = 0;
  for (int i = 0; i < size; i++){
    printf("Process %d will run at time %d until time",run_order[i],current_time);
    current_time = current_time + p[run_order[i]].service_time;
    printf(" %d. It will have a waiting time of %d.",current_time,p[run_order[i]].waiting_time);
    if(i != size - 1) {
      if (p[run_order[i+1]].arrival_time > current_time) {
        current_time = p[run_order[i+1]].arrival_time;
      } else{
        p[i+1].waiting_time  = current_time - p[run_order[i+1]].arrival_time;
      }
    }
    printf(" With a turnaround time of %d\n", p[run_order[i]].waiting_time + p[run_order[i]].service_time);
    total_turnaround_time += (p[run_order[i]].waiting_time + p[run_order[i]].service_time);
  }
  printf("Overall the algorithm has an ATT of %f.\n", (float)total_turnaround_time/(float)size);
} ////// end of FIFO //////

void sjf(struct Process p[], int size) {
  int run_order[size];
  int current_time = 0;
  //boolean to properly queue the first "instance"
  bool first_queued = true;
  //itterating through the locations in the run_order
  for(int i = 0; i<size; i++){
    int last_arrival_time = 0;
    int last_service_time = 0;
    //bool for first process to be compared to
    bool first = true;
    //bool for the case that there is no process that is ready to be processed at the current time
    bool no_proccess_ready = true;
    //itterating through the processes
    for (int j = 0; j<size; j++) {
      bool breakaway = false;
      //itterating through the existing positions in the run_order to see if the current process is already queued
      for (int k = 0; k < i; k++){
        //if the current process that's being checked is already queued
        if(run_order[k] == j) {
          breakaway = true;
          break;
        }
      }
      if(breakaway)
        continue;
      //if it's the first process that's given then it's automatically pushed to the queue
      if (first_queued) {
        if(first){
          run_order[i] = j;
          last_arrival_time = p[j].arrival_time;
          last_service_time = p[j].service_time;
          if (p[j].arrival_time == current_time) {
            no_proccess_ready = false;
          }
          first = false;
        } else if (p[j].arrival_time <= last_arrival_time) {
          if (p[j].arrival_time == last_arrival_time) {
            if (p[j].service_time < last_service_time) {
              run_order[i] = j;
              last_arrival_time = p[j].arrival_time;
              last_service_time = p[j].service_time;
            }
          } else {
            run_order[i] = j;
            last_arrival_time = p[j].arrival_time;
            last_service_time = p[j].service_time;
          }
          if (p[j].arrival_time == current_time) {
            no_proccess_ready = false;
          }
        }
      } else {
        if (first) {
          run_order[i] = j;
          last_arrival_time = p[j].arrival_time;
          last_service_time = p[j].service_time;
          first = false;
        } else {
          if (p[j].arrival_time <= last_arrival_time || p[j].arrival_time <= current_time) {
            if (p[j].service_time < last_service_time) {
              run_order[i] = j;
              last_arrival_time = p[j].arrival_time;
              last_service_time = p[j].service_time;
            }
          }
        }
        if (p[j].arrival_time <= current_time) {
          no_proccess_ready = false;
        }
      }
      if (no_proccess_ready == true) {
        current_time += p[i].arrival_time;
      }
    }
    current_time += p[i].service_time;
    first_queued = false;
  }
  current_time = p[run_order[0]].arrival_time;
  int total_turnaround_time = 0;
  for (int i = 0; i < size; i++){
    printf("Process %d will run at time %d until time",run_order[i],current_time);
    current_time = current_time + p[run_order[i]].service_time;
    printf(" %d. It will have a waiting time of %d.",current_time,p[run_order[i]].waiting_time);
    if(i != size - 1) {
      if (p[run_order[i+1]].arrival_time > current_time) {
        current_time = p[run_order[i+1]].arrival_time;
      } else{
        p[run_order[i+1]].waiting_time  = current_time - p[run_order[i+1]].arrival_time;
      }
    }
    printf(" With a turnaround time of %d\n", p[run_order[i]].waiting_time + p[run_order[i]].service_time);
    total_turnaround_time += (p[run_order[i]].waiting_time + p[run_order[i]].service_time);
  }
  printf("Overall the algorithm has an ATT of %f.\n", (float)total_turnaround_time/(float)size);
} ////// end of SJF //////

void srt(struct Process p[], int size) {
  int current_time = 0;
  struct Process current_process;
  int processes_done = 0;
  //boolean to properly queue the first "instance"
  bool first_queued = true;
  //bolean to indicate the 1st time unit has passed;
  bool first_run = true;
  for (int i = 0; i<size; i++) {
    p[i].id = i;
  }
  while (processes_done != size) {
    processes_done = 0;
    bool no_proccess_ready = true;
    int last_process_id;
    bool changed_process = false;
    //itterating through the prcoesses
    for (int i = 0; i<size; i++) {
      //bool for the case that there is no process that is ready to be processed at the current time
      if (p[i].service_time_left == 0) {
        ++processes_done;
        continue;
      }
      //if we just started the queuing
      if (first_queued) {
        current_process = p[i];
        last_process_id = p[i].id;
        first_queued = false;
      } else if (first_run) { //here we will check which process should be the first one to run
        if (current_process.arrival_time >= p[i].arrival_time) {
          if (current_process.arrival_time > p[i].arrival_time) {
            current_process = p[i];
            last_process_id = p[i].id;
          } else {
            if (current_process.service_time > p[i].service_time) {
              current_process = p[i];
              last_process_id = p[i].id;
            }
          }
        }
      } else { // this is where we will continuosuly swap which process gets to go
        if (p[i].arrival_time <= current_time) {
          if (p[i].id == current_process.id) {
            continue;
          } else if (current_process.service_time_left == 0) {
            last_process_id = current_process.id;
            current_process = p[i];
            changed_process = true;
          } else if (p[i].service_time_left < current_process.service_time_left) {
            last_process_id = current_process.id;
            p[current_process.id].waiting_time++;
            current_process = p[i];
            changed_process = true;
          } else if (p[i].service_time_left >= current_process.service_time_left) {
            p[i].waiting_time++;
          }
        } else if (current_process.service_time_left == 0) {
          last_process_id = current_process.id;
          current_process = p[i];
          changed_process = true;
        } else if (p[i].arrival_time < current_process.arrival_time) {
          current_process = p[i];
        }
      }
      if (current_time >= current_process.arrival_time && no_proccess_ready) {
        no_proccess_ready = false;
      }
    }
    if (first_run) {
      if (no_proccess_ready) {
        current_time += current_process.arrival_time;
      }
      printf("Process %d will run at time %d until time",current_process.id, current_time);
      first_run = false;
    }
    if (changed_process) {
      printf(" %d.\nProcess %d will run at time", current_time, current_process.id);
      if (no_proccess_ready) {
        current_time += current_process.arrival_time;
      }
      printf(" %d until time",current_time );
      changed_process = false;
    }
    --current_process.service_time_left;
    --p[current_process.id].service_time_left;
    ++current_time;
  }
  printf(" %d.\n",current_time);
  int total_turnaround_time = 0;
  for (int i = 0; i < size; i++) {
    printf("Process %d had a wainting time of %d and a turnaround time of %d\n",i, p[i].waiting_time, p[i].waiting_time + p[i].service_time);
    total_turnaround_time += (p[i].waiting_time + p[i].service_time);
  }
  printf("Overall the algorithm has an ATT of %f.\n", (float)total_turnaround_time/(float)size);
} ////// end of SRT //////
