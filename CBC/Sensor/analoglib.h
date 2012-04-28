// Created on Thu Nov 17 2011
#include <stdio.h>

int analog_file_capture(int port, long wait, char description[])
{
	FILE *esv = fopen("analog_test.txt", "a");
	long i;
	if( esv == NULL )
	{
		return(-1);
	}
	else
	{
		fprintf(esv, "%s\n", description);
		fprintf(esv, "Sensor Value\n");
		if(wait < 10000L)
		{
			for(i == i; (i * 10) < wait; i++)
			{
				fprintf(esv, "%d\n", analog10(port));
				msleep(10L);
			}
		}
		else
		{
			fprintf(esv, "Error...Please Review Code\n");
		}
		fprintf(esv, "Termination\n");
		fclose(esv);
		return(0);
	}
}
float analog_avg(int port, int res)
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
float analog_pchange(int port, int res, long wait)
{
	float asa[2];
	
	asa[0] = analog_avg(port, res);
	msleep(wait);
	asa[1] = analog_avg(port, res);
	
	return((asa[1] - asa[0]) / asa[0]);
}
