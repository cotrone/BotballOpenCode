#ifndef __LIGHTSTART_H__
#define __LIGHTSTART_H__

#include <pthread.h>

pthread_mutex_t lightstart_mem = PTHREAD_MUTEX_INITIALIZER;

struct run_table
{
	pthread_t thread_id;
	float process_kill_time;
};

void *wait_to_kill_all(void *process_prop)
{
	struct run_table *killer = (struct run_table *) process_prop;
	sleep(killer->process_kill_time);
	pthread_kill(killer->thread_id, 0);
}
void light_start(int port, float kill_time)
{
	int i, light_process;
	int light_off = -1;
	int light_on = -1;
	pthread_t kill_thread;
	
	struct run_table *main_process_attributes = malloc(sizeof(struct run_table));
	main_process_attributes->process_kill_time = kill_time;
	main_process_attributes->thread_id = pthread_self();
	
	cbc_display_clear();
	set_analog_floats(0);
	printf("A Button :\tProceed to Calibration\n");
	printf("B Button :\tRun Now!\n");
	
	while(1)
	{
		printf("\nTurn Light On and Off\n");
		if(analog10(port) > 512 && light_off == -1)
		{
			printf("Calibrating Off Value:");
			while(light_off == -1)
			{
				light_off = 0;
				for(i = 0; i < 3; i++)
				{
					light_off += analog10(port);
					msleep(10L);
				}
				light_off = light_off / 3;
				if(light_off < 512)
				{
					light_off = -1;
				}
			}
			printf("\t%d\n", light_off);
		}
		if(analog10(port) < 512 && light_on == -1)
		{
			printf("Calibrating On Value:");
			while(light_on == -1)
			{
				light_on = 0;
				for(i = 0; i < 3; i++)
				{
					light_on += analog10(port);
					msleep(10L);
				}
				light_on = light_on / 3;
				if(light_on > 512)
				{
					light_on = -1;
				}
			}
			printf("\t%d\n", light_on);
		}
		if(light_on < 512 && light_off > 512 && (light_off - light_on) > 100)
		{
			printf("\nCalibrated!\nOn: %d\nOff: %d\nDifference: %d\n\nWaiting for Light!\n", light_on, light_off, light_off-light_on);
			break;
		}
	}
	while(1)
	{
		if(analog10(port) > (light_off - light_on) / 2)
		{
			msleep(100L);
			while(analog10(port) > (light_off - light_on) / 2)
			{
				msleep(10L);
			}
			break;
		}
	}
	pthread_create(&kill_thread, NULL, wait_to_kill_all, (void *)main_process_attributes);
}
#endif

