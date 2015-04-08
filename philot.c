//--------------------------------------------------------
// Choong Huh
// The Dining Philosophers: POSIX Thread Implementation
// Washington State University - Vancouver

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define _POSIX_SOURCE

#define MAX_TIME 100

// GLOBAL RESOURCES

int philoNum[] = {0,1,2,3,4};
int semID;
pthread_mutex_t chopsticks[5];

//===================================================
// - randomGaussian_r 
// Provided by professor Dick Lang
//

int randomGaussian_r(int mean, int stddev, unsigned int* state) {
	double mu = 0.5 + (double) mean;
	double sigma = fabs((double) stddev);
	double f1 = sqrt(-2.0 * log((double) rand_r(state) / (double) RAND_MAX));
	double f2 = 2.0 * 3.14159265359 * (double) rand_r(state) / (double) RAND_MAX;
	if (rand_r(state) & (1 << 5))
		return (int) floor(mu + sigma * cos(f2) * f1);
	else
		return (int) floor(mu + sigma * sin(f2) * f1);
}

void* activity(void *philoNum)
{
	int id = *(int*)philoNum; // important typecast
	int chopLeft = id;
	int chopRight = (id+1)%4;
	int temp;
	
	int nomTime = 0;
	int thinkTime = 0;
	
	while(nomTime<MAX_TIME)
	{
		pthread_mutex_lock(&chopsticks[id]);
		if(!pthread_mutex_trylock(&chopsticks[chopRight])==0)		// can't grab the other chopstick!
		{
			// release the left chopstick for others and prevent deadlock
			pthread_mutex_unlock(&chopsticks[id]);

			// give it a brief time to sleep before attempting again
			temp = abs(randomGaussian_r(1, 5, (unsigned int*) philoNum));
			printf("Philosopher %d is thinking for %d seconds\n",id,temp);

			sleep(temp);
		}
		else
		{
			// feeding randomGaussian with stddev, mean, and philoNum (as random seed)
			
			temp = abs(randomGaussian_r(9,3, (unsigned int*) philoNum));			

			nomTime += temp;
			printf("Philosopher %d is eating for %d seconds\n",id,temp);
			sleep(temp);
			
			pthread_mutex_unlock(&chopsticks[id]);			// Philosopher releases chopsticks after finishing eating
            pthread_mutex_unlock(&chopsticks[chopRight]);
			
			temp = abs(randomGaussian_r(11,7, (unsigned int*) philoNum));
			printf("Philosopher %d is thinking for %d seconds\n",id,temp);

			sleep(temp);			// Philosopher now thinks
			
		}
		
	}
	
	// Philosopher has finished to eat for MAX_TIME seconds and therefore terminates.

	printf("Philosopher %d has left the table\n",id);
	return 0;
}

int main (void)
{
int i, rd;
int pid;

pthread_t philosopher[5];

/*
Below blocks initialize five threads and assign them the initial function to be run.
When finished executing, the program cleans up the memory and terminates.
*/

    for(i = 0; i < 5; i++)
    {
        if(pthread_mutex_init(&chopsticks[i], NULL) != 0)
            printf("Failed to initialize mutex\n");
    }
    
    for(i = 0; i < 5; i++)
    {
        if(pthread_create(&philosopher[i], NULL, activity, (void *) &philoNum[i])  != 0)
            printf("Failed to initialize thread\n") ;
    }
    
    for(i = 0; i < 5; i++)
    {
        pthread_join(philosopher[i], NULL);
    }
    
    for(i = 0; i < 5; i++)
    {
        pthread_mutex_destroy(&chopsticks[i]);
    }
	
	return 0;
}
