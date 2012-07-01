#ifndef __SENSORLIB_H__
#define __SENSORLIB_H__

//Structured Memory Initialization
struct analog_sensor_properties
{
	int port;
	int is_float;
	int trials;
	long refresh;
} cbcanalog[8];
struct analog_event_properties
{
	int value;
	int error;
	float change_rate;
};

typedef struct analog_sensor_properties *analog_sensor;
typedef struct analog_event_properties *analog_event;

//Prototyped Functions
analog_sensor build_analog_sensor(int port, int is_float, int trials, long refresh);
analog_event build_analog_event(int value, int error, float change_rate);
float analog_average(analog_sensor);
int read_analog(analog_sensor sensor, analog_event event);
int wait_analog(analog_sensor sensor_properties, analog_event event_properties);

//Implemented Functions
analog_sensor build_analog_sensor(int port, int is_float,int trials, long refresh)
{
	cbcanalog[port].port = port;
	cbcanalog[port].is_float = is_float;
	cbcanalog[port].trials = trials;
	cbcanalog[port].refresh = refresh;
	return(&cbcanalog[port]);
}
analog_event build_analog_event(int value, int error, float change_rate)
{
	analog_event this_event = malloc(sizeof(struct analog_event_properties));
	this_event->value = value;
	this_event->error = error;
	this_event->change_rate = change_rate;
	return(this_event);
}
float analog_average(analog_sensor sensor)
{
	int i;
	float sum = 0.0;
	for(i = 0; i < sensor->trials; i++)
	{
		sum += (float)analog10(sensor->port);
		msleep(sensor->refresh);
	}
	return(sum / sensor->trials);
}
int analog_state(analog_sensor sensor, analog_event event)
{
	int initial = analog_average(sensor);
	if(initial <= (event->value + event->error) && initial >= (event->value - event->error))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
float analog_change(analog_sensor sensor, analog_event event)
{
	float ivalue = analog_average(sensor);
	float itime = seconds();
	float fvalue = analog_average(sensor);
	float ftime = seconds();
	
	return((fvalue - ivalue) / (ftime - itime));
}
	
	
#endif
