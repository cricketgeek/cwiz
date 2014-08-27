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
struct timespec ts;

pthread_mutex_t chopstick_mutex[5];
pthread_cond_t chopstick_conds[5];
int chopsticks[5];

int phil_to_chopstick(int phil_id, direction_t d){
  return (phil_id + d) % 5;      
}

int chopstick_to_phil(int stick_id, direction_t d){
  return (stick_id + 5 - d) % 5;      
}

void pickup_one_chopstick(int stick_id, int phil_id){

	int               rc;
    struct timespec ts;
    struct timeval now;

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
    gettimeofday(&now, NULL);
    ts.tv_sec = now.tv_sec;
    ts.tv_nsec = now.tv_usec * 1000; /* usec to nsec */
    /* add the offset to get timeout value */
  	double randTime = rand() / (RAND_MAX + 1.0);    
    ts.tv_sec += randTime; //+ (int)stick_id);

  	// double randTime = rand() / (RAND_MAX + 1.0);
  	// printf("randTime is %f\n", randTime);

  	// printf("asctime of ts:%s",asctime(ts));
    //ts.tv_sec += randTime;

	// printf("\nWe are using time of %lld.%.9ld\n", (long long)ts.tv_sec, ts.tv_nsec);

	pthread_mutex_lock(&chopstick_mutex[stick_id]);

   	rc = pthread_cond_timedwait(&chopstick_conds[stick_id],&chopstick_mutex[stick_id],&ts);
   	if (rc == ETIMEDOUT) {
        printf("Philosopher %d has waited too long and furiously unlocks\n",phil_id);
        rc = pthread_mutex_unlock(&chopstick_mutex[stick_id]);
    }
    else
    {
		printf("Philosopher %d picks up chopstick %d \n", phil_id, stick_id);
		pthread_mutex_unlock(&chopstick_mutex[stick_id]); //just in case?  	
    }
	fflush(stdout);
}

void putdown_one_chopstick(int stick_id, int phil_id){

    printf("Philosopher %d puts down chopstick %d \n", phil_id, stick_id);
    fflush(stdout);
	pthread_mutex_lock(&chopstick_mutex[stick_id]);
	pthread_cond_signal(&chopstick_conds[stick_id]);
	pthread_mutex_unlock(&chopstick_mutex[stick_id]);
}

void pickup_chopsticks(int phil_id){  
    pickup_one_chopstick(phil_to_chopstick(phil_id, right), phil_id);
    pickup_one_chopstick(phil_to_chopstick(phil_id, left), phil_id);
}

void putdown_chopsticks(int phil_id){
  	/*Use putdown_chopstick to put down the chopsticks*/
	putdown_one_chopstick(phil_to_chopstick(phil_id, right),phil_id);
	putdown_one_chopstick(phil_to_chopstick(phil_id, left),phil_id);
}


