/*----------------------------------------------
Decoding of NEC Remote Control Protocol
IR-Receiver 38 kHz - on Ext Interrupt Pin (PD.2)

            Requiments:
          - 12 bytes RAM
          - 550 bytes Flash   
          - Timer0, 8 bit, Overflow Interrupt 
----------------------------------------------*/
#include "remote_control.h"

#define PRESCALER_1024 (1<<CS02) | (1<<CS00) 
// for Timer0 : 8 MHz : 1 tic = 128 us, overflow = 32,7 ms

#define Btn_UP     0x01
#define Btn_LEFT   0x02
#define Btn_RIGHT  0x03
#define Btn_DOWN   0x04
#define Btn_CENTER 0x05
#define Btn_MENU   0x06 
#define Btn_PLAY   0x07
#define RC_Short_Repeat_Counter_Limit 2  // * 100 ms - Interval between repeating commands
#define RC_Long_Repeat_Counter_Limit 10  // * 100 ms - Interval before repeat 

volatile unsigned char Global_Variable;

unsigned char RC_No_Signal_Counter = 0;            // Count of 32ms intervals without any signal
unsigned char RC_Void_Command_Flag = 0;         // trick for Apple Remote
unsigned char RC_Received_Bits_Counter = 0;
unsigned char RC_Received_1Byte_Buffer = 0;
unsigned char RC_Repeat_Counter_Limit = 0;        // Number of "Repeat signals" to skip while repeating 
unsigned char RC_Repeat_Counter = 0;            // Current number of "Repeat signals" (110 ms)
unsigned char RC_Current_Command = 0;
unsigned char RC_Received_Bytes[4] = {0,0,0,0};
unsigned char RC_Received_Bytes_Counter =0;

void RC_Init (void)
{
    DDRD &= ~(1<<2);                    // Prepare PIN
    PORTD |= (1<<2);
    GICR |= (1<<INT0);                  // External interrupt
    MCUCR = (1<<ISC00);                 // Any change on INT0
    TCNT0 = 0;                                                                          
    TIMSK |= (1<<TOIE0);                // Timer0 overflow interrupt
    TCCR0 = PRESCALER_1024;          
};                                                

void RC_Action(unsigned char RC_Command)
{    
	if (RC_Command == Btn_UP)
	{
		Global_Variable++;
	};
	if (RC_Command == Btn_DOWN)
	{
		Global_Variable--;
	};
	RC_Command = 0;
	RC_Current_Command = 0;
};

ISR (INT0_vect)     // External Interrupt    
{  
    unsigned char RC_Interval;                // Length of interval       

    RC_Interval = TCNT0;                    // Take Length of the last Interval

    TCCR0 = PRESCALER_1024; 
    TCNT0 = 0;                                 // Reset Timer0

    RC_No_Signal_Counter = 0;    

    if (PIND & (1<<2)) // If 1 on input pin
        {
            if ((RC_Interval > 60) && (RC_Interval < 80))     // 9 ms - new sequence coming (command or repeat)  
                {      
                };          
        }              
    else    // If 0 on input pin
        {   
            if ((RC_Interval > 3) && (RC_Interval < 6))   // 0.56 ms - add 0 to buffer
                {
                    RC_Received_1Byte_Buffer = (RC_Received_1Byte_Buffer >> 1);
                    RC_Received_Bits_Counter++;              
                };  
            if ((RC_Interval > 11) && (RC_Interval < 15)) // 1.69 ms - add 1 to buffer  
                {  
                    RC_Received_1Byte_Buffer = (RC_Received_1Byte_Buffer >> 1) | 0x80;
                    RC_Received_Bits_Counter++;
                };
            if (((RC_Interval > 30) && (RC_Interval < 40))) // 4,5 ms - absolutely new command signal
                {                                            // Reset all counters!    
                    RC_Repeat_Counter = 0;    
                    RC_Repeat_Counter_Limit = RC_Long_Repeat_Counter_Limit;            
                    RC_Received_1Byte_Buffer = 0;  
                    RC_Received_Bits_Counter = 0;
                    RC_Received_Bytes_Counter = 0;  
                    RC_Received_Bytes[0] = 0;
                    RC_Received_Bytes[1] = 0;
                    RC_Received_Bytes[2] = 0;
                    RC_Received_Bytes[3] = 0;
                };
            if (((RC_Interval > 16) && (RC_Interval < 21)))       // 2.25 ms - repeat signal 
                {   
                    RC_Void_Command_Flag = 0;
                };
        }; 

    if (RC_Received_Bits_Counter == 8)            // 1 Byte received
        {
            RC_Received_Bits_Counter = 0;
            RC_Received_Bytes[RC_Received_Bytes_Counter] = RC_Received_1Byte_Buffer;
            RC_Received_Bytes_Counter++;
            RC_Received_1Byte_Buffer = 0;

            if (RC_Received_Bytes_Counter == 4)      // 
                {
                    if ((RC_Received_Bytes[0] == 0xEE) && (RC_Received_Bytes[1] == 0x87)) 
                        { // Received correct Apple command    : EE 87 XX 17
                            RC_Void_Command_Flag = 0; // trick for Apple Remote
                            switch (RC_Received_Bytes[2]) 
                                {
                                    case 0x0B : RC_Current_Command = Btn_UP; break;
                                    case 0x08 : RC_Current_Command = Btn_LEFT; break;
                                    case 0x07 : RC_Current_Command = Btn_RIGHT; break;
                                    case 0x0D : RC_Current_Command = Btn_DOWN; break;
                                    case 0x02 : RC_Current_Command = Btn_MENU; break;
                                    case 0x5D : RC_Current_Command = Btn_CENTER; break;
                                    case 0x5E : RC_Current_Command = Btn_PLAY; break;
                                    case 0x04 : RC_Void_Command_Flag = 1; break; // trick for Apple Remote
                                      default : RC_Current_Command = 0x00;
                                };
                        }
                    else                        // Received incorrect command
                        {
                            RC_Current_Command = 0x00;    
                        };
                };
        };
};

ISR (TIMER0_OVF_vect) // 32 ms without any signal
{    
    RC_No_Signal_Counter++;
    if (RC_No_Signal_Counter == 1)    // 32 MS WITHOUT ANY SIGNAL //
        {
            GICR &= ~(1<<INT0);                 // Disable External Interrupts
            if (RC_Current_Command != 0)
                {    
                    if ((RC_Repeat_Counter == 0) && (!RC_Void_Command_Flag))
                        {
                            RC_Action(RC_Current_Command);    // RUN COMMAND
                        };
                    RC_Repeat_Counter++;
                    RC_Void_Command_Flag = 0;
                    if (RC_Repeat_Counter == RC_Repeat_Counter_Limit)  // Short repeat
                        {
                            RC_Repeat_Counter = 0;
                            RC_Repeat_Counter_Limit = RC_Short_Repeat_Counter_Limit;
                        };
                };
            GICR |= (1<<INT0);                // Enable External Interrupts
        }
    if (RC_No_Signal_Counter == 5)        // 160 MS WITHOUT ANY SIGNAL
        {
            TCCR0 = 0;            // Stop Timer0    
            RC_Current_Command = 0x00;
        };
};
