
#ifndef __SERVOLIB_H__
#define __SERVOLIB_H__

#include <stdio.h>
struct servo_properties
{
	int port; //Servo Port
	int max;
	int min;
	int tpm; //Ticks per Movement
	long latency; //ms between each movement
};

typedef struct servo_properties *servo;
servo cbcservo[4];

servo build_servo(int localport, int localmin, int localmax, int localtpm, long locallatency)
{
	cbcservo[localport] = malloc(sizeof(struct servo_properties));
	cbcservo[localport]->port = localport;
	cbcservo[localport]->tpm = localtpm;
	cbcservo[localport]->max = localmax;
	cbcservo[localport]->min = localmin;
	cbcservo[localport]->latency = locallatency;
	return(cbcservo[localport]);
}
//Servo Movement
void wait_servo(servo properties, int position)
{
	int delta, initial,i;
	initial = get_servo_position(properties->port);
	delta = (position - initial) / properties->tpm;
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
}
#endif
