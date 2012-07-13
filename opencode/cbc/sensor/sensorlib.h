#ifndef __SENSORLIB_H__
#define __SENSORLIB_H__

//Structured Memory Initialization
struct analog_sensor_properties
{
	int port;
	int is_float;
} cbcanalog[8];
struct analog_event_properties
{
	int value;
	int error;
};

typedef struct analog_sensor_properties *analog_sensor;
typedef struct analog_event_properties *analog_event;

//Prototyped Functions
analog_sensor build_analog_sensor(int port, int is_float);
analog_event build_analog_event(int value, int error);
float analog_average(analog_sensor);
int read_analog(analog_sensor sensor, analog_event event);
int wait_analog(analog_sensor sensor_properties, analog_event event_properties);

//Implemented Functions
analog_sensor build_analog_sensor(int port, int is_float)
{
	int i;
	int mask = 0;
	cbcanalog[port].port = port;
	cbcanalog[port].is_float = is_float;
	for(i = 0; i <= 7; i++)
	{
		if(cbcanalog[i].is_float == 1)
		{
			mask += 1 << i;
		}
		else if(cbcanalog[i].is_float != 0)
		{
			printf("Invalid set_float value for Analog: %d\nPlease set to 0 or 1\n", i);
		}
		
		printf("%d   %d\n", i, mask);	
	}
	set_analog_floats(mask);
	return(&cbcanalog[port]);
}
analog_event build_analog_event(int value, int error)
{
	analog_event this_event = malloc(sizeof(struct analog_event_properties));
	
	this_event->value = value;
	this_event->error = error;
	return(this_event);
}
float analog_average(analog_sensor sensor, int trials, long refresh)
{
	int i;
	float sum = 0.0;
	for(i = 0; i < trials; i++)
	{
		sum += (float)analog10(sensor->port);
		msleep(refresh);
	}
	return(sum / (float)trials);
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
float analog_change_rate(analog_sensor sensor, int trials, int refresh)
{
    float val[2], time[2];
	val[0] = analog_average(sensor, trials, refresh);
	time[0] = seconds();
	val[1] = analog_average(sensor, trials, refresh)
	time[1] = seconds();
	
	return((val[1] - val[1]) / (1000.0 * (time[1] - time[0])));
}

#endif
