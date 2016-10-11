/****************************************************************************
 * 
 * File             : scheduler.c
 * Description      : Declarations for functions in scheduler.c
 *
 * Author(s)        : Sparsh Saxena | Mahdi Chaker  
 * Last Modified    : October 11, 2016
*****************************************************************************/

// void init_scheduler( int sched_type );
// int schedule_me( float currentTime, int tid, int remainingTime, int tprio );
// int num_preemeptions(int tid);
// float total_wait_time(int tid);

//Declaration for Scheduler Functions
//void schedule_with_FCFS_or_PBS(Node_t* current_thread_node, int remainingTime);

//Declaration for helper methods
// Node_t* insert_to_list(float currentTime, int tid, int remainingTime, int tprio);
// void insert_per_fcfs(Node_t* new_node_address);
// void insert_per_pbs(Node_t* new_node);

// Node_t* search_list(int tid);
// Node_t* create_new_thread_node(float currentTime, int tid, int remainingTime, int tprio);
// void delete_first_node(Node_t* node_address);

// Added functions
// thread_info_t* lrq_get( float cur_time, int t_id, int remain_time, int t_prio );
// thread_info_t* lrq_insert( float cur_time, int t_id, int remain_time, int t_prio );
// thread_info_t* lrq_search_id( int t_id );
// thread_info_t* srtf_search( thread_info_t* lrq_to_search );