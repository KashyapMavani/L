#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#define MaxItems 5 // Maximum items a producer can produce or a consumer can consume
#define BufferSize 5 // Size of the buffer

sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[BufferSize];
pthread_mutex_t mutex;

void *producer(void *pno)
{
    int item;
    for(int i = 0; i < MaxItems; i++) {
        item = rand() % 1000; // Produce an random item  // random function  used to generate random  items
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);         //The mutex object referenced by mutex is locked by calling pthread_mutex_lock(). If the mutex is already locked,
                                            // the calling thread blocks until the mutex becomes available. This operation returns with the mutex object referenced by mutex in the locked state with the calling thread as its owner.
        buffer[in] = item;                  // inserts an item in buffer
        printf("Producer %d: Insert Item %d at %d\n", *((int *)pno),buffer[in],in);
        in = (in+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}
void *consumer(void *cno)
{
    for(int i = 0; i < MaxItems; i++) 
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        printf("Consumer %d: Remove Item %d from %d\n",*((int *)cno),item, out);
        out = (out+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main()
{
    pthread_t pro[5],con[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,BufferSize);
    sem_init(&full,0,0);
    int a[5] = {1,2,3,4,5}; //Just used for numbering the producer and consumer
    for(int i = 0; i < 5; i++) 
    {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);         // 1. stacksize: Inherited from the STACK runtime option 2. detachstate: Undetached
                                                                                //synch: Synchronous weight: Heavy
    }
    for(int i = 0; i < 5; i++) 
    {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }
    for(int i = 0; i < 5; i++) 
    {
        pthread_join(pro[i], NULL);
    }
    for(int i = 0; i < 5; i++) 
    {
        pthread_join(con[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}
