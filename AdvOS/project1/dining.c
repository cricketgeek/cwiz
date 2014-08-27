#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <errno.h>

#include <time.h>
#include <sys/time.h>

// #ifdef __MACH__
// #include <mach/clock.h>
// #include <mach/mach.h>
// #endif

typedef enum  {left, right} direction_t;

#define UNKNOWN 0
#define THINKING 1
#define HUNGRY 2
#define EATING 3
#define NUM_PHILOSOPHERS 5

struct timespec ts;

pthread_mutex_t chopstick_mutex[NUM_PHILOSOPHERS];
pthread_cond_t chopstick_conds[NUM_PHILOSOPHERS];
int philo_states[NUM_PHILOSOPHERS];

int phil_to_chopstick(int phil_id, direction_t d){
  return (phil_id + d) % NUM_PHILOSOPHERS;      
}

int chopstick_to_phil(int stick_id, direction_t d){
  return (stick_id + NUM_PHILOSOPHERS - d) % NUM_PHILOSOPHERS;      
}

int left_of_phil(int phil_id)
{
	return (phil_id + (NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS);
}

int right_of_phil(int phil_id)
{
	return ((phil_id + 1) % NUM_PHILOSOPHERS);
}

void update_philo_state(int phil_id)
{
	if (philo_states[phil_id] == HUNGRY 
		&& philo_states[left_of_phil(phil_id)] != EATING 
		&& philo_states[right_of_phil(phil_id)] != EATING)
	{
		philo_states[phil_id] = EATING;
		pthread_cond_signal(&chopstick_conds[phil_id],&chopstick_mutex[0]);
	}
}

void pickup_one_chopstick(int stick_id, int phil_id){

	// int               rc;
 //    struct timespec ts;
 //    struct timeval now;

// #ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
// 	clock_serv_t cclock;
// 	mach_timespec_t mts;
// 	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
// 	clock_get_time(cclock, &mts);
// 	mach_port_deallocate(mach_task_self(), cclock);
// 	ts.tv_sec = mts.tv_sec;
// 	ts.tv_nsec = mts.tv_nsec;
// #else
// 	/*clock_gettime(CLOCK_REALTIME, &ts); */
// 	/* Convert from timeval to timespec */
// 	ts.tv_sec  = tp.tv_sec;
// 	ts.tv_nsec = tp.tv_usec * 1000;
// 	rc =  gettimeofday(&tp, NULL);
// #endif
   //  gettimeofday(&now, NULL);
   //  ts.tv_sec = now.tv_sec;
   //  ts.tv_nsec = now.tv_usec * 1000; /* usec to nsec */
   //   add the offset to get timeout value 
  	// double randTime = rand() / (RAND_MAX + 1.0);    
   //  ts.tv_sec += randTime;

	// pthread_mutex_lock(&chopstick_mutex[stick_id]);
	// state[phil_id] = HUNGRY;
	// update_philo_state(phil_id);

	// while(state[phil_id] == HUNGRY)
	// {
	// 	pthread_cond_wait(&chopstick_conds[stick_id],&chopstick_mutex[stick_id]);
	// }
   	// rc = pthread_cond_timedwait(&chopstick_conds[stick_id],&chopstick_mutex[stick_id],&ts);
   	// if (rc == ETIMEDOUT) {
    //     rc = pthread_mutex_unlock(&chopstick_mutex[stick_id]);
    // }
    // else
    // {
		printf("Philosopher %d picks up chopstick %d \n", phil_id, stick_id);
		// pthread_mutex_unlock(&chopstick_mutex[stick_id]); //just in case?  	
    // }
	fflush(stdout);
}

void putdown_one_chopstick(int stick_id, int phil_id){

    printf("Philosopher %d puts down chopstick %d \n", phil_id, stick_id);
    fflush(stdout);
	// pthread_mutex_lock(&chopstick_mutex[stick_id]);
	// pthread_cond_signal(&chopstick_conds[stick_id]);
	// pthread_mutex_unlock(&chopstick_mutex[stick_id]);
}

void pickup_chopsticks(int phil_id){  

	pthread_mutex_lock(&chopstick_mutex[0]);
	philo_states[phil_id] = HUNGRY;
	update_philo_state(phil_id);

	while(philo_states[phil_id] == HUNGRY)
	{
		pthread_cond_wait(&chopstick_conds[phil_id],&chopstick_mutex[0]);
	}

    pickup_one_chopstick(phil_to_chopstick(phil_id, right), phil_id);
    pickup_one_chopstick(phil_to_chopstick(phil_id, left), phil_id);

    pthread_mutex_unlock(&chopstick_mutex[0]);
}

void putdown_chopsticks(int phil_id){
  	/*Use putdown_chopstick to put down the chopsticks*/
  	pthread_mutex_lock(&chopstick_mutex[0]);
  	philo_states[phil_id] = THINKING;

	putdown_one_chopstick(phil_to_chopstick(phil_id, right),phil_id);
	putdown_one_chopstick(phil_to_chopstick(phil_id, left),phil_id);

	update_philo_state(left_of_phil(phil_id));
	update_philo_state(right_of_phil(phil_id));

	pthread_mutex_unlock(&chopstick_mutex[0]);
	
}


