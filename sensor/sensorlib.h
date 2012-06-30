#ifndef __SENSORLIB_H__
#define __SENSORLIB_H__

//Structured Memory Initialization
struct analog_sensor_properties
{
	int port;
	int is_float;
	long refresh;
}cbcanalog[8];
struct analog_event_properties
{
	int value;
	int error;
	float change_rate;
};

typedef struct analog_sensor_properties *analog_sensor;
typedef struct analog_event_properties *analog_event;

//Prototyped Functions
analog_sensor build_analog_sensor(int port, int is_float, long refresh);
analog_event build_analog_event(int value, int error, float change_rate);
int wait_analog(analog_sensor sensor_properties, analog_event event_properties);
int analog_average(analog_sensor);

//Implemented Functions
analog_sensor build_analog_sensor(int port, int is_float, long refresh)
{
	cbcanalog[port].port = port;
	cbcanalog[port].is_float = is_float;
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
#endif
