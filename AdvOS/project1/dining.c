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
	printf("Updating philo %d state\n", phil_id);
	if (philo_states[phil_id] == HUNGRY 
		&& philo_states[left_of_phil(phil_id)] != EATING 
		&& philo_states[right_of_phil(phil_id)] != EATING)
	{
		printf("philo %d can eat now...\n", phil_id);
		pthread_mutex_lock(&chopstick_mutex[phil_id]);	
		philo_states[phil_id] = EATING;
		pthread_cond_signal(&chopstick_conds[phil_id]);
		pthread_mutex_unlock(&chopstick_mutex[phil_id]);			
	}
}

void pickup_one_chopstick(int stick_id, int phil_id){
	printf("Philosopher %d picks up chopstick %d \n", phil_id, stick_id);
	fflush(stdout);
}

void putdown_one_chopstick(int stick_id, int phil_id){

    printf("Philosopher %d puts down chopstick %d \n", phil_id, stick_id);
    fflush(stdout);
}

void pickup_chopsticks(int phil_id){  

	pthread_mutex_lock(&chopstick_mutex[phil_id]);
	philo_states[phil_id] = HUNGRY;
	pthread_mutex_unlock(&chopstick_mutex[phil_id]);
	update_philo_state(phil_id);

	pthread_mutex_lock(&chopstick_mutex[phil_id]);	
	while(philo_states[phil_id] == HUNGRY)
	{
		pthread_cond_wait(&chopstick_conds[phil_id],&chopstick_mutex[phil_id]);
	}
    pthread_mutex_unlock(&chopstick_mutex[phil_id]);
	
	printf("philo %d condition met, picking up chopsticks\n", phil_id );
    pickup_one_chopstick(phil_to_chopstick(phil_id, left), phil_id);
    pickup_one_chopstick(phil_to_chopstick(phil_id, right), phil_id);
}

void putdown_chopsticks(int phil_id){
  	/*Use putdown_chopstick to put down the chopsticks*/
  	printf("philo %d attempting to put down chopsticks and signal neighbors %d and %d\n",phil_id, 
  		left_of_phil(phil_id),right_of_phil(phil_id));

  	pthread_mutex_lock(&chopstick_mutex[phil_id]);
  	philo_states[phil_id] = THINKING;
  	pthread_mutex_lock(&chopstick_mutex[phil_id]);

  	printf("philo %d now thinking...\n", phil_id);

	putdown_one_chopstick(phil_to_chopstick(phil_id, left),phil_id);
	putdown_one_chopstick(phil_to_chopstick(phil_id, right),phil_id);

	update_philo_state(left_of_phil(phil_id));
	update_philo_state(right_of_phil(phil_id));
}


