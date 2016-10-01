/**
 * 
 * File             : scheduler.c
 * Description      : This is a stub to implement all your scheduling schemes
 *
 * Author(s)        : @author
 * Last Modified    : @date
*/

// Include Files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>

#include <math.h>
#include <pthread.h>

void init_scheduler( int sched_type );
int schedule_me( float currentTime, int tid, int remainingTime, int tprio );
int num_preemeptions(int tid);
float total_wait_time(int tid);

#define FCFS    0
#define SRTF    1
#define PBS     2
#define MLFQ    3

typedef struct Node Node_t; 

struct Node {
	
	int tid;
	Node_t* link;
};


void init_scheduler( int sched_type ) {

	
}

int schedule_me( float currentTime, int tid, int remainingTime, int tprio ) {
    int globalTime = 0;
    Node_t* ready   = NULL;
    

    return globalTime;
}

int num_preemeptions(int tid){
/**

	Fill your code here
*/

	return -1;
}

float total_wait_time(int tid){
/**

	Fill your code here
*/

  return -0.1;
}

