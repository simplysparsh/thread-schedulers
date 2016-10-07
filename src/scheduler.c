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
    int tid;            // thread id
    int tprio;          // thread priority
    float currentTime;  // time that a thread arrives
    int remainingTime;  // time a thread needs in order to finish
    Node_t* link;       // pointer to another Node

    pthread_cond_t my_turn;
    
};

void init_scheduler( int sched_type );
int schedule_me( float currentTime, int tid, int remainingTime, int tprio );
int num_preemeptions(int tid);
float total_wait_time(int tid);

//Declaration for helper methods
Node_t* insert_to_list(float currentTime, int tid, int remainingTime, int tprio, Node_t* head_addr);
Node_t* search_list(int tid, Node_t* head_addr);
Node_t* create_new_thread_node(float currentTime, int tid, int remainingTime, int tprio);
void delete_first_node(Node_t* node_address);

#define FCFS    0
#define SRTF    1
#define PBS     2
#define MLFQ    3

Node_t* ready   = NULL; 
int schedulerType = 0;
int globalTime = 0;

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
    current_thread_node = insert_to_list(currentTime, tid, remainingTime, tprio, ready);

    switch (schedulerType) {

        case FCFS:
            while(ready -> tid != tid) 
                pthread_cond_wait(&(current_thread_node->my_turn), &scheduler_lock);

            if (remainingTime != 0) {
                globalTime = ceil(currentTime); //rounds up to greater int value.
            } 
            else {
                delete_first_node(current_thread_node, ready);
                pthread_cond_signal(&(ready->my_turn));
            }
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


/* helper functions */

// Insert element to the end of linked-list and return its address
Node_t* insert_to_list(float currentTime, int tid, int remainingTime, int tprio, Node_t* head_addr) {
    Node_t* current = NULL;
    Node_t* new_node_address = NULL;
    Node_t* old_thread_address = NULL;

    Node_t* current = head_addr;
    Node_t* old_thread_address = search_list(tid, head_addr);

    if (old_thread_address != NULL) {
        old_thread_address -> currentTime = currentTime;
        old_thread_address -> remainingTime = remainingTime;
        return old_thread_address;
    } else {
        new_node_address = create_new_thread_node(currentTime, tid, remainingTime, tprio);

        if (head_addr == NULL) {
          head_addr = new_node_address;
        }
        else {
          while(current -> link != NULL) {
           current = current -> link;
          }
          current -> link = new_node_address;
        }
        return new_node_address;
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
Node_t* search_list(int tid, Node_t* head_addr) {
    Node_t* current = NULL;
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

void delete_first_node(Node_t* node_address) {
    ready = ready -> link;
    free(node_address);
}
    


