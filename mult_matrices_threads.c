#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define N 2000
#define THREADS 128

double piTotal[THREADS];

struct indices
{
    int i;
    int j;
};

int ms[N][N];
int m1[N][N];
int m2[N][N];

void getArray(int arr[N][N])
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            arr[i][j] = random() % 9 + 1;
}

void getArrayMirror(int arr[N][N])
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (i == j)
                arr[i][j] = 1;
            else
                arr[i][j] = 0;
}

void printArr(int arr[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void *filexcolumn(void *arg)
{
    int initIteration, endIteration, threadId = *(int *)arg;

    initIteration = (N * N / THREADS) * threadId;

    if (threadId == THREADS - 1)
        endIteration = N * N;
    else
        endIteration = initIteration + ((N * N / THREADS) - 1);

    
    for (int aux = initIteration; aux < endIteration; aux++)
        for (int p = 0; p < N; p++)
        {
            int j = aux % N;
            int i = (aux - j) / N;
            ms[i][j] += m1[i][p] * m2[p][j];
        }
}

int main(void)
{

    struct timeval tval_before, tval_after, tval_result;

    int threadId[THREADS], i, *retval;
    pthread_t thread[THREADS];

    getArray(m1);
    getArrayMirror(m2);
    gettimeofday(&tval_before, NULL);

    for (i = 0; i < THREADS; i++)
    {
        threadId[i] = i;
        pthread_create(&thread[i], NULL, (void *)filexcolumn, &threadId[i]);
    }

    for (i = 0; i < THREADS; i++)
    {
        pthread_join(thread[i], (void **)&retval);
    }

    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
    //   printArr(m1);
    //   printArr(ms);
    return 0;
}