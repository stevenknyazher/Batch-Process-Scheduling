#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Table {
  int id;
  int arrival;
  int total_cpu;
  int total_remaining;
  int start_time;
  int end_time;
  int turnaround_time;
  int done;
  int already_started;
} *process = NULL;
typedef struct Table table_typedef;

int maximum_processes;

int minimum(int num1, int num2) {
  if (num1 > num2) {
    return num2;
  } else {
    return num1;
  }
}

void Output() {
  printf("ID\t Arrival \t Total \t Start \t End \tTurnaround");
  printf("\n----------------------------------------------------------\n");

  for (int i = 0; i <= maximum_processes; i++) {
    if (process[i].total_cpu > 0) {
      printf("%d\t", process[i].id);
      printf(" %d\t", process[i].arrival);
      printf("\t %d", process[i].total_cpu);
      if (process[i].done == 0) {
        printf("\n");
      } else {
        printf("\t %d", process[i].start_time);
        printf("\t %d", process[i].end_time);
        printf("\t%d\n", process[i].turnaround_time);
      }
    }
  }
  printf("\n");
  return;
}

void EnterParameters() {
  int id;
  int arrival;
  int total_cpu;
  int valid_process;

  do {
    printf("Enter total number of processes: ");
    scanf("%d", &maximum_processes);
    if (maximum_processes <= 0) {
      printf("\nNumber of processes must be positive\n");
    }
  } while (maximum_processes <= 0);

  process = (table_typedef*)malloc(maximum_processes * sizeof(process));
  if (process == NULL) {
    printf("\nNo memory is allocated\n");
    exit(0);
  }

  for (int i = 0; i <= maximum_processes; i++) {
    process[i].total_cpu = 0;
    process[i].id = i;
    process[i].arrival = 0;
    process[i].total_cpu = 0;
    process[i].total_remaining = 0;
    process[i].done = 0;
    process[i].start_time = 0;
    process[i].already_started = 0;
    process[i].end_time = 0;
    process[i].turnaround_time = 0;
  }

  for (int i = 0; i < maximum_processes; i++) {
    do {
      valid_process = 1;
      printf("Enter process id: ");
      scanf("%d", &id);
      if (id < 0) {
        printf("\nProcess id must be positive\n");
        valid_process = 0;
      }
    } while (!valid_process);
    process[i].id = id;
    do {
      printf("Enter arrival cycle for process %d: ", id);
      scanf("%d", &arrival);
      if (arrival < 0) {
        printf("\nArrival cycle must be positive\n");
      }
    } while (arrival < 0);
    process[i].arrival = arrival;
    do {
      printf("Enter total cycles for process %d: ", id);
      scanf("%d", &total_cpu);
      if (total_cpu <= 0)
        printf("\nTotal cycles must be positive\n");
    } while (total_cpu <= 0);
    process[i].total_cpu = total_cpu;
  }
  printf("\n");
  Output();
  return;
}

void ProcessSchedulingFIFO() {
  int lowestArrivalTime = 0;
  int currentProcessArrival = 0;
  int processScheduled = 0;
  int scanIndex = 0;
  int totalTimer = 0;
  int foundProcess = 0;

  for (int i = 0; i <= maximum_processes; i++) {
    process[i].done = 0;
  }
  while (processScheduled <= maximum_processes) {
    if (process[scanIndex].done == 0 && (process[scanIndex].arrival <= totalTimer)) {
      process[scanIndex].start_time = totalTimer;
      totalTimer += process[scanIndex].total_cpu;
      process[scanIndex].done = 1;
      process[scanIndex].end_time = totalTimer;
      process[scanIndex].turnaround_time = process[scanIndex].end_time - process[scanIndex].arrival;
      processScheduled++;
      scanIndex++;
      foundProcess = 0;
    } else {
      totalTimer++;
    }
  }
  Output();
  return;
}

void ProcessSchedulingSJF() {
  int lowestCycleTime = 0;
  int lowestCycleIndex = 0;
  int currentProcessArrival = 0;
  int processScheduled = 0;
  int scanIndex = 0;
  int totalTimer = 0;
  int foundProcess = 0;

  for (int i = 0; i <= maximum_processes; i++) {
    process[i].done = 0;
    process[i].total_remaining = process[i].total_cpu;
  }

  while (processScheduled < maximum_processes) {
    lowestCycleTime = INT_MAX;
    scanIndex = 0;
    while (!foundProcess) {
      for (; scanIndex <= maximum_processes; scanIndex++) {
        if (process[scanIndex].done == 0 && (process[scanIndex].arrival <= totalTimer) && (process[scanIndex].total_cpu > 0)) {
          currentProcessArrival = process[scanIndex].total_cpu;
          lowestCycleTime = minimum(lowestCycleTime, currentProcessArrival);
          if (lowestCycleTime >= currentProcessArrival) {
            lowestCycleTime = currentProcessArrival;
            lowestCycleIndex = scanIndex;
            foundProcess = 1;
          }
        }
      }
      if (!foundProcess) {
        totalTimer++;
        scanIndex = 0;
      }
    }
    process[lowestCycleIndex].start_time = totalTimer;
    totalTimer += process[lowestCycleIndex].total_cpu;
    process[lowestCycleIndex].total_remaining -= process[lowestCycleIndex].total_cpu;
    process[lowestCycleIndex].done = 1;
    process[lowestCycleIndex].end_time = totalTimer;
    process[lowestCycleIndex].turnaround_time = process[lowestCycleIndex].end_time - process[lowestCycleIndex].arrival;
    processScheduled++;
    foundProcess = 0;
  }
  Output();
  return;
}

void ProcessSchedulingSRT() {
  int lowestCycleTime = 0;
  int lowestCycleIndex = 0;
  int currentProcessArrival = 0;
  int processScheduled = 0;
  int scanIndex = 0;
  int totalTimer = 0;
  int tempIndex = -1;
  int foundProcess = 0;
  int first_run = 1;

  for (int i = 0; i <= maximum_processes; i++) {
    process[i].done = 0;
    process[i].total_remaining = process[i].total_cpu;
    process[i].already_started = 0;
  }
	
  while (processScheduled < maximum_processes) {
    lowestCycleTime = INT_MAX;
    scanIndex = 0;
    while (!foundProcess) {
      for (; scanIndex <= maximum_processes; scanIndex++) {
        if (process[scanIndex].done == 0 && (process[scanIndex].arrival <= totalTimer) && (process[scanIndex].total_cpu > 0)) {
          currentProcessArrival = process[scanIndex].total_remaining;
          lowestCycleTime = minimum(lowestCycleTime, currentProcessArrival);
          if (!first_run) {
            if (process[scanIndex].total_remaining == process[lowestCycleIndex].total_remaining) {
              if (process[scanIndex].arrival > process[lowestCycleIndex].arrival) {
                currentProcessArrival = INT_MAX;
              }
            }
          }
          if (lowestCycleTime >= currentProcessArrival) {
            lowestCycleTime = currentProcessArrival;
            lowestCycleIndex = scanIndex;
            foundProcess = 1;
          }
        }
        first_run = 0;
      }
      if (!foundProcess) {
        totalTimer++;
        scanIndex = 0;
      }
    }
    if (process[lowestCycleIndex].already_started == 0) {
      process[lowestCycleIndex].already_started = 1;
      process[lowestCycleIndex].start_time = totalTimer;
      process[lowestCycleIndex].total_remaining--;
    } else {
      process[lowestCycleIndex].total_remaining--;
    }
    totalTimer++;
    if (process[lowestCycleIndex].total_remaining == 0) {
      process[lowestCycleIndex].done = 1;
      process[lowestCycleIndex].end_time = totalTimer;
      process[lowestCycleIndex].turnaround_time = process[lowestCycleIndex].end_time - process[lowestCycleIndex].arrival;
      processScheduled++;
    }
    first_run = 1;
    foundProcess = 0;
  }
  Output();
  return;
}

void Quit() {
  if (process != NULL) {
    free(process);
  }
  return;
}

int main() {
  int i = 0;
  enum { PARAMETERS, FIFO, SJF, SRT, QUIT, INVALID } menuChoice;

  while (i != 5) {
    printf("Batch scheduling\n-------------------------------\n");
    printf("1) Enter parameters\n");
    printf("2) Schedule processes with FIFO alogirthm\n");
    printf("3) Schedule processes with SJF algorithm\n");
    printf("4) Schedule processes with SRT algorithm\n");
    printf("5) Quit and free memory\n");
    printf("\nEnter selection: ");
    scanf("%d", &i);
    if (i == 1) {
      menuChoice = PARAMETERS;
    } else if (i == 2) {
      menuChoice = FIFO;
    } else if (i == 3) {
      menuChoice = SJF;
    } else if (i == 4) {
      menuChoice = SRT;
    } else if (i == 5) {
      menuChoice = QUIT;
    } else {
      menuChoice = INVALID;
    }
		
    switch (menuChoice) {
      case PARAMETERS:
        EnterParameters();
        break;
      case FIFO:
        ProcessSchedulingFIFO();
        break;
      case SJF:
        ProcessSchedulingSJF();
        break;
      case SRT:
        ProcessSchedulingSRT();
        break;
      case QUIT:
        Quit();
        break;
      case INVALID:
        printf("Option doesn't exist\n");
    }
  }
  printf("Quitting program");
  return 1;
}
