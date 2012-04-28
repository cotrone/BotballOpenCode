
#ifndef __SERVOLIB_H__
#define __SERVOLIB_H__

#include <stdio.h>
typedef struct servo_properties
{
	int port; //Servo Port
	int max;
	int min;
	int tpm; //Ticks per Movement
	int next_move;
	int next_position;
	int process_id;
	long latency; //ms between each movement
} myservo;

typedef myservo *servo;
servo cbcservo[4];

servo build_servo(int localport, int localmin, int localmax, int localtpm, long locallatency)
{
	cbcservo[localport] = malloc(sizeof(struct servo_properties));
	cbcservo[localport]->port = localport;
	cbcservo[localport]->tpm = localtpm;
	cbcservo[localport]->max = localmax;
	cbcservo[localport]->min = localmin;
	cbcservo[localport]->latency = locallatency;
	cbcservo[localport]->next_move = 0;
	return(cbcservo[localport]);
}
//Servo Movement
int wait_servo(servo properties, int position)
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
	return(0);
}
//Sevo Thread Update
int update_servo_buffer(servo properties, int position)
{
	cbcservo[properties->port]->next_position = position;
	cbcservo[properties->port]->next_move = 1;
}
void move_servo_0(void)
{
	if(cbcservo[0]->next_move)
		wait_servo(cbcservo[0], cbcservo[0]->next_position);
	cbcservo[0]->next_move = 0;
}
void move_servo_1(void)
{
	if(cbcservo[1]->next_move)
		wait_servo(cbcservo[1], cbcservo[1]->next_position);
	cbcservo[1]->next_move = 0;
}
void move_servo_2(void)
{
	if(cbcservo[2]->next_move)
		wait_servo(cbcservo[2], cbcservo[2]->next_position);
	cbcservo[2]->next_move = 0;
}
void move_servo_3(void)
{
	if(cbcservo[3]->next_move)
		wait_servo(cbcservo[3], cbcservo[3]->next_position);
	cbcservo[3]->next_move = 0;
}
int move_servos()
{
	cbcservo[0]->process_id = start_process(move_servo_0);
	cbcservo[1]->process_id = start_process(move_servo_1);
	cbcservo[2]->process_id = start_process(move_servo_2);
	cbcservo[3]->process_id = start_process(move_servo_3);
}
#endif
