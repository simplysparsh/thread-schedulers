/****************************************************************************
 * 
 * File             : scheduler.c
 * Description      : This is a stub to implement all your scheduling schemes
 *
 * Author(s)        : Sparsh Saxena | Mahdi Chaker  
 * Last Modified    : October 11, 2016
*****************************************************************************/

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
    int tid;            // thread id
    int tprio;          // thread priority
    float currentTime;  // time that a thread arrives
    int remainingTime;  // time a thread needs in order to finish
    Node_t* link;       // pointer to another Node

    pthread_cond_t my_turn;
    
};

#include "scheduler.h"

#define FCFS    0
#define SRTF    1
#define PBS     2
#define MLFQ    3

Node_t* ready   = NULL; 
int schedulerType = 0;
int globalTime = -1;

//Global mutex locks
pthread_mutex_t scheduler_lock;

void init_scheduler( int sched_type ) {
    schedulerType = sched_type;

    // Initialize global mutex locks
    pthread_mutex_init(&scheduler_lock, NULL);
}

int schedule_me( float currentTime, int tid, int remainingTime, int tprio ) {
    pthread_mutex_lock (&scheduler_lock);

    Node_t* current_thread_node = NULL;
    current_thread_node = insert_to_list(currentTime, tid, remainingTime, tprio);

    switch (schedulerType) {

        case FCFS: //current_thread_node, remainingTime
            schedule_with_FCFS(current_thread_node, remainingTime);
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
    globalTime ++;
    pthread_mutex_unlock (&scheduler_lock);
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

/* Scheduler Functions */

void schedule_with_FCFS(Node_t* current_thread_node, int remainingTime ) {
    while(ready -> tid != current_thread_node -> tid) 
        pthread_cond_wait(&(current_thread_node->my_turn), &scheduler_lock);

    if (remainingTime == 0) {
        delete_first_node(current_thread_node);
        if(ready != NULL) 
            pthread_cond_signal(&(ready->my_turn));
        globalTime--;
    } 
}


/* helper functions */

// Check if element already present in list. If yes, update it and return address. 
// Else, insert element to the end of linked-list and return its address
Node_t* insert_to_list(float currentTime, int tid, int remainingTime, int tprio) {
    Node_t* current = NULL;
    Node_t* new_node_address = NULL;
    Node_t* old_thread_address = NULL;

    current = ready;
    old_thread_address = search_list(tid);

    if (old_thread_address != NULL) {
        old_thread_address -> currentTime = currentTime;
        old_thread_address -> remainingTime = remainingTime;
        return old_thread_address;
    } else {
        new_node_address = create_new_thread_node(currentTime, tid, remainingTime, tprio);

        if (ready == NULL) {
          ready = new_node_address;
        }
        else {
            switch (schedulerType) {
                case FCFS:
                  insert_per_fcfs(new_node_address);
                  break;
                case PBS:
                  insert_per_pbs(new_node_address);
                  break;  
            }
        }
        return new_node_address;
    } 
}

//Simply add node to the end of the list
void insert_per_fcfs(Node_t* new_node_address) {
    Node_t* current = NULL;
    current = ready;

    while(current -> link != NULL) {
        current = current -> link;
    }
    current -> link = new_node_address;
}

//Insert node such that they are always
//in order of priority or currentTime
void insert_per_pbs(Node_t* new_node) {
    Node_t* current = NULL;
    Node_t* previous = NULL;
    current = ready;

    while(current -> tprio < new_node -> tprio) {
        previous = current;
        current = current -> link;
    }

    if(current -> currentTime < new_node -> currentTime) {
        while(current -> tprio == (current -> link) -> tprio) {
            if(new_node -> currentTime <= (current -> link) -> currentTime)
                break;
            current = current -> link;
        }
        new_node -> link = current -> link;
        current -> link = new_node;
    } 
    else {
        new_node -> link = current;
        previous -> link = new_node;
    }
}

Node_t* create_new_thread_node(float currentTime, int tid, int remainingTime, int tprio) {
    Node_t* new_node_address = NULL;

    //Allocate and initialize
    new_node_address = malloc(sizeof(Node_t));
    new_node_address -> tid = tid;
    new_node_address -> currentTime = currentTime;
    new_node_address -> remainingTime = remainingTime;
    new_node_address -> tprio = tprio;
    new_node_address -> link = NULL;
    pthread_cond_init (&(new_node_address -> my_turn), NULL);

    return new_node_address;
}


// Search thread in the linked-list
Node_t* search_list(int tid) {
    Node_t* current = NULL;
    current = ready;

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

void delete_first_node(Node_t* node_address) {
    ready = ready -> link;
    free(node_address);
}
    


