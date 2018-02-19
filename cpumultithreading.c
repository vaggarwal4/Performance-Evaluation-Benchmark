
include<stdio.h>
#include<string.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h> 
#define iterations 1000000
void *Flop(void *arg)
{
  double a=5;
  double b=10;
  int i;
  double sum;
  
  // convert string argument to long
  long iterations=strtol((char*)arg,NULL,0);
  for(i = 0; i < iterations; i++)
  {
    sum=a+b;
  }
  return NULL;
}
void threadflop(int nofthreads)
{
  clock_t start, end;
  double cputime;
  long double a=5;
  int i;
  pthread_t th[10];
  long iterations=ITERATIONS/nofthreads;
  char iterationStr[20];
  printf(iterationStr, 20, "%lu",iterations);

  printf("\nProgram to find FLOPS for %d threads",nofthreads);
  start = clock();
  for(i=0;i<nofthreads;i++)
  {
    pthread_create(&th[i],NULL,Flop,iterationStr);
    pthread_join(th[i], NULL);
  }
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("\nTime: %f s\n",cpu_time_used);
  double Flops=(ITERATIONS)/(double)(cpu_time_used);
  double gFlops=(double)Flops/1000000000;// Calculate Giga Flops Formula: F lops * 10raised to (-9).
  printf("GFLOPS : %f\n",gFlops);
}

int main()
{
  

  threadflop(1);
  threadflop(2);
  threadflop(4);
  threadflop(8);


  return 0;
}