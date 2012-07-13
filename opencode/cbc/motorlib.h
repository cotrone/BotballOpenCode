#ifndef __MOTORLIB_H__
#define __MOTORLIB_H__

struct motor_attributes
{
    int port;
    int speed;
    long tick_cyle;
};
typdef struct motor_attributes *motor;
struct motor_position_attributes(int rel_pos, int speed, 
motor build_motor(int port, long tick_cycle)
{
    motor motor_prop = malloc(sizeof(struct motor_attributes));
    motor_prop->port = port;
    motor_prop->speed = speed;
    motor_prop->tick_cycle = tick_cycle;
    return(motor_prop);
}


#endif
