// Useful Create  Functions
#ifndef __CREATE_DRIVE_H__
#define __CREATE_DRIVE_H__

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
	serial_write_byte(get_high_byte(packet_id));
    serial_write_byte(get_low_byte(packet_id));
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
    int dps;
    
    dps = omega * 1.00;
    serial_write_byte(137)
    serial_write_byte(get_high_byte(dps));
	serial_write_byte(get_low_byte(dps));
    if(speed < 0)
    {
        serial_write_byte(255);
        serial_write_byte(255);
    }
    if(speed > 0)
    {
        serial_write_byte(0);
        serial_write_byte(1);
    }
}
//Smart Drive Functions, Speed always positive
void create_drive_arc(unsigned int speed, int radius, float angle)
{
	create_wait_theta(0);
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
	create_sync();
	create_stop();
}
void create_drive_segment(unsigned int speed, int distance)
{
	create_wait_length(0);
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
	create_sync();
	create_stop();
}
void create_spin_angle(unsigned int speed, int angle)
{	
	create_wait_theta(0);
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
	create_sync();
	create_stop();
}
//Create Cliff Alignment
void create_align_front(int speed, int flcliff_TH, int frcliff_TH)
{
	int boollcliff = 1;
	int boolrcliff = 1;
	printf("\tWhite\nL: %d\tR: %d\n", get_create_lfcliff_amt(0.02), get_create_rfcliff_amt(0.02));
	while(boollcliff == 1 || boolrcliff == 1)
	{
		if(get_create_lfcliff_amt(0.02) < flcliff_TH)
			boollcliff = 0;
		if(get_create_rfcliff_amt(0.02) < frcliff_TH)
			boolrcliff = 0;
		create_direct(speed * boolrcliff, speed * boollcliff);
	}
	printf("\tBlack\nL: %d\tR: %d\n\n", get_create_lfcliff_amt(0.02), get_create_rfcliff_amt(0.02));
	create_stop();
}
void create_align_middle(int speed, int mlcliff_TH, int mrcliff_TH)
{
	int boollcliff = 1;
	int boolrcliff = 1;
	printf("\tWhite\nL: %d\tR: %d\n", get_create_lcliff_amt(0.02), get_create_rcliff_amt(0.02));
	while(boollcliff == 1 || boolrcliff == 1)
	{
		if(get_create_lcliff_amt(0.02) < mlcliff_TH)
			boollcliff = 0;
		if(get_create_rcliff_amt(0.02) < mrcliff_TH)
			boolrcliff = 0;
		create_direct(speed * boolrcliff, speed * boollcliff);
	}
	printf("\tBlack\nL: %d\tR: %d\n\n", get_create_lcliff_amt(0.02), get_create_rcliff_amt(0.02));
	create_stop();
}
void create_find_line(int speed, int flcliff_TH, int frcliff_TH)
{
	if(speed < 0)
	{
		create_direct(speed, -speed);
		while(get_create_rfcliff_amt(0.02) > frcliff_TH){}
		create_stop();
		create_direct(-speed / 3, speed);
		while(get_create_lfcliff_amt(0.02) > flcliff_TH){}
		create_stop();
	}
	if(speed > 0)
	{
		create_direct(-speed, speed);
		while(get_create_lfcliff_amt(0.02) > flcliff_TH){}
		create_stop();
		create_direct(speed, -speed / 3);
		while(get_create_rfcliff_amt(0.02) > frcliff_TH){}
		create_stop();
	}
}
void create_follow_line(int speed, int flcliff_TH, int frcliff_TH)
{
	int boollcliff = 1;
	int boolrcliff = 1;
	int lvalue, rvalue;
	
	printf("\tWhite\nL: %d\tR: %d\n", get_create_lfcliff_amt(0.02), get_create_rfcliff_amt(0.02));
	while(boollcliff == 1 || boolrcliff == 1)
	{
		lvalue = get_create_lfcliff_amt(0.02);
		if(lvalue < flcliff_TH)
			boollcliff = 0;
		else if(lvalue > flcliff_TH)
			boollcliff = 1;
		create_direct(speed * boolrcliff + speed / 2, speed * boollcliff + speed / 2);
		rvalue = get_create_lfcliff_amt(0.02);
		if(rvalue < frcliff_TH)
			boolrcliff = 0;
		else if(rvalue > frcliff_TH)
			boolrcliff = 1;
		create_direct(speed * boolrcliff + speed / 2, speed * boollcliff + speed / 2);
	}
	printf("\tBlack\nL: %d\tR: %d\n\n", get_create_lfcliff_amt(0.02), get_create_rfcliff_amt(0.02));
	create_stop();
}
#endif
