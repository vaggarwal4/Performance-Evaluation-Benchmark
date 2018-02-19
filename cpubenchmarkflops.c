nclude<stdio.h>
#include<string.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>    
int i;
clock_t start, end;
start = clock();
double a=4;
double b=5;
double sum;
for(i = 0; i < 10000000; i++)
{
	 sum= a + b;

}
end = clock();
cpu= ((double) (end - start)) / CLOCKS_PER_SEC;
double Flops=(ITERATIONS)/(double)(cpu_time_used);
printf("\nTime: %f s\n",cpu_time_used);