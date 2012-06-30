#ifndef __SERVOLIB_H__
#define __SERVOLIB_H__

#include <pthread.h>

struct servo_properties
{
	int port;
	int max;
	int min;
	int next_tpm;
	int next_position;
	int is_moving;
	pthread_t process_id;
	long next_latency;
}cbcservo[4];

struct servo_movement
{
	int smart_position;
	int smart_tpm;
	long smart_latency;
};

typedef struct servo_movement *servomove;
typedef struct servo_properties *servo;
pthread_mutex_t servo_mem = PTHREAD_MUTEX_INITIALIZER;

servo build_servo(int port, int min, int max)
{
	cbcservo[port].port = port;
	cbcservo[port].min = min;
	cbcservo[port].max = max;
	return(&cbcservo[port]);
}
servomove build_servomove(int smart_position, int smart_tpm, long smart_latency)
{
	servomove new_movement = malloc(sizeof(struct servo_movement));
	new_movement->smart_position = smart_position;
	new_movement->smart_tpm = smart_tpm;
	new_movement->smart_latency = smart_latency;
	return(new_movement);
}
void *control_servo(void *ptr_servo)
{
	servo build_properties = (struct servo_properties *) ptr_servo;
	int i;
	int initial = get_servo_position(build_properties->port);
	int delta = (build_properties->next_position - initial) / build_properties->next_tpm;
	
	if(build_properties->max >= build_properties->next_position && build_properties->min <= build_properties->next_position && build_properties->min < build_properties->max)
	{
		if(initial < build_properties->next_position)
		{
			for(i = 0; i < delta; i++)
			{
				set_servo_position(build_properties->port, initial + (build_properties->next_tpm * i));
				msleep(build_properties->next_latency);
			}
		}
		if(initial > build_properties->next_position)
		{
			for(i = 0; i > delta; i--)
			{
				set_servo_position(build_properties->port, initial + (build_properties->next_tpm * i));
				msleep(build_properties->next_latency);
			}
		}
	}
	if(get_servo_position(build_properties->port) != build_properties->next_position)
	{
		set_servo_position(build_properties->port, build_properties->next_position);
	}
	build_properties->is_moving = 1;
}
void wait_servo(servo build_properties, servomove move_properties)
{
	build_properties->next_position = move_properties->smart_position;
	build_properties->next_tpm = move_properties->smart_tpm;
	build_properties->next_latency = move_properties->smart_latency;
	control_servo((void *)build_properties);
}
void move_servo(servo build_properties,servomove move_properties)
{
	int thread_num;
	pthread_t this_thread;
	build_properties->is_moving = 1;
	build_properties->next_position = move_properties->smart_position;
	build_properties->next_tpm = move_properties->smart_tpm;
	build_properties->next_latency = move_properties->smart_latency;
	if((thread_num = pthread_create( &this_thread, NULL, &control_servo, (void *)build_properties)))
	{
		printf("Threading Failure: %d\n", thread_num);
	}
	else
	{
		build_properties->process_id = this_thread;
	}
}
void block_servo(servo build_properties)
{
		pthread_join(build_properties->process_id, NULL);
}
#endif
