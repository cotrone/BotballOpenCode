#ifndef CBC_NAV_H_INCLUDED
#define CBC_NAV_H_INCLUDED
#define PI 3.1415
/*
Create by Kevin Cotrone


This a CBC nav library intended to bring
to add a lot of basic navigation to the
CBC and to hopefully make the CBC useful
for a robot.

Last modified: 1/22/2012
*/

struct cbcRobot
{
    int rightWheel;     //Right motor port
    int leftWheel;      //Left motor port
    long wheelDist;     //Distance between center of two wheels
    long wheelDiam;     //Wheel Diameter
    long ticksPerRight;   //1100 works for most
    long ticksPerLeft;

};

struct cbcRobot robot = {0,3,118,57,1100,1100};


long distance_to_ticks_left(long d)
{
	return (robot.ticksPerLeft/(robot.wheelDiam*PI))*d;
}

long distance_to_ticks_right(long d)
{
	return (robot.ticksPerRight/(robot.wheelDiam*PI))*d;
}

void wait_for_cbc()
{
    bmd(robot.rightWheel);
    bmd(robot.leftWheel);
}
void cbc_straight(long dist, long speed)
{

    mrp(robot.rightWheel, speed, distance_to_ticks_right(dist));
    mrp(robot.leftWheel, speed, distance_to_ticks_left(dist));
}

//Rad and dist are in mm, and vel is in mm/sec
void cbc_arc_right(long rad, long ang, long vel)
{
	long dist = ((PI/180)*ang)*rad;
    float time = dist/vel;
    float angle = (PI/180)*ang;

    float wheelCirc = (robot.wheelDiam)*PI;
    //Right Motor
    long rightDist = (rad-((0.5)*robot.wheelDist))*angle;		//Radius for right wheel * radians it needs to move = right side length
    long rightVel = distance_to_ticks_right(rightDist)/time;

    //Left Motor
    long leftDist = (rad+((0.5)*robot.wheelDist))*angle;        //Radius for left wheel * radians needed to move = left side length
    long leftVel = distance_to_ticks_left(leftDist)/time;

    mrp(robot.rightWheel, rightVel, distance_to_ticks_right(rightDist));
    mrp(robot.leftWheel, leftVel, distance_to_ticks_left(leftDist));
    printf("right wheel vel: %d, dist: %d, left wheel vel: %d, left wheel dist: %d \n", rightVel, rightDist, leftVel, leftDist);    //stupid debug
}

//Rad and dist are in mm, and vel is in mm/sec

void cbc_arc_left(long radius, long ang, long vel)
{
    float radians = (PI/180)*ang;
    float dist = radians*radius;
    float time = dist/vel;

    float wheelCirc = (robot.wheelDiam)*PI;

    //Right motor
    long rightDist = (radius+((0.5)*robot.wheelDist))*radians;
    long rightVel = distance_to_ticks_right(rightDist)/time;

    //Left motor
    long leftDist = (radius-((0.5)*robot.wheelDist))*radians;
    long leftVel = distance_to_ticks_right(leftDist)/time;

    mrp(robot.leftWheel, (int)leftVel, (long)distance_to_ticks_left(leftDist));
    mrp(robot.rightWheel, (int)rightVel, (long)distance_to_ticks_right(rightDist));
}

//Spin angle in degrees and speed in mm/s
void cbc_spin(long ang, long speed)
{
    float rad = ang*(PI/180);
    long leftDist = 0;
    long rightDist = 0;

    leftDist = distance_to_ticks_left(-(robot.wheelDist/2)*rad);    //Calculate the length both wheels have to move
    rightDist = distance_to_ticks_right((robot.wheelDist/2)*rad);   //And offset left to neg so that you actually turn 90 or
                                                                    //Negative 90 etc.

    //Move the wheels to the correct distances
    mrp(robot.leftWheel, speed, leftDist);
    mrp(robot.rightWheel, speed, rightDist);

    printf("right wheel 1: %d, left wheel 2: %d\n", rightDist, leftDist);   //stupid debug code
}

#endif // CBC_NAV_H_INCLUDED
