#include<stdio.h>
#include<string.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>    
int i;
clock_t start, end;
start = clock();
int a=4;
int b=5;
int sum;
for(i = 0; i < 10000000; i++)
{
	sum= a + b

}
end = clock();
cpu= ((int) (end - start)) / CLOCKS_PER_SEC;
int Iops=(ITERATIONS)/(int)(cpu_time_used);
printf("\nTime: %f s\n",cpu_time_used);