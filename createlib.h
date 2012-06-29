#ifndef __CREATELIB_H__
#define __CREATELIB_H__

#ifdef __arm__
#define serial_write_byte create_write_byte
#define serial_read create_read_block
extern struct _create_state stateOfCreate;
#else
#define CREATE_BUSY
#define CREATE_FREE
#define HIGH_BYTE(x) ((x & 0xFFFF) >> 8)
#define LOW_BYTE(x) (x & 0xFF)
#endif

#define get_high_byte HIGH_BYTE
#define get_low_byte LOW_BYTE
#define PI 3.14159

#define EVENT_WHEEL_DROP 1
#define EVENT_FRONT_WHEEL_DROP 2
#define EVENT_LEFT_WHEEL_DROP 3
#define EVENT_RIGHT_WHEEL_DROP 4
#define EVENT_BUMP 5
#define EVENT_LEFT_BUMP 6
#define EVENT_RIGHT_BUMP 7
#define EVENT_VIRTUAL_WALL 8
#define EVENT_WALL 9
#define EVENT_CLIFF 10
#define EVENT_LEFT_CLIFF 11
#define EVENT_FRONT_LEFT_CLIFF 12
#define EVENT_FRONT_RIGHT_CLIFF 13
#define EVENT_RIGHT_CLIFF 14
#define EVENT_HOME_BASE 15
#define EVENT_ADVANCE_BUTTON 16
#define EVENT_PLAY_BUTTON 17
#define EVENT_DIGITAL_INPUT(port) (18+port)
#define EVENT_OI_MODE_PASSIVE 22

struct create_script_buffer
{
	char script[100];
	int length;
} create_script;

const int rest=30;
const int a=56;
const int aS=57;
const int b=58;
const int bS =59;
const int c=60;
const int cS=61;
const int d=62;
const int dS=63;
const int e=64;
const int f=65;
const int fS=66;
const int g=67;
const int gS=68;

float speed_profile[2][15] = {
{0.112, 0.302, 0.492, 0.669, 0.805, 0.912, 0.977, 1.000, 0.977, 0.912, 0.805, 0.669, 0.492, 0.302, 0.112},
{0.977, 0.912, 0.805, 0.669, 0.492, 0.302, 0.112, 0.000, -0.112, -0.302, -0.492, -0.669, -0.805, -0.912, -0.977},
};

int create_length(long msec)//Convert ms to 64ths
{
	return((msec*64)/1000);
}
void create_load_onesong(int *gc_song)
{
	int i, length; //Counter Variables for loop iterations.

	CREATE_BUSY;
	serial_write_byte(140);
	serial_write_byte(*(gc_song++));
	length = *(gc_song);
	serial_write_byte(*(gc_song++));
	for( i = 0; i < length; i++)
	{
		serial_write_byte(*(gc_song++));
		serial_write_byte(*(gc_song++));
	}
	CREATE_FREE;
}
//Create Wait for Light Sensor, plug into create bay ports 20 and 17
void create_wait_for_light()
{
	CREATE_BUSY;
	serial_write_byte(147);
	serial_write_byte(4);
	serial_write_byte(158);
	serial_write_byte(18);
	serial_write_byte(147);
	serial_write_byte(0);
	CREATE_FREE;
}
void create_sync()
{
    char buffer[1];
    char *bptr = buffer;

    int read_count = 0;
    int count = 1;

    CREATE_BUSY;
    serial_write_byte(142);
    serial_write_byte(35);

    while(read_count < count)
    {
        #ifdef __arm__
        read_count += serial_read(buffer+read_count, count-read_count);
        #endif
    }
    CREATE_FREE;
}

//Create Wait Functions
void create_wait_theta(int angle)
{
	CREATE_BUSY;
	serial_write_byte(157);
	serial_write_byte(get_high_byte(angle));
    serial_write_byte(get_low_byte(angle));
	CREATE_FREE;
}
void create_wait_length(int distance)
{
	CREATE_BUSY;
	serial_write_byte(156);
	serial_write_byte(get_high_byte(distance));
    serial_write_byte(get_low_byte(distance));
	CREATE_FREE;
}
void create_wait_duration(int dseconds)
{
	CREATE_BUSY;
	serial_write_byte(155);
	serial_write_byte(get_high_byte(dseconds));
    serial_write_byte(get_low_byte(dseconds));
	CREATE_FREE;
}
void create_wait_sensor(int packet_id)
{
	CREATE_BUSY;
	serial_write_byte(158);
	serial_write_byte(packet_id);
	CREATE_FREE;
}
//Create Drive Functions, Same as KIPR libraries

void create_direct(int rspeed, int lspeed)
{
	CREATE_BUSY;
	serial_write_byte(145);
	serial_write_byte(get_high_byte(rspeed));
	serial_write_byte(get_low_byte(rspeed));
	serial_write_byte(get_high_byte(lspeed));
	serial_write_byte(get_low_byte(lspeed));
	CREATE_FREE;
}
void create_straight(int speed)
{
	CREATE_BUSY;
	serial_write_byte(145);
	serial_write_byte(get_high_byte(speed));
	serial_write_byte(get_low_byte(speed));
	serial_write_byte(get_high_byte(speed));
	serial_write_byte(get_low_byte(speed));
	CREATE_FREE;
}
void create_arc(int speed, int radius)
{
	CREATE_BUSY;
    serial_write_byte(137);
	serial_write_byte(get_high_byte(speed));
	serial_write_byte(get_low_byte(speed));
	serial_write_byte(get_high_byte(radius));
	serial_write_byte(get_low_byte(radius));
	CREATE_FREE;
}
void create_spin(int omega)
{
    serial_write_byte(137);
    serial_write_byte(get_high_byte(omega));
	serial_write_byte(get_low_byte(omega));
    if(omega < 0)
    {
        serial_write_byte(255);
        serial_write_byte(255);
    }
    if(omega > 0)
    {
        serial_write_byte(0);
        serial_write_byte(1);
    }
}
//Smart Drive Functions, Speed always positive
void create_drive_arc(unsigned int speed, int radius, float angle)
{
	CREATE_BUSY;
	serial_write_byte(137);
	if((radius * angle) < 0)
	{
		serial_write_byte(get_high_byte(-1 * speed));
		serial_write_byte(get_low_byte(-1 * speed));
	}
	else
	{
		serial_write_byte(get_high_byte(speed));
		serial_write_byte(get_low_byte(speed));
	}
	serial_write_byte(get_high_byte(radius));
	serial_write_byte(get_low_byte(radius));
	CREATE_FREE;
	create_wait_theta(angle);
	create_stop();
	create_sync();
}
void create_drive_segment(unsigned int speed, int distance)
{
	CREATE_BUSY;
	serial_write_byte(145);
	if(distance < 0)
	{
		serial_write_byte(get_high_byte(-1 * speed));
		serial_write_byte(get_low_byte(-1 * speed));
		serial_write_byte(get_high_byte(-1 * speed));
		serial_write_byte(get_low_byte(-1 * speed));
	}
	else
	{
		serial_write_byte(get_high_byte(speed));
		serial_write_byte(get_low_byte(speed));
		serial_write_byte(get_high_byte(speed));
		serial_write_byte(get_low_byte(speed));
	}
	CREATE_FREE;
	create_wait_length(distance);
	create_stop();
	create_sync();
}
void create_spin_angle(unsigned int speed, int angle)
{
	CREATE_BUSY;
	serial_write_byte(145);
	if(angle < 0)
	{
		serial_write_byte(get_high_byte(-1 * speed));
		serial_write_byte(get_low_byte(-1 * speed));
		serial_write_byte(get_high_byte(speed));
		serial_write_byte(get_low_byte(speed));
	}
	else
	{
		serial_write_byte(get_high_byte(speed));
		serial_write_byte(get_low_byte(speed));
		serial_write_byte(get_high_byte(-1 * speed));
		serial_write_byte(get_low_byte(-1 * speed));
	}
	CREATE_FREE;
	create_wait_theta(angle);
	create_stop();
	create_sync();
}
void create_drive_touch(int rspeed, int lspeed, int rport, int lport)
{
    create_drive_direct(rspeed, lspeed);
    while(!digital(rport) || !digital(lport))
    {
        if(digital(rport))
        {
            create_drive_direct(0, lspeed);
            msleep(10);
        }
        if(digital(lport))
        {
            create_drive_direct(rspeed, 0);
            msleep(10);
        }
    }
	create_stop();
    create_sync();
}

void create_drive_bump(int vel)
{
	CREATE_BUSY;
	serial_write_byte(145);
	serial_write_byte(get_high_byte(vel));
	serial_write_byte(get_low_byte(vel));
	serial_write_byte(get_high_byte(vel));
	serial_write_byte(get_low_byte(vel));
	serial_write_byte(158);
	serial_write_byte(EVENT_BUMP);
	CREATE_FREE;
}
//Create Acceleration
void create_accel_straight(int profile, float max_velocity, float distance)
{
	int i;
    float interval;

    interval = (PI * distance) / (30.0 * max_velocity);
    for( i = 0; i < 15; i++)
    {
        create_straight((int)(max_velocity * speed_profile[profile][i]));
        sleep(interval);
    }
}
void create_accel_arc(int profile, float max_velocity, float radius, float angle)
{
	int i;
    float interval;

    interval = (PI * angle * radius * PI) / (5400.0 * max_velocity);
    for(i = 0; i < 15; i++)
    {
        create_arc((int)(max_velocity * speed_profile[profile][i]), (int)radius);
        sleep(interval);
    }
}
void create_accel_spin(int profile, float max_omega, float angle)
{
	int i;
    float interval;

    interval = (PI * angle) / (30.0 * max_omega);
    for( i = 0; i < 15; i++)
    {
        create_spin((int)(max_omega * speed_profile[profile][i]));
        sleep(interval);
    }
}
void create_translate(float x, float y, unsigned int speed)
{
    float phi = abs(atan((float)y / (float)x));
    float theta = abs((PI / 2.0) - phi);
    float length = sqrt((x * x + y * y) / 4.0);
    float radius = length * sin(phi) / sin(theta);
	phi *= (180.0 / PI);
	theta *= (180.0 / PI);
    if(x > 0)
    {
        if(y > 0)
        {
            create_drive_arc(speed, (int)(radius * -1.0), (int)(theta * -1.0));
            create_drive_arc(speed, (int)radius, (int)theta);
        }
        if(y < 0)
        {
            create_drive_arc(speed, (int)(radius * -1.0), (int)theta);
            create_drive_arc(speed, (int)radius, (int)(theta * -1.0));
        }
    }
    if(x < 0)
    {
        if(y > 0)
        {
            create_drive_arc(speed, (int)radius, (int)theta);
            create_drive_arc(speed, (int)(radius * -1.0), (int)(theta * -1.0));
        }
        if(y < 0)
        {
            create_drive_arc(speed, (int)radius, (int)(theta * -1.0));
            create_drive_arc(speed, (int)(radius * -1.0), (int)theta);
        }
    }
}
//Create Scripting
void script_write_byte(char byte)
{
	if(create_script.length < 100)
	{
		create_script.script[create_script.length++] = byte;
	}
	else
	{
		printf("\nError: Script Can Not Excede 100 Bytes\n");
	}
}
void load_script()
{

	int i;
	create_connect();
	printf("CHECK POWER LED\n");
	printf("If GREEN, Restart Program\n");
	printf("If YELLOW, Restart Program\n");
	printf("If ORANGE, Press A\n");

	while(a_button()) msleep(20);//wait for the a button to not be pressed to be pressed
	while(!a_button()) msleep(20);//wait for the a button to not be pressed

	printf("Create connected, loading script");

	serial_write_byte(152);
	serial_write_byte(create_script.length);
	printf(".");
	for( i = 0; i < create_script.length; i++)
	{
		serial_write_byte(create_script.script[i]);
	}
	printf(".");
	create_power_led(64,255);// 4 bytes
	printf(".\n");
    printf("Uploaded %d bytes...\nOkay to disconnect download cable!/n", create_script.length);
}
//Create Waits for Light Sensor, plug into create bay ports 20 and 17
void script_wait_for_light()
{
	script_write_byte(147);
	script_write_byte(4);
	script_write_byte(158);
	script_write_byte(18);
}
void script_reboot()
{
	script_write_byte(7);
}
void script_wait_theta(int angle)
{
	script_write_byte(157);
	script_write_byte(get_high_byte(angle));
	script_write_byte(get_low_byte(angle));
}
void script_wait_length(int distance)
{
	script_write_byte(156);
	script_write_byte(get_high_byte(distance));
	script_write_byte(get_low_byte(distance));
}
void script_wait_duration(int dseconds)
{
	script_write_byte(155);
	script_write_byte(get_high_byte(dseconds));
	script_write_byte(get_low_byte(dseconds));
}
//Packet ID's listed in the define's
void script_wait_sensor(int packet_id)
{
	script_write_byte(158);
	script_write_byte(get_high_byte(packet_id));
	script_write_byte(get_low_byte(packet_id));
}
//Create Script Drive Functions
void script_direct(int rspeed, int lspeed)
{
	script_write_byte(145);
	script_write_byte(get_high_byte(rspeed));
	script_write_byte(get_low_byte(rspeed));
	script_write_byte(get_high_byte(lspeed));
	script_write_byte(get_low_byte(lspeed));
}
void script_straight(int speed)
{
	script_write_byte(145);
	script_write_byte(get_high_byte(speed));
	script_write_byte(get_low_byte(speed));
	script_write_byte(get_high_byte(speed));
	script_write_byte(get_low_byte(speed));
}
void script_stop()
{
	script_write_byte(145);
	script_write_byte(0);
	script_write_byte(0);
	script_write_byte(0);
	script_write_byte(0);
}
void script_arc(int speed, int radius)
{
	script_write_byte(get_high_byte(speed));
	script_write_byte(get_low_byte(speed));
	script_write_byte(get_high_byte(radius));
	script_write_byte(get_low_byte(radius));
}
//Smart Drive Functions, Speed always positive
void script_drive_arc(unsigned int speed, int radius, float angle)
{
	script_write_byte(137);
	if((radius * angle) < 0)
	{
		script_write_byte(get_high_byte(-1 * speed));
		script_write_byte(get_low_byte(-1 * speed));
	}
	else
	{
		script_write_byte(get_high_byte(speed));
		script_write_byte(get_low_byte(speed));
	}
	script_write_byte(get_high_byte(radius));
	script_write_byte(get_low_byte(radius));
	script_wait_theta(angle);
}
void script_drive_segment(unsigned int speed, int distance)
{
	script_write_byte(145);
	if(distance < 0)
	{
		script_write_byte(get_high_byte(-1 * speed));
		script_write_byte(get_low_byte(-1 * speed));
		script_write_byte(get_high_byte(-1 * speed));
		script_write_byte(get_low_byte(-1 * speed));
	}
	else
	{
		script_write_byte(get_high_byte(speed));
		script_write_byte(get_low_byte(speed));
		script_write_byte(get_high_byte(speed));
		script_write_byte(get_low_byte(speed));
	}
	script_wait_length(distance);
}
void script_spin_angle(unsigned int speed, int angle)
{
	script_write_byte(145);
	if(angle < 0)
	{
		script_write_byte(get_high_byte(-1 * speed));
		script_write_byte(get_low_byte(-1 * speed));
		script_write_byte(get_high_byte(speed));
		script_write_byte(get_low_byte(speed));
	}
	else
	{
		script_write_byte(get_high_byte(speed));
		script_write_byte(get_low_byte(speed));
		script_write_byte(get_high_byte(-1 * speed));
		script_write_byte(get_low_byte(-1 * speed));
	}
	script_wait_theta(angle);
}
void script_LSD(char b1, char b2, char b3)
{
	script_write_byte(138);
	script_write_byte(b1 + 2 * b2 + 4 * b3);
}
void script_LSDPWM(char s1, char s2, char s3)
{

	script_write_byte(144);
	script_write_byte(s1);
	script_write_byte(s2);
	script_write_byte(s3);
}
#endif
