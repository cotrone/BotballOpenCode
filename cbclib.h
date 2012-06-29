#ifndef __CBC_NAV_H__
#define __CBC_NAV_H__

#define PI 3.14159
float analog_avg(int port, int res,long latency);
// 0 <--> 7 (port), + (# of trials), + in ms
//This function averages an analog sensor using 'res' trials with 'latency" ms between each reading

float analog_pchange(int port, int res, long latency, long pause);
// 0 <--> 7 (port),  + (# of trials), + in ms, + in ms
//This function uses analog_avg to measure the proportional change between two trials

void build_lwheel(int port, long ticks_cycle, float speed_proportion, float wheel_diameter, float radial_distance);
// 0 <--> 3 (port), ~1100 (ticks), 1.0 + | - 0.25 (unitless), + in mm, + in mm
//Shortcut to build the left wheel structure to be utilized by various functions

void build_rwheel(int port, long ticks_cycle, float speed_proportion, float wheel_diameter, float radial_distance);
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

struct wheel_properties
{
    int port;
    long ticks_cycle;
    float speed_proportion;
    float wheel_diameter;
    float radial_distance;
} lwheel, rwheel;

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
void build_lwheel(int port, long ticks_cycle, float speed_proportion, float wheel_diameter, float radial_distance)
{
    lwheel.port = port;
    lwheel.ticks_cycle = ticks_cycle;
    lwheel.speed_proportion = speed_proportion;
    lwheel.wheel_diameter = wheel_diameter;
    lwheel.radial_distance = radial_distance;
}
void build_rwheel(int port, long ticks_cycle, float speed_proportion, float wheel_diameter, float radial_distance)
{
    rwheel.port = port;
    rwheel.ticks_cycle = ticks_cycle;
    rwheel.speed_proportion = speed_proportion;
    rwheel.wheel_diameter = wheel_diameter;
    rwheel.radial_distance = radial_distance;
}
int wait_movement()
{
    bmd(rwheel.port);
    bmd(lwheel.port);
    return 1;
}
int cbc_straight(int speed, float distance)
{
    float lticks = (distance * lwheel.ticks_cycle) / (lwheel.wheel_diameter * PI);
    float rticks = (distance * rwheel.ticks_cycle) / (rwheel.wheel_diameter * PI);
    float lspeed = (float)speed * lwheel.speed_proportion;
    float rspeed = (float)speed * rwheel.speed_proportion;
    
    if(rspeed > 1000 || lspeed > 1000 || rspeed < -1000 || lspeed < -1000)
    {
        printf("\nWarning! Invalid Speed\n");
        return -1;
    }
    else
    {
        mrp(lwheel.port, (int)lspeed, (long)lticks);
        mrp(rwheel.port, (int)rspeed, (long)rticks);
        return 1;
    }
}
int cbc_arc(int speed, float radius, float theta) // 0 <--> 1000 (unitless), + | - in mm, + | - in degrees
{
    float arc_length = radius * theta * (PI / 180.0);
    float ldistance = (radius - lwheel.radial_distance) * theta * (PI / 180.0);
    float rdistance = (radius + rwheel.radial_distance) * theta * (PI / 180.0);
    float lticks = (ldistance * lwheel.ticks_cycle) / (lwheel.wheel_diameter * PI);
    float rticks = (rdistance * rwheel.ticks_cycle) / (rwheel.wheel_diameter * PI);
    float lspeed = (float)speed * lwheel.speed_proportion * ldistance / arc_length;
    float rspeed = (float)speed * rwheel.speed_proportion * rdistance / arc_length;
    
    if(rspeed > 1000 || lspeed > 1000 || rspeed < -1000 || lspeed < -1000)
    {
        printf("\nWarning! Invalid Speed, Please Lower\n");
        return -1;
    }
    else if(radius < lwheel.radial_distance || radius < lwheel.radial_distance)
    {
        printf("\nWarning! Invalid Radius, Please Raise\n");
        return -1;
    }
    else
    {
        mrp(lwheel.port, (int)lspeed, (long)lticks);
        mrp(rwheel.port, (int)rspeed, (long)rticks);
        return 1;
    }
}
int cbc_spin(int speed, float theta)
{
    float ldistance = -1.0 * lwheel.radial_distance * theta * (PI / 180.0);
    float rdistance = rwheel.radial_distance * theta * (PI / 180.0);
    float lticks = (ldistance * lwheel.ticks_cycle) / (lwheel.wheel_diameter * PI);
    float rticks = (rdistance * rwheel.ticks_cycle) / (rwheel.wheel_diameter * PI);
    float lspeed = (float)speed * lwheel.speed_proportion * lwheel.radial_distance / (lwheel.radial_distance + rwheel.radial_distance);
    float rspeed = (float)speed * rwheel.speed_proportion * rwheel.radial_distance / (lwheel.radial_distance + rwheel.radial_distance);
    
    if(rspeed > 1000 || lspeed > 1000 || rspeed < -1000 || lspeed < -1000)
    {
        printf("\nWarning! Invalid Speed, Please Lower\n");
        return -1;
    }
    else
    {
        mrp(lwheel.port, (int)lspeed, (long)lticks);
        mrp(rwheel.port, (int)rspeed, (long)rticks);
        return 1;
    }

}
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
void rebuild_servo(servo properties, int tpm, long latency)
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
#endif
