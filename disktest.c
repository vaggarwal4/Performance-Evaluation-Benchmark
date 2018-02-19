#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

long  gb = 10737418240;
long BlockSize;
int nthread;
long looptn;


void makefile(){
    int i;
    FILE *fp;

   fp = fopen("test1.text", "w");
   for( i=0;i<400000000;i++)
   {
   fputs("Creating a file test...\n", fp);
    }
    fclose(fp);
}

void * blockSequence()
{
    int i;
    char * buffer = (char *)malloc(BlockSize * sizeof(char));


    int fp;
    if ((fp = open("test1.text", O_RDONLY, 0666)) < 0) {
        printf("File open failed.\n");
        exit(-1);
    }

    for ( i = 0; i < looptn / BlockSize; i++) {
        read(fp, buffer, BlockSize);
    }

    close(fp);
    free(buffer);

    return NULL;
}




void * blockRandom()
{
    int i,r;
    char * buffer = (char *)malloc(BlockSize * sizeof(char));
    srand((int)time(0));

    int fp;
    if ((fp = open("test1.text", O_RDONLY, 0666)) < 0) {
        printf("File open failed.\n");
        exit(-1);
    }

    for ( i = 0; i < looptn / BlockSize; i++) {
        r = (rand() % 250);
        lseek(fp, r, SEEK_SET);
        read(fp, buffer, BlockSize);
    }

    close(fp);
    free(buffer);

    return NULL;
}

void * SequenceWriteMemset()
{
    int i;
    char * buffer = (char *)malloc(BlockSize * sizeof(char));


    int fp;
    if ((fp = open("test1.text", O_CREAT|O_TRUNC|O_WRONLY, 0666)) < 0) {
        printf("File open failed.\n");
        exit(-1);
    }

    for ( i = 0; i < looptn / BlockSize; i++) {
        //memset(buffer, '@', BlockSize);
        write(fp, buffer, BlockSize);
    }

    close(fp);
    free(buffer);

    return NULL;
}

double multithreadSqRW(long a, int b){
  clock_t start,end;
  double time;
  int i;
  pthread_t th[10];
  BlockSize=a;
  nthread=b;
  printf("\n..Sequential Read  for %ld BlockSize and %d thread..\n",BlockSize, nthread);
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
  printf("Creating a large file \n");
  makefile();
  printf("File Created............\n");
  time =  multithreadSqRW(8,1);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lf ms\n",latency);
  time =  multithreadSW(8,1);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lfms\n",latency);
  time =  multithreadRR(8,1);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lfms\n",latency);
//Latency for BlockSize = 8 and thread =2
  time =  multithreadSqRW(8,2);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lfms\n",latency);
  time =  multithreadSW(8,2);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lfms\n",latency);
  time =  multithreadRR(8,2);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lfms\n",latency);
//Latency for BlockSize = 8 and thread =4
  time =  multithreadSqRW(8,4);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lfms\n",latency);
  time =  multithreadSW(8,4);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lfms\n",latency);
  time =  multithreadRR(8,4);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lfms\n",latency);
//Latency for BlockSize = 8 and thread =8
  time =  multithreadSqRW(8,8);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lf\nms",latency);
  time =  multithreadSW(8,8);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lf\nms",latency);
  time =  multithreadRR(8,8);
  latency=(time*1000000)/(looptn*nthread*BlockSize);
  printf("\n latency :%.12lf\nms",latency);
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
