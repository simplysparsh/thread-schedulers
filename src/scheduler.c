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
#include "scheduler.h"

void init_scheduler( int sched_type );
int schedule_me( float currentTime, int tid, int remainingTime, int tprio );
int num_preemeptions( int tid );
float total_wait_time( int tid );

#define FCFS    0
#define SRTF    1
#define PBS     2
#define MLFQ    3

typedef struct thread_info thread_info_t; 
struct thread_info {
    int thread_id;              // thread id
    int thread_priority;        // thread priority
    float arrival_time;         // time that a thread arrives
    float wait_time;            // time spent waiting
    int remaining_time;         // time a thread needs in order to finish
    thread_info_t* next;        // pointer to another Node

    pthread_cond_t turn_cond;   // condition variable which determines which thread turn
};

// Global variables to be initialized by init_scheduler and used by schedule_me
thread_info_t* linear_ready_queue   = NULL;
thread_info_t* lrq_tail             = NULL;
int scheduler_type;

// Added functions
thread_info_t* lrq_get( float cur_time, int t_id, int remain_time, int t_prio );
thread_info_t* lrq_insert( float cur_time, int t_id, int remain_time, int t_prio );
thread_info_t* lrq_delete( thread_info_t* thread_to_delete );
thread_info_t* lrq_search_id( int t_id );
thread_info_t* srtf_search( thread_info_t* lrq_to_search );

// Global mutex lock
pthread_mutex_t scheduler_lock;

void init_scheduler( int sched_type ) {
    scheduler_type  = sched_type;

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

    int next_scheduled_time;
    thread_info_t* current_thread_info  = NULL;
    thread_info_t* next_thread_info     = NULL;

    switch (scheduler_type) {
        case FCFS:
            //
            break;

        case SRTF:
            // get a pointer to the current thread info struct
            // whether or not it already exists in the ready queue
            current_thread_info = lrq_get( currentTime, tid, remainingTime, tprio );

            // if the current thread is not the lowest remaining time, wait
            while ( current_thread_info != srtf_search( linear_ready_queue ) ) {
                pthread_cond_wait( &(current_thread_info -> turn_cond), &scheduler_lock );
            }

            // when it is this thread's turn to run, schedule it to run at
            // the current time (it is this thread's turn)
            next_scheduled_time = currentTime;

            // if this thread is done running, tell the next thread to run
            if ( remainingTime == 0 ) {
                lrq_delete( current_thread_info );

                if ( linear_ready_queue != NULL ) {
                    next_thread_info    = srtf_search( linear_ready_queue );
                    pthread_cond_signal( &(next_thread_info -> turn_cond) );
                }
            }

            break;

        case PBS:
            //
            break;

        case MLFQ:
            //current_thread_info   = mlfq_insert();
            break;
    }

    pthread_mutex_unlock( &scheduler_lock );

    return next_scheduled_time;
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

//
// Utility functions
//

// linear ready queue: get or insert if not exist in list
thread_info_t* lrq_get( float cur_time, int t_id, int remain_time, int t_prio ) {
    thread_info_t* temp_thread_info = NULL;

    // try finding the current thread in the linear ready queue
    temp_thread_info    = lrq_search_id( t_id );

    // if the current thread exists in the queue, update it
    if ( temp_thread_info != NULL ) {
        temp_thread_info -> arrival_time    = cur_time;
        temp_thread_info -> remaining_time  = remain_time;
    } else {
    // else, current thread not queued yet, so add it
        temp_thread_info    = lrq_insert( cur_time, t_id, remain_time, t_prio );
    }

    return temp_thread_info;
}

// add a thread info struct to a linear ready queue
thread_info_t* lrq_insert( float cur_time, int t_id, int remain_time, int t_prio ) {
    thread_info_t* added_thread_info;

    // place a new thread info struct in memory and fill its values
    added_thread_info                       = malloc( sizeof( thread_info_t ) );
    added_thread_info -> thread_id          = t_id;
    added_thread_info -> arrival_time       = cur_time;
    added_thread_info -> remaining_time     = remain_time;
    added_thread_info -> thread_priority    = t_prio;
    added_thread_info -> next               = NULL;

    pthread_cond_init( &(added_thread_info -> turn_cond), NULL );

    // if there is nothing in the queue, add the first item there
    if ( linear_ready_queue == NULL ) {
        linear_ready_queue  = added_thread_info;
    } else {
        lrq_tail -> next    = added_thread_info;
    }

    lrq_tail                                = added_thread_info;

    return added_thread_info;
}

// delete a thread info struct from a linear ready queue
thread_info_t* lrq_delete( thread_info_t* thread_to_delete ) {
    thread_info_t* next_thread_info = NULL;
    thread_info_t* cursor           = NULL;

    // start at the beginning of the LRQ
    cursor  = linear_ready_queue;

    // look for the thread info struct PRECEDING the one we want to delete
    while ( (cursor != NULL) && ((cursor -> next) != thread_to_delete) ) {
        cursor  = (cursor -> next);
    }

    // as long as there is something to work with, skip over the thread info
    // struct to delete, then free that struct to delete
    if (cursor != NULL) {
        (cursor -> next)    = (thread_to_delete -> next);
        next_thread_info    = (cursor -> next);
        free( thread_to_delete );
    }

    return next_thread_info;
}

// search a linear ready queue for a specific thread id
thread_info_t* lrq_search_id( int t_id ) {
    thread_info_t* cursor   = NULL;

    // start at the head of the LRQ
    cursor  = linear_ready_queue;

    // search through the linear ready queue until the requested thread id is found
    // or, alternatively, return NULL, meaning the list was searched and a match
    // was not found
    while ( (cursor != NULL) && ((cursor -> thread_id) != t_id) ) {
        cursor  = (cursor -> next);
    }

    return cursor;
}

// search a linear ready queue according to SRTF tiebreaking rules
thread_info_t* srtf_search( thread_info_t* lrq_to_search ) {
    //compare pairs of nodes for tiebreak then continue
    thread_info_t* srt_thread_info  = NULL;
    thread_info_t* cursor           = NULL;

    // start at the head of the LRQ
    srt_thread_info = lrq_to_search;
    cursor          = lrq_to_search;

    // step through the list and select the thread with the shortest remaining time
    // or, in the case of a tie, the thread with the least currentTime
    while ( cursor != NULL ) {
        // first check: is the current shortest-remaining-time (SRT) thread still SRT?
        if ( (cursor -> remaining_time) < (srt_thread_info -> remaining_time) ) {
            srt_thread_info = cursor;
        // next check: do we have to resolve a tie?
        } else if ( (cursor -> remaining_time) == (srt_thread_info -> remaining_time) ) {
            if ( (cursor -> arrival_time) < (srt_thread_info -> arrival_time) ) {
                srt_thread_info = cursor;
            }
        }

        cursor  = (cursor -> next);
    }

    return srt_thread_info;
}