// Created on Sat Feb 4 2012

#ifndef __CREATE_ACCEL_H__
#define __CREATE_ACCEL_H__

#include "create_header.h"
#include "create_drive.h"

const float speed_profile[15] = 
{0.112, 0.302, 0.492, 0.669, 0.805, 0.912, 0.977, 1.000, 0.977, 0.912, 0.805, 0.669, 0.492, 0.302, 0.112}; 
void create_accel_straight(float max_velocity, float distance)
{
	int i;
    float interval;
    
    interval = (PI * distance) / (30.0 * max_speed)
    for( i = 0; i < 15; i++)
    {
        create_straight((int)(max_velocity * speed_profile[i]));
        sleep(interval);
    }
    create_stop();
}
void create_accel_arc(float max_velocity, float radius, float angle)
{
	int i;
    float interval;
    
    interval = (PI * angle * radius * pi) / (5400.0 * max_velocity)
    for( i = 0; i < 15; i++)
    {
        create_arc((int)(max_speed * speed_profile[i]), (int)radius);
        sleep(interval);
    }
    create_stop();
}
void create_accel_spin(float max_omega, float angle)
{
	int i;
    float interval;
    
    interval = (PI * angle) / (30.0 * max_omega)
    for( i = 0; i < 15; i++)
    {
        create_spin((int)(max_omega * speed_profile[i]));
        sleep(interval);
    }
    create_stop();
}
#endif
