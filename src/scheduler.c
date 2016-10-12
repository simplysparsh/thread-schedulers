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

#include "scheduler.h"

#define FCFS    0
#define SRTF    1
#define PBS     2
#define MLFQ    3

void init_scheduler( int sched_type ) {
    schedulerType = sched_type;

    // Initialize global mutex locks
    pthread_mutex_init(&scheduler_lock, NULL);

    switch (sched_type) {
        case FCFS:
            //
            break;

        case SRTF:
            linear_ready_queue  = NULL;
            break;

        case PBS:
            //
            break;

        case MLFQ:
            //
            break;
    }

    return;
}

int schedule_me( float currentTime, int tid, int remainingTime, int tprio ) {
    pthread_mutex_lock( &scheduler_lock );

    Node_t* current_thread_node = NULL;

    //Insert elements as per the kind of scheduler
    current_thread_node = insert_to_list(currentTime, tid, remainingTime, tprio);

    switch (schedulerType) {

        case FCFS:
            schedule_with_FCFS_or_PBS(current_thread_node, remainingTime);
            globalTime ++;
            break;

        case PBS:
            schedule_with_FCFS_or_PBS(current_thread_node, remainingTime);
            globalTime ++;
            break;

        case SRTF:
            int next_scheduled_time;
            Node_t* current_thread_info  = NULL;
            Node_t* next_thread_info     = NULL;

            // get a pointer to the current thread info struct
            // whether or not it already exists in the ready queue
            current_thread_info = lrq_get( currentTime, tid, remainingTime, tprio );

            // if the current thread is not the lowest remaining time, wait
            while ( current_thread_info != srtf_search( linear_ready_queue ) ) {
                pthread_cond_wait( &(current_thread_info -> my_turn), &scheduler_lock );
            }

            // if this thread is done running, tell the next thread to run
            if ( remainingTime == 0 ) {
                lrq_delete( current_thread_info );

                if ( linear_ready_queue != NULL ) {
                    next_thread_info    = srtf_search( linear_ready_queue );
                    pthread_cond_signal( &(next_thread_info -> my_turn) );
                }
            } else {
                next_scheduled_time = ++globalTime;
            }

            break;

        case MLFQ:
            // code
            break;
    }
    pthread_mutex_unlock (&scheduler_lock);
    return globalTime;
}

int num_preemeptions(int tid) {
/*
    Fill your code here
*/
    return -1;
}

float total_wait_time(int tid) {
/*
    Fill your code here
*/
    return -0.1;
}

/* Scheduler Functions */

void schedule_with_FCFS_or_PBS(Node_t* current_thread_node, int remainingTime ) {

    //If the current thread is not the first in queue, then block it.
    while(ready -> tid != current_thread_node -> tid) 
        pthread_cond_wait(&(current_thread_node->my_turn), &scheduler_lock);

    //Unlock the next thread if first one is complete.
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

    //Get the address of the thread if it already exists in the queue.
    old_thread_address = search_list(tid);

    //Update the thread value if it already exists in the queue.
    if (old_thread_address != NULL) {
        old_thread_address -> currentTime = currentTime;
        old_thread_address -> remainingTime = remainingTime;
        return old_thread_address;
    } else {
        //If it's a new thread, add it to the queue. 
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
    Node_t* curr = NULL;
    Node_t* prev = NULL;
    curr = ready;

    if(ready -> tprio > new_node -> tprio) {
        new_node -> link = ready;
        ready = new_node;
    } 
    else {

        while(curr != NULL && (curr -> tprio < new_node -> tprio)) {
            prev = curr;
            curr = curr -> link;
        }

        if(curr != NULL && (curr -> currentTime < new_node -> currentTime)) {

            while(curr -> link != NULL && curr -> tprio == (curr -> link) -> tprio) {
                if(new_node -> currentTime <= (curr -> link) -> currentTime)
                    break;
                curr = curr -> link;
            }
            new_node -> link = curr -> link;
            curr -> link = new_node;
        } 
        else {
            new_node -> link = curr;
            prev -> link = new_node;
        }
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

// Delete the first node in the linked-list
void delete_first_node(Node_t* node_address) {
    ready = ready -> link;
    free(node_address);
}

//
// linear ready queue: get or insert if not exist in list
Node_t* lrq_get( float cur_time, int t_id, int remain_time, int t_prio ) {
    Node_t* temp_thread_info = NULL;

    // try finding the current thread in the linear ready queue
    temp_thread_info    = lrq_search_id( t_id );

    // if the current thread exists in the queue, update it
    if ( temp_thread_info != NULL ) {
        temp_thread_info -> currentTime    = cur_time;
        temp_thread_info -> remainingTime  = remain_time;
    } else {
    // else, current thread not queued yet, so add it
        temp_thread_info    = lrq_insert( cur_time, t_id, remain_time, t_prio );
    }

    return temp_thread_info;
}

// add a thread info struct to a linear ready queue
Node_t* lrq_insert( float cur_time, int t_id, int remain_time, int t_prio ) {
    Node_t* added_thread_info;

    // place a new thread info struct in memory and fill its values
    added_thread_info                       = malloc( sizeof( Node_t ) );
    added_thread_info -> tid                = t_id;
    added_thread_info -> currentTime        = cur_time;
    added_thread_info -> remainingTime      = remain_time;
    added_thread_info -> tprio              = t_prio;
    added_thread_info -> link               = NULL;

    pthread_cond_init( &(added_thread_info -> my_turn), NULL );

    // if there is nothing in the queue, add the first item there
    if ( linear_ready_queue == NULL ) {
        linear_ready_queue  = added_thread_info;
    } else {
        lrq_tail -> link    = added_thread_info;
    }

    lrq_tail                                = added_thread_info;

    return added_thread_info;
}

// delete a thread info struct from a linear ready queue
Node_t* lrq_delete( Node_t* thread_to_delete ) {
    Node_t* next_thread_info = NULL;
    Node_t* cursor           = NULL;

    // start at the beginning of the LRQ
    cursor  = linear_ready_queue;

    // look for the thread info struct PRECEDING the one we want to delete
    while ( (cursor != NULL) && ((cursor -> link) != thread_to_delete) ) {
        cursor  = (cursor -> link);
    }

    // as long as there is something to work with, skip over the thread info
    // struct to delete, then free that struct to delete
    if (cursor != NULL) {
        (cursor -> link)    = (thread_to_delete -> link);
        next_thread_info    = (cursor -> link);
        free( thread_to_delete );
    }

    return next_thread_info;
}

// search a linear ready queue for a specific thread id
Node_t* lrq_search_id( int t_id ) {
    Node_t* cursor   = NULL;

    // start at the head of the LRQ
    cursor  = linear_ready_queue;

    // search through the linear ready queue until the requested thread id is found
    // or, alternatively, return NULL, meaning the list was searched and a match
    // was not found
    while ( (cursor != NULL) && ((cursor -> tid) != t_id) ) {
        cursor  = (cursor -> link);
    }

    return cursor;
}

// search a linear ready queue according to SRTF tiebreaking rules
Node_t* srtf_search( Node_t* lrq_to_search ) {
    //compare pairs of nodes for tiebreak then continue
    Node_t* srt_thread_info  = NULL;
    Node_t* cursor           = NULL;

    // start at the head of the LRQ
    srt_thread_info = lrq_to_search;
    cursor          = lrq_to_search;

    // step through the list and select the thread with the shortest remaining time
    // or, in the case of a tie, the thread with the least currentTime
    while ( cursor != NULL ) {
        // first check: is the current shortest-remaining-time (SRT) thread still SRT?
        if ( (cursor -> remainingTime) < (srt_thread_info -> remainingTime) ) {
            srt_thread_info = cursor;
        // next check: do we have to resolve a tie?
        } else if ( (cursor -> remainingTime) == (srt_thread_info -> remainingTime) ) {
            if ( (cursor -> currentTime) < (srt_thread_info -> currentTime) ) {
                srt_thread_info = cursor;
            }
        }

        cursor  = (cursor -> link);
    }

    return srt_thread_info;
}