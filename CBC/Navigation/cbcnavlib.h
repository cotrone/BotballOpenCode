#ifndef __CBC_NAV_H_
#define __CBC_NAV_H_

#define PI 3.14159

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

int cbc_arc(int speed, float radius, float theta)
// 0 <--> 1000 (unitless), + | - in mm, + | - in degrees
//Moves the center of the CBC drivetrain in a constant radial arc
//Returns 1 if executed, -1 if error is detected

int cbc_spin(int speed, float theta);
// 0 <--> 1000, + | - in degrees
//Rotates the center of the CBC drivetrain 
//Returns 1 if executed, -1 if error is detected

struct wheel
{
    int port;
    long ticks_cycle;
    float speed_proportion;
    float wheel_diameter;
    float radial_distance;
} lwheel, rwheel;

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
    float lspeed = (float)speed * lwheel.speed_proportion * ldistance / arc_length;
    float rspeed = (float)speed * rwheel.speed_proportion * rdistance / arc_length;
    
    if(rspeed > 1000 || lspeed > 1000 || rspeed < 0 || lspeed < 0)
    {
        printf("\nWarning! Invalid Speed\n")
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
    
    if(rspeed > 1000 || lspeed > 1000 || rspeed < 0 || lspeed < 0)
    {
        printf("\nWarning! Invalid Speed\n")
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
	return(cbc_arc(speed, 0.0, theta));
}
#endif
