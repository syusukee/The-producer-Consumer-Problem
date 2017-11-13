#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define MAX 50
#define buffer_MAX 10
pthread_mutex_t the_mutex;
pthread_cond_t condc,condp;
int buffer = 0;
int total_pro = 0;
int total_con = 0;
void *producer(void *ptr)
{
	while (total_pro < MAX){
		pthread_mutex_lock(&the_mutex);
		while (buffer == buffer_MAX){
		        pthread_t tid = pthread_self();
                        printf("FULL! Producer %u is waiting!\n",(unsigned int)tid);
			pthread_cond_wait(&condp, &the_mutex);	
		}
                total_pro++;
		buffer++;
                sleep(0.1);
		pthread_t tid = pthread_self();
		printf("Producer %u produce %d \n",(unsigned int)tid,total_pro);
		pthread_cond_broadcast(&condc);
		pthread_cond_broadcast(&condp);
		pthread_mutex_unlock(&the_mutex);
	}
	pthread_exit(0);
}

void *consumer(void *ptr)
{
	while (total_con < MAX){
		pthread_mutex_lock(&the_mutex);
		while (buffer == 0){
		        pthread_t tid = pthread_self();
                        printf("EMPTY! Consumer %u is waiting!\n",(unsigned int)tid);
			pthread_cond_wait(&condc, &the_mutex);
		}
                total_con++;
		buffer--;
                sleep(0.1);
		pthread_t tid = pthread_self();
		printf("Consumer %u consume %d \n", (unsigned int)tid,total_con);
		pthread_cond_broadcast(&condp);
		pthread_cond_broadcast(&condc);
		pthread_mutex_unlock(&the_mutex);
	}
	pthread_exit(0);
}

int main(int argc, char **argv)
{
	pthread_t pro1, pro2, pro3, con1, con2;
	pthread_mutex_init(&the_mutex, 0);
	pthread_cond_init(&condc, 0);
	pthread_cond_init(&condp, 0);
	pthread_create(&con1, 0, consumer, 0);
	pthread_create(&con2, 0, consumer, 0);
	pthread_create(&pro1, 0, producer, 0);
	pthread_create(&pro2, 0, producer, 0);
	pthread_create(&pro3, 0, producer, 0);
	pthread_join(pro1, 0);
	pthread_join(pro2, 0);
	pthread_join(pro3, 0);
	pthread_join(con1, 0);
	pthread_join(con2, 0);
	pthread_cond_destroy(&condc);
	pthread_cond_destroy(&condp);
	pthread_mutex_destroy(&the_mutex);
}
