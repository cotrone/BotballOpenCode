// Created on Thu Nov 17 2011
#include <stdio.h>
int analog_file_capture(int port, long wait, char description[]);
// 0 <--> 7 (port), + in ms, "..." character string
//This funtion saves analog sensor data for later analysis

float analog_avg(int port, int res,long latency);
// 0 <--> 7 (port), + (# of trials), + in ms
//This function averages an analog sensor using 'res' trials with 'latency" ms between each reading

float analog_pchange(int port, int res, long latency, long pause);
// 0 <--> 7 (port),  + (# of trials), + in ms, + in ms
//This function uses analog_avg to measure the proportional change between two trials
int analog_file_capture(int port, long wait, char description[])
{
    FILE *acap = fopen("analog_test.txt", "a");
    long i;
    if( acap == NULL )
    {
        return -1;
    }
    else
    {
        fprintf(acap, "%s\n", description);
        fprintf(acap, "Sensor Value\n");
        if(wait < 120000L)
        {
            for(i == i; (i * 10) < wait; i++)
            {
                fprintf(acap, "%d\n", analog10(port));
                msleep(10L);
            }
        }
        else
        {
            fprintf(acap, "\nWarining! Invalid Wait time\n");
        }
        fprintf(acap, "Termination\n");
        fclose(acap);
        return 0;
    }
}
float analog_avg(int port, int res, long latency)
{
	int i, sum;
	sum = 0;
	for(i = 0; i < res; i++)
	{
		sum += analog10(port);
		msleep(latency);
	}
	return((float)sum / (float)res);
}
float analog_pchange(int port, int res, long latency, long pause)
{
    float initial = analog_avg(port, res, latency);
    msleep(pause);
    float final = analog_avg(port, res, latency);
    
    return((final - initial) / initial);
}