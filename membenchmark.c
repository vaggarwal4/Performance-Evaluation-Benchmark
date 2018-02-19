#include<stdio.h>
#include<sys/time.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>

//MaxSize= 1GB
//BlockSize = 8,8KB,8MB,80MB
long gb= 1374389534.72;
long looptn,BlockSize;
int nthread;
 void *blockSequence()
{
  int i;
  long MaxSize=(long )(gb);
	long *memory=(long*)malloc(MaxSize);
	long *memory1=(long*)malloc(MaxSize);


	for(i = 0; i < looptn; i += BlockSize){

		memcpy(memory1, memory, BlockSize);
	}

	 free(memory);
	 free(memory1);

}

void *SequenceWriteMemset()
{

	  int i;
		long MaxSize=(long )(gb);
		long *memory=(long*)malloc(MaxSize);
		long *memory1=(long*)malloc(MaxSize);


		for(i = 0; i < looptn; i += BlockSize){

			memset(memory1,'@', BlockSize);
		}

		 free(memory);
		 free(memory1);

}
void *blockRandom()
{

	int i,r;
	long *memory=(long*)malloc(gb);
	long *memory1=(long*)malloc(gb);
	//m=(int)looptn/BlockSize;
	for (i = 0;i < looptn; i += BlockSize)
	{
		r = rand()% 250;
		memcpy(&memory1[r], &memory[r], BlockSize);
	}
free(memory);
free(memory1);

}

double multithreadSqRW(long a, int b){
  clock_t start,end;
  double time;
  int i;
  pthread_t th[10];
  BlockSize=a;
  nthread=b;
  printf("\n..Sequential Read+Write  for %ld BlockSize and %d thread..\n",BlockSize, nthread);
  printf("-------------------------------------");
  start=clock();
  looptn=gb/nthread;
  for(i=0;i<nthread;i++)
  {
    pthread_create(&th[i],NULL,blockSequence,NULL);//creates a thread

  }
  for(i=0;i<nthread;i++)
  {
    pthread_join(th[i], NULL); //joins the threads
  }
  end=clock();
  time = ((double)end - start) / CLOCKS_PER_SEC ;
  return time;
}


double multithreadRR(long a,long b){
  clock_t start,end;
  double time;
  int i;
  pthread_t th[10];
  BlockSize=a;
  nthread=b;
  printf("\n..Random Read for %ld BlockSize and %d thread\n",BlockSize,nthread);
  printf("-------------------------------------");
  start = clock();
  looptn=gb/nthread;
  for(i=0;i<nthread;i++)
  {
    pthread_create(&th[i],NULL,blockRandom,NULL);//creates a thread

  }
  for(i=0;i<nthread;i++)
  {
    pthread_join(th[i], NULL); //joins the threads
  }
  end = clock();
  time = ((double)end - start) / CLOCKS_PER_SEC;
  return time;
}


double multithreadSW(long a, long b){
  clock_t start,end;
  double time;
  int i;
  pthread_t th[10];
  BlockSize=a;
  nthread=b;
  printf("\n..Sequential Write for %ld BlockSize and %d thread\n",BlockSize,nthread);
  printf("-------------------------------------");
  start = clock();
  looptn=gb/nthread;
  for(i=0;i<nthread;i++)
  {
    pthread_create(&th[i],NULL,SequenceWriteMemset,NULL);//creates a thread

  }
  for(i=0;i<nthread;i++)
  {
    pthread_join(th[i], NULL); //joins the threads
  }
  end = clock();
  time = ((double)end - start) / CLOCKS_PER_SEC;
  return time;

}

int main()
{
  double time;
  double latency;
  double throughput;
  //Latency for BlockSize = 8 and thread =1
  time =  multithreadSqRW(8,1);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lf us\n",latency);
  time =  multithreadSW(8,1);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lfus\n",latency);
  time =  multithreadRR(8,1);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lfus\n",latency);
//Latency for BlockSize = 8 and thread =2
  time =  multithreadSqRW(8,2);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lfus\n",latency);
  time =  multithreadSW(8,2);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lfus\n",latency);
  time =  multithreadRR(8,2);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lfus\n",latency);
//Latency for BlockSize = 8 and thread =4
  time =  multithreadSqRW(8,4);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lfus\n",latency);
  time =  multithreadSW(8,4);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lfus\n",latency);
  time =  multithreadRR(8,4);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lfus\n",latency);
//Latency for BlockSize = 8 and thread =8
  time =  multithreadSqRW(8,8);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lf\nus",latency);
  time =  multithreadSW(8,8);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lf\nus",latency);
  time =  multithreadRR(8,8);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lf\nus",latency);
//throughput for BlockSize = 8192 and thread =1
  time =  multithreadSqRW(8192,1);
  throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
  printf("\n Throughput :%.5lfMB/sec\n",throughput);
  time =  multithreadSW(8192,1);
  throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
  printf("\n Throughput :%.5lfMB/sec\n",throughput);
  time =  multithreadRR(8192,1);
  throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
  printf("\n Throughput :%.5lfMB/sec\n",throughput);

  //throughput for BlockSize = 8192 and thread =2
    time =  multithreadSqRW(8192,2);
    throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
    printf("\n Throughput :%.5lfMB/sec\n",throughput);
    time =  multithreadSW(8192,2);
    throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
    printf("\n Throughput :%.5lfMB/sec\n",throughput);
    time =  multithreadRR(8192,2);
    throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
    printf("\n Throughput :%.5lfMB/sec\n",throughput);

    //throughput for BlockSize = 8192 and thread =4
      time =  multithreadSqRW(8192,4);
      throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
      printf("\n Throughput :%.5lfMB/sec\n",throughput);
      time =  multithreadSW(8192,4);
      throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
      printf("\n Throughput :%.5lfMB/sec\n",throughput);
      time =  multithreadRR(8192,4);
      throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
      printf("\n Throughput :%.5lfMB/sec\n",throughput);

      //throughput for BlockSize = 8192 and thread =8
        time =  multithreadSqRW(8192,8);
        throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
        printf("\n Throughput :%.5lfMB/sec\n",throughput);
        time =  multithreadSW(8192,8);
        throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
        printf("\n Throughput :%.5lfMB/sec\n",throughput);
        time =  multithreadRR(8192,8);
        throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
        printf("\n Throughput :%.5lfMB/sec\n",throughput);

        //throughput for BlockSize = 8388608 and thread =1
          time =  multithreadSqRW(8388608,1);
          throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
          printf("\n Throughput :%.5lfMB/sec\n",throughput);
          time =  multithreadSW(8388608,1);
          throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
          printf("\n Throughput :%.5lfMB/sec\n",throughput);
          time =  multithreadRR(8388608,1);
          throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
          printf("\n Throughput :%.5lfMB/sec\n",throughput);

          //throughput for BlockSize = 8388608 and thread =2
            time =  multithreadSqRW(8388608,2);
            throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
            printf("\n Throughput :%.5lfMB/sec\n",throughput);
            time =  multithreadSW(8388608,2);
            throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
            printf("\n Throughput :%.5lfMB/sec\n",throughput);
            time =  multithreadRR(8388608,2);
            throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
            printf("\n Throughput :%.5lfMB/sec\n",throughput);

            //throughput for BlockSize = 8388608 and thread =4
              time =  multithreadSqRW(8388608,4);
              throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
              printf("\n Throughput :%.5lfMB/sec\n",throughput);
              time =  multithreadSW(8388608,4);
              throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
              printf("\n Throughput :%.5lfMB/sec\n",throughput);
              time =  multithreadRR(8388608,4);
              throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
              printf("\n Throughput :%.5lfMB/sec\n",throughput);

              //throughput for BlockSize = 8388608 and thread =8
                time =  multithreadSqRW(8388608,8);
                throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
                printf("\n Throughput :%.5lfMB/sec\n",throughput);
                time =  multithreadSW(8388608,8);
                throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
                printf("\n Throughput :%.5lfMB/sec\n",throughput);
                time =  multithreadRR(8388608,8);
                throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
                printf("\n Throughput :%.5lfMB/sec\n",throughput);

                //throughput for BlockSize = 8388608 and thread =1
                  time =  multithreadSqRW(83886080,1);
                  throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
                  printf("\n Throughput :%.5lfMB/sec\n",throughput);
                  time =  multithreadSW(83886080,1);
                  throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
                  printf("\n Throughput :%.5lfMB/sec\n",throughput);
                  time =  multithreadRR(83886080,1);
                  throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
                  printf("\n Throughput :%.5lfMB/sec\n",throughput);

                  //throughput for BlockSize = 8388608 and thread =2
                    time =  multithreadSqRW(83886080,2);
                    throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
                    printf("\n Throughput :%.5lfMB/sec\n",throughput);
                    time =  multithreadSW(83886080,2);
                    throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
                    printf("\n Throughput :%.5lfMB/sec\n",throughput);
                    time =  multithreadRR(83886080,2);
                    throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
                    printf("\n Throughput :%.5lfMB/sec\n",throughput);

                    //throughput for BlockSize = 8388608 and thread =4
                      time =  multithreadSqRW(83886080,4);
                      throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
                      printf("\n Throughput :%.5lfMB/sec\n",throughput);
                      time =  multithreadSW(83886080,4);
                      throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
                      printf("\n Throughput :%.5lfMB/sec\n",throughput);
                      time =  multithreadRR(83886080,4);
                      throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
                      printf("\n Throughput :%.5lfMB/sec\n",throughput);

                      //throughput for BlockSize = 8388608 and thread =8
                        time =  multithreadSqRW(83886080,8);
                        throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
                        printf("\n Throughput :%.5lfMB/sec\n",throughput);
                        time =  multithreadSW(83886080,8);
                        throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
                        printf("\n Throughput :%.5lfMB/sec\n",throughput);
                        time =  multithreadRR(83886080,8);
                        throughput=(looptn*nthread*BlockSize)/(time*1024*1204);
                        printf("\n Throughput :%.5lfMB/sec\n",throughput);


}
