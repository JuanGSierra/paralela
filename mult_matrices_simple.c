#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define N 1024
#define THREADS 1

double piTotal[THREADS];

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

void filexcolumn(int i, int size, int j)
{
  int aux = 0;
  for (int p = 0; p < size; p++)
    ms[i][j] += m1[i][p] * m2[p][j];
}

int main(void)
{

  struct timeval tval_before, tval_after, tval_result;

  getArray(m1);
  getArrayMirror(m2);
  gettimeofday(&tval_before, NULL);

  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      int aux = 0;
      for (int k = 0; k < N; k++)
      {
        aux += m1[i][k] * m2[k][j];
      }
      ms[i][j] = aux;
    }
  }

  gettimeofday(&tval_after, NULL);
  timersub(&tval_after, &tval_before, &tval_result);
  printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
  //   printArr(m1);
  //   printArr(ms);
  return 0;
}