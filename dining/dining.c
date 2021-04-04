#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N
static int totround=0;
int state[N];
int forks[N+1]={0,0,0,0,0,0};
int count[N]={0,0,0,0,0};
int phil[N] = { 0, 1, 2, 3, 4 };
pthread_mutex_t lock;
sem_t mutex;
sem_t S[N];

void test(int phnum)
{
	if (state[phnum] == HUNGRY
		&& state[LEFT] != EATING
		&& state[RIGHT] != EATING) {
		
		state[phnum] = EATING;

		sleep(2);

		printf("PHIL[%d] takes fork %d and %d\n",
					phnum + 1, LEFT + 1, phnum + 1);
		forks[LEFT+1]=1;
		forks[phnum+1]=1;
	    printf("STICKS ");
		printf("[12345]\n");
		printf("STICKS [");
		for(int i=1;i<6;i++)
		{
			printf("%d",forks[i]);
		}
		printf("]\n");
		

		printf("PHIL[%d]  is Eating\n", phnum + 1);
		sem_post(&S[phnum]);
	}
}


void take_fork(int phnum)
{

	sem_wait(&mutex);

	
	state[phnum] = HUNGRY;

	printf("PHIL[%d]  is Hungry\n", phnum + 1);

	
	test(phnum);

	sem_post(&mutex);

	sem_wait(&S[phnum]);

	sleep(1);
}


void put_fork(int phnum)
{

	sem_wait(&mutex);

	state[phnum] = THINKING;

	printf("PHIL[%d] putting fork %d and %d down\n",
		phnum + 1, LEFT + 1, phnum + 1);
	printf("PHIL[%d] ROUND %d DONE\n", phnum+1, count[phnum]);
	pthread_mutex_lock(&lock);
	pthread_mutex_unlock(&lock);
	forks[LEFT+1]=0;
	forks[phnum+1]=0;
	printf("STICKS ");
	printf("[12345]\n");
	printf("STICKS [");
	for(int i=1;i<6;i++)
	{
		printf("%d",forks[i]);
	}
	printf("]\n");
	if(count[phnum]<3)
        {
	printf("PHIL[%d] is thinking\n", phnum + 1);
	test(LEFT);
	test(RIGHT);
        }
	sem_post(&mutex);
	
}

void* philospher(void* num)
{
	int *j1= num;
        int j= *j1;
        


	while (count[j]<3) 
       {
                

		int* i = num;

		sleep(1);

		take_fork(*i);

		sleep(0);

		put_fork(*i);
                count[j]=count[j]+1;
	}
        totround+=1;
        pthread_exit(NULL);
        
}

int main()
{
        pthread_mutex_init(&lock, NULL);

	int i;
	pthread_t thread_id[N];
	sem_init(&mutex, 0, 1);

	for (i = 0; i < N; i++)

		sem_init(&S[i], 0, 0);

	for (i = 0; i < N; i++)
	{

		pthread_create(&thread_id[i], NULL,
					philospher, &phil[i]);

		printf("PHIL[%d]  is thinking\n", i + 1);
	}

	for (i = 0; i < N; i++)

		pthread_join(thread_id[i], NULL);
        printf("DINNER PARTY OVER\n");
}
