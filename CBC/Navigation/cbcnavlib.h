#ifndef __CBC_NAV_H_
#define __CBC_NAV_H_

#define PI 3.14159

struct CBC_robot
{
	float drivetrain;
	
	struct wheel
	{
		int port;
		long ticks_cycle;
		float speed;
		float wheel_diameter;
	} lwheel, rwheel;
} cbcrobot;

void wait_for_cbc()
{
    bmd(cbcrobot.rwheel.port);
    bmd(cbcrobot.lwheel.port);
}
void cbc_straight(int speed, float distance)
{
	float lticks = distance * cbcrobot.lwheel.ticks_cycle / (cbcrobot.lwheel.wheel_diameter * PI);
	float rticks = distance * cbcrobot.rwheel.ticks_cycle / (cbcrobot.rwheel.wheel_diameter * PI);
	
    mrp(cbcrobot.lwheel.port, (int)((float)speed * cbcrobot.lwheel.speed), (long)lticks);
	mrp(cbcrobot.rwheel.port, (int)((float)speed * cbcrobot.rwheel.speed), (long)rticks);
}
void cbc_arc(int speed, float radius, float theta) // 0 <--> 1000 (unitless), + | - in mm, + | - in degrees
{
	float arc_length = radius * theta * PI / 180.0;
	float ldistance = (radius - (cbcrobot.drivetrain * 0.5)) * theta * (PI / 180.0);
	float rdistance = (radius + (cbcrobot.drivetrain * 0.5)) * theta * (PI / 180.0);
	float lticks = ldistance * cbcrobot.lwheel.ticks_cycle / (cbcrobot.lwheel.wheel_diameter * PI);
	float rticks = rdistance * cbcrobot.rwheel.ticks_cycle / (cbcrobot.rwheel.wheel_diameter * PI);
	
	mrp(cbcrobot.lwheel.port, (int)((float)speed * cbcrobot.lwheel.speed * (ldistance / arc_length)), (long)lticks);
	mrp(cbcrobot.rwheel.port, (int)((float)speed * cbcrobot.rwheel.speed * (rdistance / arc_length)), (long)rticks);
}
void cbc_spin(int speed, float theta)
{
	float rdistance = cbcrobot.drivetrain * 0.5 * theta * PI / 180.0;
	float ldistance = rdistance * -1.0;
	float lticks = ldistance * cbcrobot.lwheel.ticks_cycle / (cbcrobot.lwheel.wheel_diameter * PI);
	float rticks = rdistance * cbcrobot.rwheel.ticks_cycle / (cbcrobot.rwheel.wheel_diameter * PI);
	
	mrp(cbcrobot.lwheel.port, (int)((float)speed * cbcrobot.lwheel.speed), (long)lticks);
	mrp(cbcrobot.rwheel.port, (int)((float)speed * cbcrobot.rwheel.speed), (long)rticks);
}
	
#endif
