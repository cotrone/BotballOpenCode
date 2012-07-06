#ifndef __NAVLIB_H__
#define __NAVLIB_H__

#include <stdio.h>

#define PI 3.14159
#define DEG_TO_RAD (PI / 180.0)

void build_left_wheel(int port, long ticks_cycle, float speed_proportion, float wheel_diameter, float radial_distance);
// 0 <--> 3 (port), ~1100 (ticks), 1.0 + | - 0.25 (unitless), + in mm, + in mm
//Shortcut to build the left wheel structure to be utilized by various functions

void build_left_wheel(int port, long ticks_cycle, float speed_proportion, float wheel_diameter, float radial_distance);
// 0 <--> 3 (port), ~1100 (ticks), 1.0 + | - 0.25 (unitless), + in mm, + in mm
//Shortcut to build the right wheel structure to be utilized by various functions

int cbc_straight(int speed, float distance);
// 0 <--> 1000, + | - in mm
//Moves the center of the CBC drivetrain in a straight line
//Returns 1 if executed, -1 if error is detected

int cbc_arc(int speed, float radius, float theta);
// 0 <--> 1000 (unitless), + | - in mm, + | - in degrees
//Moves the center of the CBC drivetrain in a constant radial arc
//Returns 1 if executed, -1 if error is detected

int cbc_spin(int speed, float theta);
// 0 <--> 1000, + | - in degrees
//Rotates the center of the CBC drivetrain 
//Returns 1 if executed, -1 if error is detected

struct cbc_side
{
	struct wheel_properties
	{
		int port;
		int last_requested_speed;
		long ticks_cycle;
		float speed_proportion;
		float wheel_diameter;
		float radial_distance;
	}wheel;
	struct lincolns_tophat
	{
		int port;
		int black;
		int white;
		int error;
		long timeout;
	}tophat;
	struct touch_me
	{
		int port;
		long timeout;
	}touch;
}left, right;

void build_left_wheel(int port, long ticks_cycle, float speed_proportion, float wheel_diameter, float radial_distance)
{
	left.wheel.port = port;
	left.wheel.ticks_cycle = ticks_cycle;
	left.wheel.speed_proportion = speed_proportion;
	left.wheel.wheel_diameter = wheel_diameter;
	left.wheel.radial_distance = radial_distance;
}
void build_right_wheel(int port, long ticks_cycle, float speed_proportion, float wheel_diameter, float radial_distance)
{
	right.wheel.port = port;
	right.wheel.ticks_cycle = ticks_cycle;
	right.wheel.speed_proportion = speed_proportion;
	right.wheel.wheel_diameter = wheel_diameter;
	right.wheel.radial_distance = radial_distance;
}
void build_left_tophat(int port, int white, int black, int error, long timeout)
{
	left.tophat.port = port;
	left.tophat.white = white;
	left.tophat.black = black;
	left.tophat.error = error;
	left.tophat.timeout = timeout;
}
void build_right_tophat(int port, int white, int black, int error, long timeout)
{
	right.tophat.port = port;
	right.tophat.white = white;
	right.tophat.black = black;
	right.tophat.error = error;
	right.tophat.timeout = timeout;
}
void build_left_touch(int port, long timeout)
{
	right.touch.port = port;
	right.touch.timeout = timeout;
}
void build_right_touch(int port, long timeout)
{
	right.touch.port = port;
	right.touch.timeout = timeout;
}
void cbc_wait()
{
	bmd(left.wheel.port);
	bmd(right.wheel.port);
}
void cbc_halt()
{
	off(left.wheel.port);
	off(right.wheel.port);
}
void cbc_stop()
{
	cbc_wait();
	cbc_halt();
}
int cbc_direct(int speed)
{
	float lspeed = (float)speed * left.wheel.speed_proportion;
	float rspeed = (float)speed * right.wheel.speed_proportion;
	
	mav(left.wheel.port, (int)lspeed);
	mav(right.wheel.port, (int)rspeed);
	left.wheel.last_requested_speed = (int)lspeed;
	right.wheel.last_requested_speed = (int)rspeed;
	return 1;
}
int cbc_straight(int speed, float distance)
{
	float lticks = (distance * left.wheel.ticks_cycle) / (left.wheel.wheel_diameter * PI);
	float rticks = (distance * right.wheel.ticks_cycle) / (right.wheel.wheel_diameter * PI);
	float lspeed = (float)speed * left.wheel.speed_proportion;
	float rspeed = (float)speed * right.wheel.speed_proportion;
	
	if(rspeed > 1000 || lspeed > 1000 || rspeed < -1000 || lspeed < -1000)
	{
		printf("\nWarning! Invalid Speed\n");
		return -1;
	}
	else
	{
		mrp(left.wheel.port, (int)lspeed, (long)lticks);
		mrp(right.wheel.port, (int)rspeed, (long)rticks);
		cbc_wait();
		left.wheel.last_requested_speed = (int)lspeed;
		right.wheel.last_requested_speed = (int)rspeed;
		return 1;
	}
}
int cbc_arc(int speed, float radius, float theta) // 0 <--> 1000 (unitless), + | - in mm, + | - in degrees
{
	float arc_length = radius * theta * DEG_TO_RAD;
	float ldistance = (radius - left.wheel.radial_distance) * theta * DEG_TO_RAD;
	float rdistance = (radius + right.wheel.radial_distance) * theta * DEG_TO_RAD;
	float lticks = (ldistance * left.wheel.ticks_cycle) / (left.wheel.wheel_diameter * PI);
	float rticks = (rdistance * right.wheel.ticks_cycle) / (right.wheel.wheel_diameter * PI);
	float lspeed = (float)speed * left.wheel.speed_proportion * ldistance / arc_length;
	float rspeed = (float)speed * right.wheel.speed_proportion * rdistance / arc_length;
	
	if(rspeed > 1000 || lspeed > 1000 || rspeed < -1000 || lspeed < -1000)
	{
		printf("\nWarning! Invalid Speed, Please Lower\n");
		return -1;
	}
	else if(radius < left.wheel.radial_distance || radius < right.wheel.radial_distance)
	{
		printf("\nWarning! Invalid Radius, Please Raise\n");
		return -1;
	}
	else
	{
		mrp(left.wheel.port, (int)lspeed, (long)lticks);
		mrp(right.wheel.port, (int)rspeed, (long)rticks);
		cbc_wait();
		left.wheel.last_requested_speed = (int)lspeed;
		right.wheel.last_requested_speed = (int)rspeed;
		return 1;
	}
}
int cbc_spin(int speed, float theta)
{
	float ldistance = -1.0 * left.wheel.radial_distance * theta * DEG_TO_RAD;
	float rdistance = right.wheel.radial_distance * theta * DEG_TO_RAD;
	float lticks = (ldistance * left.wheel.ticks_cycle) / (left.wheel.wheel_diameter * PI);
	float rticks = (rdistance * right.wheel.ticks_cycle) / (right.wheel.wheel_diameter * PI);
	float lspeed = (float)speed * left.wheel.speed_proportion * left.wheel.radial_distance / (left.wheel.radial_distance + left.wheel.radial_distance);
	float rspeed = (float)speed * right.wheel.speed_proportion * right.wheel.radial_distance / (right.wheel.radial_distance + right.wheel.radial_distance);
	
	if(rspeed > 1000 || lspeed > 1000 || rspeed < -1000 || lspeed < -1000)
	{
		printf("\nWarning! Invalid Speed, Please Lower\n");
		return -1;
	}
	else
	{
		mrp(left.wheel.port, (int)lspeed, (long)lticks);
		mrp(right.wheel.port, (int)rspeed, (long)rticks);
		cbc_wait();
		left.wheel.last_requested_speed = (int)lspeed;
		right.wheel.last_requested_speed = (int)rspeed;
		return 1;
	}
	
}
int cbc_align_touch()
{
	long ltimeout = left.touch.timeout;
	long rtimeout = right.touch.timeout;
	while(ltimeout > 0 || rtimeout > 0)
	{
		if(digital(left.touch.port) == 1 && digital(right.touch.port) == 0)
		{
			mav(left.wheel.port, left.wheel.last_requested_speed / 10);
			mav(right.wheel.port, right.wheel.last_requested_speed);
			msleep(10L);
		}
		else if(digital(left.touch.port) == 0 && digital(right.touch.port) == 1)
		{
			mav(left.wheel.port, left.wheel.last_requested_speed);
			mav(right.wheel.port, right.wheel.last_requested_speed / 10);
			msleep(10L);
		}
		else if(digital(left.touch.port) == 0 && digital(right.touch.port) == 0)
		{
			mav(left.wheel.port, left.wheel.last_requested_speed);
			mav(right.wheel.port, right.wheel.last_requested_speed);
			msleep(10L);
		}
		else if(digital(left.touch.port) == 1 && digital(right.touch.port) == 1)
		{
			break;
		}
		ltimeout -= 10L;
		rtimeout -= 10L;
	}
	cbc_halt();
}
int cbc_align_white()
{
	int mask = (1 << left.tophat.port) + (1 << right.tophat.port);
	long ltimeout = left.touch.timeout;
	long rtimeout = right.touch.timeout;
	set_analog_floats(mask);
	while(ltimeout > 0 || rtimeout > 0)
	{
		if(analog10(left.tophat.port) < (left.tophat.white + left.tophat.error) && analog10(right.tophat.port) > (right.tophat.white + right.tophat.error))
		{
			mav(left.wheel.port, 0);
			mav(right.wheel.port, right.wheel.last_requested_speed);
			msleep(10L);
		}
		else if(analog10(left.tophat.port) > (left.tophat.white + left.tophat.error) && analog10(right.tophat.port) < (right.tophat.white + right.tophat.error))
		{
			mav(left.wheel.port, left.wheel.last_requested_speed);
			mav(right.wheel.port, 0);
			msleep(10L);
		}
		else if(analog10(left.tophat.port) > (left.tophat.white + left.tophat.error) && analog10(right.tophat.port) > (right.tophat.white + right.tophat.error))
		{
			mav(left.wheel.port, left.wheel.last_requested_speed);
			mav(right.wheel.port, right.wheel.last_requested_speed);
			msleep(10L);
		}
		else if(analog10(left.tophat.port) > (left.tophat.white + left.tophat.error) && analog10(right.tophat.port) > (right.tophat.white + right.tophat.error))
		{
			break;
		}
		ltimeout -= 10L;
		rtimeout -= 10L;
	}
	cbc_halt();
}
int cbc_align_black()
{
	int mask = (1 << left.tophat.port) + (1 << right.tophat.port);
	long ltimeout = left.touch.timeout;
	long rtimeout = right.touch.timeout;
	set_analog_floats(mask);
	while(ltimeout > 0 || rtimeout > 0)
	{
		if(analog10(left.tophat.port) > (left.tophat.black - left.tophat.error) && analog10(right.tophat.port) < (right.tophat.black - right.tophat.error))
		{
			mav(left.wheel.port, 0);
			mav(right.wheel.port, right.wheel.last_requested_speed);
			msleep(10L);
		}
		else if(analog10(left.tophat.port) < (left.tophat.black - left.tophat.error) && analog10(right.tophat.port) > (right.tophat.black - right.tophat.error))
		{
			mav(left.wheel.port, left.wheel.last_requested_speed);
			mav(right.wheel.port, 0);
			msleep(10L);
		}
		else if(analog10(left.tophat.port) < (left.tophat.black - left.tophat.error) && analog10(right.tophat.port) < (right.tophat.black - right.tophat.error))
		{
			mav(left.wheel.port, left.wheel.last_requested_speed);
			mav(right.wheel.port, right.wheel.last_requested_speed);
			msleep(10L);
		}
		else if(analog10(left.tophat.port) > (left.tophat.black - left.tophat.error) && analog10(right.tophat.port) > (right.tophat.black - right.tophat.error))
		{
			break;
		}
		ltimeout -= 10L;
		rtimeout -= 10L;
	}
	cbc_halt();
}
#endif

