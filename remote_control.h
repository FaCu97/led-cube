#ifndef __REMOTE_CONTROL_H
#define __REMOTE_CONTROL_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define BTN_UP     0x01
#define BTN_LEFT   0x02
#define BTN_RIGHT  0x03
#define BTN_DOWN   0x04
#define BTN_CENTER 0x05
#define BTN_MENU   0x06 
#define BTN_PLAY   0x07

void RC_Init(void);
uint8_t RC_Get_Command(void);

#endif
