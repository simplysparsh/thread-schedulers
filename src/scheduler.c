/**
 * 
 * File             : scheduler.c
 * Description      : This is a stub to implement all your scheduling schemes
 *
 * Author(s)        : Sparsh Saxena | Mahdi Chaker  
 * Last Modified    : October 11, 2016
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

typedef struct Node Node_t; 
struct Node {
    int tid;
    int tprio;
    float currentTime;
    int remainingTime;

    //condition variable
    Node_t* link;

};

void init_scheduler( int sched_type );
int schedule_me( float currentTime, int tid, int remainingTime, int tprio );
int num_preemeptions(int tid);
float total_wait_time(int tid);

//Declaration for helper methods
void insert_to_list(int tid, int tprio, Node_t* head_addr);
Node_t* search_list(int tid, Node_t* head_addr);

Node_t* ready   = NULL; 

#define FCFS    0
#define SRTF    1
#define PBS     2
#define MLFQ    3

int schedulerType = 0;
pthread_mutex_t lock;

void init_scheduler( int sched_type ) {
    schedulerType = sched_type;
}

int schedule_me( float currentTime, int tid, int remainingTime, int tprio ) {
    int globalTime = 0;
    

    pthread_mutex_lock (&lock);

    /* Lock a mutex prior to updating the 
    ready list and unlock it upon updating. */
    
        insert_to_list(tid, tprio, ready);
    pthread_mutex_unlock (&lock);

    switch (schedulerType) {

        case FCFS:
            while(ready -> tid != tid);
            globalTime = ceil(currentTime); //rounds up to greater int value.
            break;

        case SRTF:
            // code
            break;

        case PBS:
            // code
            break;

        case MLFQ:
            // code
            break;
    }

    return globalTime;
}

int num_preemeptions(int tid){
/*

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


/* helper functions */

// Insert elements to the end of linked-list
void insert_to_list(int tid, int tprio, Node_t* head_addr) {
    Node_t* new_node_address = NULL;
    Node_t* current = head_addr;

    new_node_address = malloc(sizeof(Node_t));
    new_node_address -> tid = tid;
    new_node_address -> link = NULL;

    if (head_addr == NULL) {
      head_addr = new_node_address;
    }
    else {
      while(current -> link != NULL) {
       current = current -> link;
      }
      current -> link = new_node_address;
    }
}

// Search thread in the linked-list
Node_t* search_list(int tid, Node_t* head_addr) {
    Node_t* current = head_addr;

    while(current != NULL) {

        if (current -> tid == tid) {
            return current;
        } 
        else {
            current = current -> link;
        }
    }

    return NULL;
}
    


