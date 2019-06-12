/*
 * Team Id:HC#145
 * Author List: Pratap
 * Filename: MOTION.h
 * Theme: Homcoming(HC)
 * Functions: motion_pin_config,timer5_init,velocity,motion_set,forward,back,left,right,
soft_left,soft_right,soft_left_2,soft_right_2,stop,left_encoder_pin_config,right_encoder_pin_config
,left_position_encoder_interrupt_init,right_position_encoder_interrupt_init,ISR,angle_rotate,
linear_distance_mm,forward_mm,back_mm,left_degrees,right_degrees,soft_left_degrees,soft_right_degrees,soft_left_2_degrees
,soft_right_2_degrees
 * Global Variables: NONE
 */


#ifndef MOTION_H_
#define MOTION_H_

void motion_pin_config (void)
{
    DDRA = DDRA | 0x0F;
    PORTA = PORTA & 0xF0;
    DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
    PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}
// Timer 5 initialized in PWM mode for velocity control
// Prescale:256
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:225.000Hz
void timer5_init()
{
    TCCR5B = 0x00;  //Stop
    TCNT5H = 0xFF;  //Counter higher 8-bit value to which OCR5xH value is compared with
    TCNT5L = 0x01;  //Counter lower 8-bit value to which OCR5xH value is compared with
    OCR5AH = 0x00;  //Output compare register high value for Left Motor
    OCR5AL = 0xFF;  //Output compare register low value for Left Motor
    OCR5BH = 0x00;  //Output compare register high value for Right Motor
    OCR5BL = 0xFF;  //Output compare register low value for Right Motor
    OCR5CH = 0x00;  //Output compare register high value for Motor C1
    OCR5CL = 0xFF;  //Output compare register low value for Motor C1
    TCCR5A = 0xA9;  /*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
                      For Overriding normal port functionality to OCRnA outputs.
                      {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/
    
    TCCR5B = 0x0B;  //WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

void velocity (unsigned char left_motor, unsigned char right_motor)
{
    OCR5AL = (unsigned char)left_motor;
    OCR5BL = (unsigned char)right_motor;
}

//Function used for setting motor's direction
void motion_set (unsigned char Direction)
{
    unsigned char PortARestore = 0;

    Direction &= 0x0F;          // removing upper nibbel as it is not needed
    PortARestore = PORTA;           // reading the PORTA's original status
    PortARestore &= 0xF0;           // setting lower direction nibbel to 0
    PortARestore |= Direction;  // adding lower nibbel for direction command and restoring the PORTA status
    PORTA = PortARestore;           // setting the command to the port
}

void forward (void) //both wheels forward
{
    motion_set(0x06);
}

void back (void) //both wheels backward
{
    motion_set(0x09);
}

void left (void) //Left wheel backward, Right wheel forward
{
    motion_set(0x05);
}

void right (void) //Left wheel forward, Right wheel backward
{
    motion_set(0x0A);
}

void soft_left (void) //Left wheel stationary, Right wheel forward
{
    motion_set(0x04);
}

void soft_right (void) //Left wheel forward, Right wheel is stationary
{
    motion_set(0x02);
}

void soft_left_2 (void) //Left wheel backward, right wheel stationary
{
    motion_set(0x01);
}

void soft_right_2 (void) //Left wheel stationary, Right wheel backward
{
    motion_set(0x08);
}

void stop (void)
{
    motion_set(0x00);
}
//Function to configure INT4 (PORTE 4) pin as input for the left position encoder
void left_encoder_pin_config(void)
{
    DDRE  = DDRE & 0xEF;  //Set the direction of the PORTE 4 pin as input
    PORTE = PORTE | 0x10; //Enable internal pull-up for PORTE 4 pin
}

//Function to configure INT5 (PORTE 5) pin as input for the right position encoder
void right_encoder_pin_config (void)
{
    DDRE  = DDRE & 0xDF;  //Set the direction of the PORTE 4 pin as input
    PORTE = PORTE | 0x20; //Enable internal pull-up for PORTE 4 pin
}
void left_position_encoder_interrupt_init (void) //Interrupt 4 enable
{
    cli(); //Clears the global interrupt
    EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge
    EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder
    sei();   // Enables the global interrupt
}

void right_position_encoder_interrupt_init (void) //Interrupt 5 enable
{
    cli(); //Clears the global interrupt
    EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge
    EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for right position encoder
    sei();   // Enables the global interrupt
}

//ISR for right position encoder
ISR(INT5_vect)
{
    ShaftCountRight++;  //increment right shaft position count
}


//ISR for left position encoder
ISR(INT4_vect)
{
    ShaftCountLeft++;  //increment left shaft position count
}
void angle_rotate(unsigned int Degrees)
{
    float ReqdShaftCount = 0;
    unsigned long int ReqdShaftCountInt = 0;

    ReqdShaftCount = (float) Degrees/ 4.090; // division by resolution to get shaft count
    ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
    ShaftCountRight = 0;
    ShaftCountLeft = 0;

    while (1)
    {
        if((ShaftCountRight >= ReqdShaftCountInt) | (ShaftCountLeft >= ReqdShaftCountInt))
        break;
    }
    stop(); //Stop robot
}

//Function used for moving robot forward by specified distance

void linear_distance_mm(unsigned int DistanceInMM)
{
    float ReqdShaftCount = 0;
    unsigned long int ReqdShaftCountInt = 0;

    ReqdShaftCount = DistanceInMM / 5.338; // division by resolution to get shaft count
    ReqdShaftCountInt = (unsigned long int) ReqdShaftCount;
    
    ShaftCountRight = 0;
    while(1)
    {
        if(ShaftCountRight > ReqdShaftCountInt)
        {
            break;
        }
    }
    stop(); //Stop robot
}

void forward_mm(unsigned int DistanceInMM)
{
    forward();
    linear_distance_mm(DistanceInMM);
}

void back_mm(unsigned int DistanceInMM)
{
    back();
    linear_distance_mm(DistanceInMM);
}

void left_degrees(unsigned int Degrees)
{
    // 87 pulses for 360 degrees rotation 4.090 degrees per count
    left(); //Turn left
    angle_rotate(Degrees);
}



void right_degrees(unsigned int Degrees)
{
    // 87 pulses for 360 degrees rotation 4.090 degrees per count
    right(); //Turn right
    angle_rotate(Degrees);
}


void soft_left_degrees(unsigned int Degrees)
{
    // 176 pulses for 360 degrees rotation 2.045 degrees per count
    soft_left(); //Turn soft left
    Degrees=Degrees*2;
    angle_rotate(Degrees);
}

void soft_right_degrees(unsigned int Degrees)
{
    // 176 pulses for 360 degrees rotation 2.045 degrees per count
    soft_right();  //Turn soft right
    Degrees=Degrees*2;
    angle_rotate(Degrees);
}

void soft_left_2_degrees(unsigned int Degrees)
{
    // 176 pulses for 360 degrees rotation 2.045 degrees per count
    soft_left_2(); //Turn reverse soft left
    Degrees=Degrees*2;
    angle_rotate(Degrees);
}

void soft_right_2_degrees(unsigned int Degrees)
{
    // 176 pulses for 360 degrees rotation 2.045 degrees per count
    soft_right_2();  //Turn reverse soft right
    Degrees=Degrees*2;
    angle_rotate(Degrees);
}


/*
 * Function Name:init_motion
 * Input: None
 * Output: None
 * Logic: initializes the motion port and pins
 * Example Call: init_motion()
 */

void init_motion(){
    cli();
    motion_pin_config();
    left_encoder_pin_config(); //left encoder pin config
    right_encoder_pin_config(); //right encoder pin config
    left_position_encoder_interrupt_init();
    right_position_encoder_interrupt_init();
    timer5_init();
    sei();
}




#endif /* MOTION_H_ */