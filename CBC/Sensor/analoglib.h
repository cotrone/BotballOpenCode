// Created on Thu Nov 17 2011
#include <stdio.h>
int analog_file_capture(int port, long wait, char description[]);
float analog_avg(int port, int res,long latency);
float analog_pchange(int port, int res, long wait);
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
float analog_avg(int port, int res,long latency)
{
	int i, sum;
	sum = 0;
	for(i = 0; i < res; i++)
	{
		sum += analog10(port);
		msleep(2L);
	}
	return((float)sum / (float)res);
}