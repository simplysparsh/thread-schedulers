/****************************************************************************
 * 
 * File             : scheduler.c
 * Description      : Declarations for functions in scheduler.c
 *
 * Author(s)        : Sparsh Saxena | Mahdi Chaker  
 * Last Modified    : October 11, 2016
*****************************************************************************/

// structure to hold thread info
typedef struct Node Node_t; // [thread_info] [thread_info_t]
struct Node {		// [thread_info]
    int tid;            // [thread_id] thread id
    int tprio;          // [thread_priority] thread priority
    float currentTime;  // [arrival_time] time that a thread arrives
    float waitTime		// [wait_time] time spent waiting
    int remainingTime;  // [remaining_time] time a thread needs in order to finish
    int numPreemptions;	// number of times a thread was preempted
    Node_t* link;       // [next] pointer to another Node

    pthread_cond_t my_turn;	// [turn_cond]
    
};

// Global variables to be initialized by init_scheduler and used by schedule_me
Node_t* ready				= NULL; 
int schedulerType			= 0;
int globalTime				= -1;
Node_t* linear_ready_queue	= NULL;
Node_t* lrq_tail			= NULL;
int scheduler_type			= 0;

//Global mutex locks
pthread_mutex_t scheduler_lock;

// provided functions
void init_scheduler( int sched_type );
int schedule_me( float currentTime, int tid, int remainingTime, int tprio );
int num_preemeptions( int tid );
float total_wait_time( int tid );

//Declaration for Scheduler Functions
void schedule_with_FCFS_or_PBS( Node_t* current_thread_node, int remainingTime );

//Declaration for helper methods
Node_t* insert_to_list(float currentTime, int tid, int remainingTime, int tprio);
void insert_per_fcfs(Node_t* new_node_address);
void insert_per_pbs(Node_t* new_node);

Node_t* search_list(int tid);
Node_t* create_new_thread_node(float currentTime, int tid, int remainingTime, int tprio);
void delete_first_node(Node_t* node_address);

// Added functions
Node_t* lrq_get( float cur_time, int t_id, int remain_time, int t_prio );
Node_t* lrq_insert( float cur_time, int t_id, int remain_time, int t_prio );
Node_t* lrq_delete( Node_t* thread_to_delete );
Node_t* lrq_search_id( int t_id );
Node_t* srtf_search( Node_t* lrq_to_search );