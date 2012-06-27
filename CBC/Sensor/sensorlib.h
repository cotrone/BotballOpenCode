// Created on Wed Feb 8 2012

#ifndef __SENSORLIB_H__ // Change FILE to your file's name
#define __SENSORLIB_H__

typedef struct digital_properties
{
	int port;
	int def;
	long timeout;
} mydigital;
typedef struct analog_properties
{
	int port;
	int threshold;
	int sigma;
	long timeout;
} myanalog;

typedef mydigital *digital_sensor;
typedef myanalog *analog_sensor;
digital_sensor cbcdigital[8];
analog_sensor cbcanalog[8];

digital_sensor build_digital(int localport, int localdefault, long localtimeout)
{
	cbcdigital[localport - 8] = malloc(sizeof(struct digital_properties));
	cbcdigital[localport - 8]->port = localport;
	cbcdigital[localport - 8]->def = localdefault;
	cbcdigital[localport - 8]->timeout = localtimeout;
	return(cbcdigital[localport]);
}
analog_sensor build_analog(int localport, int localthreshold,int localsigma, long localtimeout)
{
	cbcanalog[localport] = malloc(sizeof(struct analog_properties));
	cbcanalog[localport]->port = localport;
	cbcanalog[localport]->threshold = localthreshold;
	cbcanalog[localport]->sigma = localsigma;
	cbcanalog[localport]->timeout = localtimeout;
	return(cbcanalog[localport]);
}
void wait_digital(digital_sensor properties)
{
	int i = 0;
	while(digital(properties->port) == properties->def && (i * 10) < properties->timeout)
		msleep(10L);
}
void wait_analog(analog_sensor properties)
{
	int i = 0;
	int def = analog10(properties->port);
	if(def > (properties->threshold - properties->sigma))
	while(analog10(properties->port) > (properties->threshold - properties->sigma) && (i * 10) < properties->timeout)
	{ msleep(10L); }
	if(def < (properties->threshold + properties->sigma))
	while(analog10(properties->port) < (properties->threshold + properties->sigma) && (i * 10) < properties->timeout)
	{ msleep(10L); }
}
#endif
