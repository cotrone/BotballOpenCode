
#ifndef __CREATE_HEADER_H
#define __CREATE_HEADER_H

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

#endif
