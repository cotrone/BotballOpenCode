#ifndef __SERVOLIB_H__
#define __SERVOLIB_H__
#include <pthread.h>
struct servo_properties
{
	int port, max, min, initial, delta;
	pthread_t process_id;
    struct servo_movement *next_move;
}cbcservo[4];
struct servo_movement
{
	int position, tpm;
	long latency;
};
typedef struct servo_movement *servo_movement;
typedef struct servo_properties *servo;
pthread_mutex_t servo_mem = PTHREAD_MUTEX_INITIALIZER;
servo build_servo(int port, int min, int max)
{
	cbcservo[port].port = port;
	cbcservo[port].min = min;
	cbcservo[port].max = max;
	if(cbcservo[port].min >=0 && cbcservo[port].max <= 2048 && cbcservo[port].min < cbcservo[port].max)
	{
		return(&cbcservo[port]);
	}
	else
	{
		cbcservo[port].min = 0;
		cbcservo[port].max = 2048;
		return(&cbcservo[port]);
	}
}
servo_movement build_servo_movement(int position, int tpm, long latency)
{
	servo_movement new_movement = malloc(sizeof(struct servo_movement));
	new_movement->position = position;
	new_movement->tpm = tpm;
	new_movement->latency = latency;
	return(new_movement);
}
void *control_servo(void *ptr_servo)
{
	pthread_mutex_lock(&servo_mem);
    int i;
	servo build_properties = (struct servo_properties *) ptr_servo;
		if(build_properties->delta > 0)
		{
			for(i = 0; i < build_properties->delta; i++)
			{
				set_servo_position(build_properties->port, build_properties->initial + (build_properties->next_move->tpm * i));
				msleep(build_properties->next_move->latency);
			}
		}
		if(build_properties->delta < 0)
		{
			for(i = 0; i > build_properties->delta; i--)
			{
				set_servo_position(build_properties->port, build_properties->initial + (build_properties->next_move->tpm * i));
				msleep(build_properties->next_move->latency);
			}
		}
		if(get_servo_position(build_properties->port) != build_properties->next_move->position)
		{
			set_servo_position(build_properties->port, build_properties->next_move->position);
		}
	pthread_mutex_unlock(&servo_mem);
}
void move_servo(servo build_properties,servo_movement move_properties)
{
    if(build_properties->initial == -1)
	{
		set_servo_position(build_properties->port, build_properties->next_move->position);
	}
    else if(build_properties->max >= build_properties->next_move->position && build_properties->min <= build_properties->next_move->position)
	{
    build_properties->next_move = move_properties;
    build_properties->delta = (build_properties->next_move->position - get_servo_position(build_properties->port)) / build_properties->next_move->tpm;
    pthread_create(&build_properties->process_id, NULL, &control_servo, (void *)build_properties);
    }
	    else
	{
		printf("Invalid Movement of Servo: %d", build_properties->port);
	}
}
void bsd(servo build_properties)
{
	pthread_join(build_properties->process_id, NULL);
}
void wait_servo(servo build_properties, servo_movement move_properties)
{
	build_properties->next_move = move_properties;
    move_servo(build_properties, move_properties);
    bsd(build_properties);
}
void kill_servo(servo this_servo)
{
	pthread_kill(this_servo->process_id, 1);
}
#endif
