
#ifndef __SERVOLIB_H__
#define __SERVOLIB_H__

#include <stdio.h>
struct servo_properties
{
	int port;
	int max;
	int min;
	int tpm;
	long latency;
};

typedef struct servo_properties *servo;
servo cbcservo[4];

int servo_port, servo_final_position, servo_inital_position, servo_tpm, servo_min, servo_max;
long servo_latency;

servo build_servo(int port, int min, int max, int tpm, long latency)
{
	cbcservo[port] = malloc(sizeof(struct servo_properties));
	cbcservo[port]->port = port;
	cbcservo[port]->tpm = tpm;
	cbcservo[port]->max = max;
	cbcservo[port]->min = min;
	cbcservo[port]->latency = latency;
	return(cbcservo[port]);
}
void rebuild_servo(servo properties, int tpm, long locallatency)
{
    properties->tpm = tpm;
    properties->latency = latency;
}
void wait_servo(servo properties, int position)
{
	int i;
	int initial = get_servo_position(properties->port);
	int delta = (position - initial) / properties->tpm;
    
	if(properties->max >= position && properties->min <= position)
	{
		if(initial < position)
		{
			for(i = 0; i < delta; i++)
			{
				set_servo_position(properties->port, initial + (properties->tpm * i));
				msleep(properties->latency);
			}
		}
		if(initial > position)
		{
			for(i = 0; i > delta; i--)
			{
				set_servo_position(properties->port, initial + (properties->tpm * i));
				msleep(properties->latency);
			}
		}
	}
    if(get_servo_position(properties->port) != position)
    {
        set_servo_position(properties->port, position);
    }
}
void move_servo(servo properties, int position)
{
    servo_port = properties->port;
    servo_final_position = position;
    servo_intitial_position = get_servo_position(properties->port);
    servo_tpm = properties->tpm;
    servo_latency = properties->latency;
    servo_max = properties->max;
    servo_min = properties->min;
    start_process(servo_thread_movement);
}
void servo_thread_movement()
{
    int i;
    int delta = (servo_final_position - servo_inital_position) / servo_tpm;
	if(servo_max >= servo_final_position && servo_min <= servo_final_position)
	{
		if(servo_inital_position < servo_final_position)
		{
			for(i = 0; i < delta; i++)
			{
				set_servo_position(servo_port, servo_inital_position + (servo_tpm * i));
				msleep(servo_latency);
			}
		}
		else if(servo_inital_position > servo_final_position)
		{
			for(i = 0; i > delta; i--)
			{
				set_servo_position(servo_port, servo_inital_position + (servo_tpm * i));
				msleep(servo_latency);
			}
		}
 
	}
    if(get_servo_position(servo_port) != servo_final_position)
    {
        set_servo_position(servo_port, servo_final_position);
    }
}
#endif
