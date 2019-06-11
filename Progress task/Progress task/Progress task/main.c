▪ /*
▪ * Team Id: HC#145
▪ * Author List: Pratap Mridha,Sujan Bag
▪ * Theme: Homecoming (HC)
▪ * Functions: buzzer_pin_config,buzzer_on,buzzer_off,init_ports,lcd_reset,lcd_init,lcd_wr_command, lcd_wr_char, lcd_line1,lcd_line2, lcd_string,lcd_port_init,lcd_set_4bit,lcd_init,lcd_wr_command,lcd_wr_char,lcd_home,lcd_string,lcd_cursor,lcd_print,display,SIGNAL,servo1_pin_config,servo2_pin_config,servo3_pin_config,timer1_init,
* *            servo_init,timer1_init,servo_1,servo_2,servo_3,servo_1_free,servo_2_free,servo_3_free,init_servo,adc_pin_config,adc_init,ADC_init,ADC_Conversion,print_sensor,return_sensor,motion_pin_config,timer5_init,velocity,motion_set,forward,back,ISR,parsestr,traverse,straight_arm_free,normal_grab_and_hold,extended_grab_and_hold,straight_arm_free,deposit,run,decode,
▪ * Global Variables: ShaftCounterLeft,ShaftCounterRight,Degrees,Left_sensors,Right_sensor,Center_sensor,pick,habitat_node,animal,N,S,W,E
▪ */

#define F_CPU 14745600

#include <avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include <string.h>
#include <math.h>
#include <ctype.h>



volatile unsigned long int ShaftCountLeft = 0; //to keep track of left position encoder
volatile unsigned long int ShaftCountRight = 0; //to keep track of right position encoder
volatile unsigned int Degrees; //to accept angle in degrees for turning




/*********************************************LCD INTERFACING SECTION***************************************************/
#define RS 0
#define RW 1
#define EN 2
#define lcd_port PORTC

#define sbit(reg,bit)	reg |= (1<<bit)			// Macro defined for Setting a bit of any register.
#define cbit(reg,bit)	reg &= ~(1<<bit)		// Macro defined for Clearing a bit of any register.

void init_ports();
void lcd_reset();
void lcd_init();
void lcd_wr_command(unsigned char);
void lcd_wr_char(char);
void lcd_line1();
void lcd_line2();
void lcd_string(char*);

unsigned int temp;
unsigned int unit;
unsigned int tens;
unsigned int hundred;
unsigned int thousand;
unsigned int million;
void buzzer_pin_config ()
{
	cli();
	DDRC=0x00;
	DDRC=DDRC |0x08;
	sei();
}
void buzzer_on(){
	PORTC= 0xff & 0x08;// dont mask(11111111 | 00001000)
}
void buzzer_off(){
	PORTC=0xff & 0xf7 ;//dont mask(11111111 & 11110111)
}
void lcd_port_init(){
 DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
 PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7

}

//Function to Reset LCD
void lcd_set_4bit()
{
	_delay_ms(1);

	cbit(lcd_port,RS);				//RS=0 --- Command Input
	cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
	lcd_port = 0x30;				//Sending 3
	sbit(lcd_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd_port,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(lcd_port,RS);				//RS=0 --- Command Input
	cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
	lcd_port = 0x30;				//Sending 3
	sbit(lcd_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd_port,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(lcd_port,RS);				//RS=0 --- Command Input
	cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
	lcd_port = 0x30;				//Sending 3
	sbit(lcd_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd_port,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(lcd_port,RS);				//RS=0 --- Command Input
	cbit(lcd_port,RW);				//RW=0 --- Writing to LCD
	lcd_port = 0x20;				//Sending 2 to initialise LCD 4-bit mode
	sbit(lcd_port,EN);				//Set Enable Pin
	_delay_ms(1);					//Delay
	cbit(lcd_port,EN);				//Clear Enable Pin


}

//Function to Initialize LCD
void lcd_init()
{
	_delay_ms(1);

	lcd_wr_command(0x28);			//LCD 4-bit mode and 2 lines.
	lcd_wr_command(0x01);
	lcd_wr_command(0x06);
	lcd_wr_command(0x0E);
	lcd_wr_command(0x80);

}

//Function to Write Command on LCD
void lcd_wr_command(unsigned char cmd)
{
	unsigned char temp;
	temp = cmd;
	temp = temp & 0xF0;
	lcd_port &= 0x0F;
	lcd_port |= temp;
	cbit(lcd_port,RS);
	cbit(lcd_port,RW);
	sbit(lcd_port,EN);
	_delay_ms(5);
	cbit(lcd_port,EN);

	cmd = cmd & 0x0F;
	cmd = cmd<<4;
	lcd_port &= 0x0F;
	lcd_port |= cmd;
	cbit(lcd_port,RS);
	cbit(lcd_port,RW);
	sbit(lcd_port,EN);
	_delay_ms(5);
	cbit(lcd_port,EN);
}

//Function to Write Data on LCD
void lcd_wr_char(char letter)
{
	char temp;
	temp = letter;
	temp = (temp & 0xF0);
	lcd_port &= 0x0F;
	lcd_port |= temp;
	sbit(lcd_port,RS);
	cbit(lcd_port,RW);
	sbit(lcd_port,EN);
	_delay_ms(5);
	cbit(lcd_port,EN);

	letter = letter & 0x0F;
	letter = letter<<4;
	lcd_port &= 0x0F;
	lcd_port |= letter;
	sbit(lcd_port,RS);
	cbit(lcd_port,RW);
	sbit(lcd_port,EN);
	_delay_ms(5);
	cbit(lcd_port,EN);
}


//Function to bring cursor at home position
void lcd_home()
{
	lcd_wr_command(0x80);
}


//Function to Print String on LCD
void lcd_string(char *input_str)
{
	while(*input_str != '\0')
	{
		lcd_wr_char(*input_str);
		input_str++;
	}
}

//Position the LCD cursor at "row", "column".

void lcd_cursor (char row, char column)
{
	switch (row) {
		case 1: lcd_wr_command (0x80 + column - 1); break;
		case 2: lcd_wr_command (0xc0 + column - 1); break;
		case 3: lcd_wr_command (0x94 + column - 1); break;
		case 4: lcd_wr_command (0xd4 + column - 1); break;
		default: break;
	}
}

//Function To Print Any input value upto the desired digit on LCD
void lcd_print (char row, char coloumn, unsigned int value, int digits)
{
	unsigned char flag=0;
	if(row==0||coloumn==0)
	{
		lcd_home();
	}
	else
	{
		lcd_cursor(row,coloumn);
	}
	if(digits==5 || flag==1)
	{
		million=value/10000+48;
		lcd_wr_char(million);
		flag=1;
	}
	if(digits==4 || flag==1)
	{
		temp = value/1000;
		thousand = temp%10 + 48;
		lcd_wr_char(thousand);
		flag=1;
	}
	if(digits==3 || flag==1)
	{
		temp = value/100;
		hundred = temp%10 + 48;
		lcd_wr_char(hundred);
		flag=1;
	}
	if(digits==2 || flag==1)
	{
		temp = value/10;
		tens = temp%10 + 48;
		lcd_wr_char(tens);
		flag=1;
	}
	if(digits==1 || flag==1)
	{
		unit = value%10 + 48;
		lcd_wr_char(unit);
	}
	if(digits>5)
	{
		lcd_wr_char('E');
	}

}

/*************************************************************************************************************************/



/*********************************************SERIAL COMMUNICATION UART2**************************************************/
unsigned char data; //to store received data from UDR2
char input_str[100]="";
void uart2_init(void)
{
	UCSR2B = 0x00; //disable while setting baud rate
	UCSR2A = 0x00;
	UCSR2C = 0x06;
	UBRR2L = 0x5F; //set baud rate lo
	UBRR2H = 0x00; //set baud rate hi
	UCSR2B = 0x98;
}
int i=0;
void display(){
	cli();
	lcd_set_4bit();
	lcd_init();

	lcd_cursor(1,1);
	for(i=0;input_str[i]!='#';i++){
	if(input_str[i]=='\n'){
	lcd_cursor(2,1);
	continue;
	}
	lcd_wr_char(input_str[i]);


	}




}



SIGNAL(USART2_RX_vect) {
	data=UDR2;// receives data 

	strcat(input_str,&data); //concats to the input_str 

	if(data=='#'){

		display();// displays the input_str on the lcd of fire bird v
	}
}

/*****************************************************************************************************************************/

/******************************************Servo section*****************************/
//Configure PORTB 5 pin for servo motor 1 operation
void servo1_pin_config (void)
{
	DDRB  = DDRB | 0x20;  //making PORTB 5 pin output
	PORTB = PORTB | 0x20; //setting PORTB 5 pin to logic 1
}

//Configure PORTB 6 pin for servo motor 2 operation
void servo2_pin_config (void)
{
	DDRB  = DDRB | 0x40;  //making PORTB 6 pin output
	PORTB = PORTB | 0x40; //setting PORTB 6 pin to logic 1
}

//Configure PORTB 7 pin for servo motor 3 operation
void servo3_pin_config (void)
{
	DDRB  = DDRB | 0x80;  //making PORTB 7 pin output
	PORTB = PORTB | 0x80; //setting PORTB 7 pin to logic 1
}
void servo_init(){
servo1_pin_config(); //Configure PORTB 5 pin for servo motor 1 operation
servo2_pin_config(); //Configure PORTB 6 pin for servo motor 2 operation
servo3_pin_config(); //Configure PORTB 7 pin for servo motor 3 operation
}

void timer1_init(void)
{
 TCCR1B = 0x00; //stop
 TCNT1H = 0xFC; //Counter high value to which OCR1xH value is to be compared with
 TCNT1L = 0x01;	//Counter low value to which OCR1xH value is to be compared with
 OCR1AH = 0x03;	//Output compare Register high value for servo 1
 OCR1AL = 0xFF;	//Output Compare Register low Value For servo 1
 OCR1BH = 0x03;	//Output compare Register high value for servo 2
 OCR1BL = 0xFF;	//Output Compare Register low Value For servo 2
 OCR1CH = 0x03;	//Output compare Register high value for servo 3
 OCR1CL = 0xFF;	//Output Compare Register low Value For servo 3
 ICR1H  = 0x03;
 ICR1L  = 0xFF;
 TCCR1A = 0xAB; /*{COM1A1=1, COM1A0=0; COM1B1=1, COM1B0=0; COM1C1=1 COM1C0=0}
 					For Overriding normal port functionality to OCRnA outputs.
				  {WGM11=1, WGM10=1} Along With WGM12 in TCCR1B for Selecting FAST PWM Mode*/
 TCCR1C = 0x00;
 TCCR1B = 0x0C; //WGM12=1; CS12=1, CS11=0, CS10=0 (Prescaler=256)
}


//Function to rotate Servo 1 by a specified angle in the multiples of 1.86 degrees
void servo_1(unsigned char degrees)
{
	float PositionPanServo = 0;
	PositionPanServo = (float)degrees ;
	OCR1AH = 0x00;
	OCR1AL = (unsigned char) PositionPanServo;
}


//Function to rotate Servo 2 by a specified angle in the multiples of 1.86 degrees
void servo_2(unsigned char degrees)
{
	float PositionTiltServo = 0;
	PositionTiltServo = (float)degrees ;
	OCR1BH = 0x00;
	OCR1BL = (unsigned char) PositionTiltServo;
}

//Function to rotate Servo 3 by a specified angle in the multiples of 1.86 degrees
void servo_3(unsigned char degrees)
{
	float PositionServo = 0;
	PositionServo = (float)degrees;
	OCR1CH = 0x00;
	OCR1CL = (unsigned char) PositionServo;
}

//servo_free functions unlocks the servo motors from the any angle
//and make them free by giving 100% duty cycle at the PWM. This function can be used to
//reduce the power consumption of the motor if it is holding load against the gravity.

void servo_1_free (void) //makes servo 1 free rotating
{
	OCR1AH = 0x03;
	OCR1AL = 0xFF; //Servo 1 off
}

void servo_2_free (void) //makes servo 2 free rotating
{
	OCR1BH = 0x03;
	OCR1BL = 0xFF; //Servo 2 off
}

void servo_3_free (void) //makes servo 3 free rotating
{
	OCR1CH = 0x03;
	OCR1CL = 0xFF; //Servo 3 off
}

//initialise the servos
void init_servo(){
	cli();
	servo_init();
	timer1_init();
	sei();
}

/*********************************************************************************/

/**************************ADC for SENSORS   ***************************/

unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;
unsigned char sharp, distance, adc_reading;
unsigned int value;
float BATT_Voltage, BATT_V;

//ADC pin configuration
void adc_pin_config (void)
{
	DDRF = 0x00; //set PORTF direction as input
	PORTF = 0x00; //set PORTF pins floating
	DDRK = 0x00; //set PORTK direction as input
	PORTK = 0x00; //set PORTK pins floating
}
void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}
void ADC_init()
{
	adc_pin_config();
	adc_init();
}

//This Function accepts the Channel Number and returns the corresponding Analog Value
unsigned char ADC_Conversion(unsigned char Ch)
{
	unsigned char a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;
	ADMUX= 0x20| Ch;
	ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for ADC conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}



// This Function prints the Analog Value Of Corresponding Channel No. at required Row
// and Coloumn Location.
void print_sensor(char row, char coloumn,unsigned char channel)
{
	ADC_Value = ADC_Conversion(channel);
	lcd_print(row, coloumn, ADC_Value, 3);
}


// This Function calculates the actual distance in millimeters(mm) from the input
// analog value of Sharp Sensor.
unsigned int Sharp_GP2D12_estimation(unsigned char adc_reading)
{
	float distance;
	unsigned int distanceInt;
	distance = (int)(10.00*(2799.6*(1.00/(pow(adc_reading,1.1546)))));
	distanceInt = (int)distance;
	if(distanceInt>800)
	{
		distanceInt=800;
	}
	return distanceInt;
}
int return_sensor(unsigned char channel)
{
	ADC_Value = ADC_Conversion(channel);
	return ADC_Value;
}
/************************************************************************/

/*****************************motion************************************/
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
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
 					  For Overriding normal port functionality to OCRnA outputs.
				  	  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/

	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
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

	Direction &= 0x0F; 			// removing upper nibbel as it is not needed
	PortARestore = PORTA; 			// reading the PORTA's original status
	PortARestore &= 0xF0; 			// setting lower direction nibbel to 0
	PortARestore |= Direction; 	// adding lower nibbel for direction command and restoring the PORTA status
	PORTA = PortARestore; 			// setting the command to the port
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
	// 88 pulses for 360 degrees rotation 4.090 degrees per count
	left(); //Turn left
	angle_rotate(Degrees);
}



void right_degrees(unsigned int Degrees)
{
	// 88 pulses for 360 degrees rotation 4.090 degrees per count
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

/***********************************************************************/


/**********************(arm)*******************/

/*Funtion name:straight_arm_free
  input: none
  output: none
  logic: makes the arm straight upward

*/

void straight_arm_free(){
	servo_1(105);
	_delay_ms(100);
	servo_2(95);
	_delay_ms(1000);



	/*servo_3(30);
	_delay_ms(200);
	servo_2(27);
	_delay_ms(1000);
	*/
	servo_3_free();
	_delay_ms(200);
	servo_2_free();
	_delay_ms(200);
	servo_1_free();



}

/*Function name: normal_grab_hold
input:no input
output:none
logic: it grabs picks up animal and grabs it
example call: by traverse function
normal_grab_and_hold()
*/

void normal_grab_and_hold(){
	servo_1(105);
	_delay_ms(100);
	servo_2(88);
	_delay_ms(200);
	servo_3(32);
	_delay_ms(1000);
	servo_2(27);
	/*servo_1(105);
	_delay_ms(100);
	servo_2(27);*/


	_delay_ms(1000);
	servo_3(63);

	_delay_ms(1000);
	servo_1(105);
	_delay_ms(100);
	servo_2(95);
	_delay_ms(1000);
	servo_1_free();
	servo_2_free();

}

/*
same as normal_grab_and_hold 
only for corners
*/
void extended_grab_and_hold(){
	servo_1(105);
	_delay_ms(100);
	servo_2(95);
	_delay_ms(200);
	servo_3(32);
	_delay_ms(500);
	servo_2(52);
	_delay_ms(200);
	servo_1(80);

	_delay_ms(1000);
	servo_3(63);
	_delay_ms(100);
	servo_2_free();
	_delay_ms(1000);
	servo_1(105);
	_delay_ms(500);
	servo_2(95);
	_delay_ms(1000);
	servo_1_free();
	servo_2_free();

}


void straight_arm(){
	servo_1(105);
	_delay_ms(100);
	servo_2(95);
	_delay_ms(1000);



	/*servo_3(30);
	_delay_ms(200);
	servo_2(27);
	_delay_ms(1000);
	*/
	servo_3_free();
	_delay_ms(200);

}
/*
*function name: deposit
			logic it releses the animals from grab;
*/
void deposit(){

	servo_1(105);
	_delay_ms(500);
	servo_2(28);
	_delay_ms(1000);
	servo_3(32);
	_delay_ms(700);


	straight_arm_free();

}

/*******************************************************************************************/
#define		THRESHOLD		95
#define		VELOCITY_MAX	155
#define		VELOCITY_MIN	113
#define 	VELOCITY_LOW	0
unsigned char Left_sensor = 0;
unsigned char Center_sensor = 0;
unsigned char Right_sensor = 0;

void sensor_value(){

	Left_sensor=return_sensor(3);
	Center_sensor=return_sensor(2);
	Right_sensor=return_sensor(1);
}

/*
	Function name : run(int k)
	input(): number of nodes
	output: none
	logic: it drives the robot straight and count nodes
	example call: called by traverse function
		run(5)
*/
void run(int k){
	unsigned char flag ;
	int count=0,count_flag=0;
	cli();
	init_motion();
	sei();
	while(1){
		sensor_value();
									//Prints Value of White Line Sensor3
	flag=0;
		sensor_value();
		if(Center_sensor>THRESHOLD)               // Is middle blackline is within threshold limit
		{
			flag=1;

			forward();      // Run robot at max velocity
			velocity(VELOCITY_MAX,VELOCITY_MAX);

		}
		sensor_value();
		if((Left_sensor<THRESHOLD) && (flag==0))  // Is left blackline is not within threshold limit
	//if((Left_white_line>THRESHOLD) && (flag==0))  // Is left blackline is not within threshold limit
		{
			flag=1;
			     // Run robot left wheel at max velocity and right wheel
			forward();
			velocity(VELOCITY_MAX,VELOCITY_MIN);

		/*	velocity(VELOCITY_MIN,60);      // Run robot right wheel at max velocity and left wheel
			lcd_print (2,1,VELOCITY_MIN,3);           // at min velocity
			lcd_print (2,5,VELOCITY_MAX,3);*/
		}
		sensor_value();
		if(Left_sensor>70 && Center_sensor >40 &&(flag==0)){
			flag=1;
			forward();
			velocity(VELOCITY_MIN,VELOCITY_MAX);
		}
		sensor_value();
		if((Right_sensor<THRESHOLD) && (flag==0)) // Is right blackline is not within threshold limit
		//if((Right_white_line>THRESHOLD) && (flag==0)) // Is right blackline is not within threshold limit
		{
			flag=1;
			     // Run robot right wheel at max velocity and left wheel
			forward();
			velocity(VELOCITY_MIN+5,VELOCITY_MAX);

		/*	velocity(60,VELOCITY_MIN);      // Run robot left wheel at max velocity and right wheel
			lcd_print (2,1,VELOCITY_MAX,3);           // at min velocity
			lcd_print (2,5,VELOCITY_MIN,3);*/
		}
		sensor_value();
		if(Right_sensor>70 && Center_sensor >40 &&(flag==0)){
			flag=1;
			forward();
			velocity(VELOCITY_MAX,VELOCITY_MIN);
		}
		sensor_value();
		if(Center_sensor<THRESHOLD && Left_sensor<THRESHOLD && Right_sensor<THRESHOLD && flag==0 )
		// if all blackline sensor are not within threshold limit
		{
			flag=1;
			velocity(VELOCITY_LOW,VELOCITY_LOW);      // stop the robot
			stop();

		}
		sensor_value();
		count_flag=0;
		if((Center_sensor>THRESHOLD && Left_sensor>THRESHOLD) || (Right_sensor>THRESHOLD && Center_sensor>THRESHOLD)||(Right_sensor>THRESHOLD && Center_sensor>THRESHOLD&&Left_sensor>THRESHOLD) )
		// if all blackline sensor are not within threshold limit
		{
			count_flag=1;
			sensor_value();
			_delay_ms(300);
		}
		count=count_flag+count;
		lcd_print(2,10,count,2);
		if(count==k){
		count=0;

		_delay_ms(300);
		stop();
		_delay_ms(500);
		//right_degrees(88);

		 break;




	/*do 	{
		right();

		if(return_sensor(1)+return_sensor(2)+return_sensor(3)>120)
		break;
		}while(1);

		stop();
		_delay_ms(200);*/
	}

	}
}


char * decode(int x){

	switch(x){
		case 1:
		return "B1" ;
		case 2:
		return "C1";
		case 3:
		return "D1";
		case 4:
		return "E1";
		case 5:
		return "F1";
		case 6:
		return "B2";
		case 7:
		return "C2";

		case 8:
		return "D2";
		case 9:
		return "E2";


		case 10:
		return "F2";

		case 11:
		return "B3";
		case 12:
		return "C3";
		case 13:
		return "D3";
		case 14:
		return "E3";
		case 15:
		return "F3";
		case 16:
		return "B4";

		case 17:
		return "C4";
		case 18:
		return "D4";
		case 19:
		return "E4";
		case 20:
		return "F4";
		case 21:
		return "B5";

		case 22:
		return "C5";
		case 23:
		return "D5";
		case 24:
		return "E5";
		case 25:
		return "F5";
	}
}








int pick=0;


char habitat_node[100]="",animal[100]="";

int N=1,S=0,E=0,W=0;

/*
▪ * Function Name:traverse(char A,char B,char C, char D)
▪ * Input: 4 arguments A,B,C,D
▪ * Output: it returns nothing
▪ * Logic: its traverse from one location to another location A and B contrains the charecters of the first loctation and C,D contains the destination locations charecters 
▪ * Example Call: traverse(a,b,c,d);
   it is called by parsestr()
▪ */

void traverse(char A,char B,char C,char D){
	int route[2];
	int i=0,j=0;

	j=0;
	cli();
	lcd_set_4bit();
	lcd_init();

	sei();

		route[0]=A-C;
		route[1]=B-D;




	/*/route[0]=-2;
	route[1]=0;

	j=0;*/

	if(route[0]<0){
		/**********rotate to east***************/
			if(N==1){
				right_degrees(88);
				N=0;
				E=1;
				S=0;
				W=0;
				}
				else{
					if(E==1){

					N=0;
					E=1;
					S=0;
					W=0;
					}
					else{
						if (S==1)
						{
						left_degrees(88);
						N=0;
						E=1;
						S=0;
						W=0;
						}
							else{
							if (W==1)
							{
							right_degrees(181);
							N=0;
							E=1;
							S=0;
							W=0;
							}

						}

					}



				}


		/*N=0;
		S=0;
		E=1;
		W=0;
		printf("  Rotate to East  ");*/
		}
		else if(route[0]>0){
			/***********************rotate to west*************************/
			if(N==1){
				left_degrees(88);
				N=0;
				E=0;
				S=0;
				W=1;
			}
			else{
				if(E==1){
					right_degrees(181);
					N=0;
					E=0;
					S=0;
					W=1;
				}
				else{
					if (S==1)
					{
						right_degrees(88);
						N=0;
						E=0;
						S=0;
						W=1;
					}
					else{
						if (W==1)
						{
							N=0;
							E=0;
							S=0;
							W=1;
						}

					}

				}

			}

		/*N=0;
		S=0;
		E=0;
		W=1;
		printf("  Rotate to West  ");*/
			}
	/*****************Special section**************************/
	if(abs(route[0])!=0){
		run(abs(route[0]));
	}
	/******************************************/
	if(route[1]<0){
		/****************NORTH***************************/
		if(N==1){
			N=1;
			E=0;
			S=0;
			W=0;
		}
		else{
			if(E==1){
				left_degrees(88);
				N=1;
				E=0;
				S=0;
				W=0;
			}
			else{
				if (S==1)
				{
					right_degrees(181);
					N=1;
					E=0;
					S=0;
					W=0;
				}
				else{
					if (W==1)
					{
						right_degrees(88);
						N=1;
						E=0;
						S=0;
						W=0;
					}

				}

			}

		}

		}else if(route[1]>0){
			/*****************south************************************/
		if(N==1){
			left_degrees(181);
			N=0;
			E=0;
			S=1;
			W=0;
		}
		else{
			if(E==1){
				right_degrees(88);
				N=0;
				E=0;
				S=1;
				W=0;
			}
			else{
				if (S==1)
				{

					N=0;
					E=0;
					S=1;
					W=0;
				}
				else{
					if (W==1)
					{
						left_degrees(88);
						N=0;
						E=0;
						S=1;
						W=0;
					}

				}

			}

		}
	}
	/***********************Special section**********************************/
	if (abs(route[1])!=0)
	{
		run(abs(route[1]));
	}

	/*************************************************************/

	if((C=='B'||C=='C'||C=='D'||C=='E')&&(D=='1')){
		/**************************WEST***********************************/
		if(N==1){
			left_degrees(88);
			N=0;
			E=0;
			S=0;
			W=1;
		}
		else{
			if(E==1){
				left_degrees(181);
				N=0;
				E=0;
				S=0;
				W=1;
			}
			else{
				if (S==1)
				{
					right_degrees(88);
					N=0;
					E=0;
					S=0;
					W=1;
				}
				else{
					if (W==1)
					{
						N=0;
						E=0;
						S=0;
						W=1;
					}

				}

			}

		}

	}
	if((D=='2'||D=='3'||D=='4'||D=='5')&&(C=='F')){
			/******************************SOUTH*********************************/
		if(N==1){
			left_degrees(181);
			N=0;
			E=0;
			S=1;
			W=0;
		}
		else{
			if(E==1){
				right_degrees(181);
				N=0;
				E=0;
				S=1;
				W=0;
			}
			else{
				if (S==1)
				{

					N=0;
					E=0;
					S=1;
					W=0;
				}
				else{
					if (W==1)
					{
						left_degrees(88);
						N=0;
						E=0;
						S=1;
						W=0;
					}

				}

			}

		}

	}
	if((D=='2'||D=='3'||D=='4'||D=='5')&&(C=='A')){
		/***********************NORTh***************************************/
		if(N==1){
			N=1;
			E=0;
			S=0;
			W=0;
		}
		else{
			if(E==1){
				left_degrees(88);
				N=1;
				E=0;
				S=0;
				W=0;
			}
			else{
				if (S==1)
				{
					right_degrees(181);
					N=1;
					E=0;
					S=0;
					W=0;
				}
				else{
					if (W==1)
					{
						right_degrees(88);
						N=1;
						E=0;
						S=0;
						W=0;
					}

				}

			}

		}

	}
	if((C=='B'||C=='C'||C=='D'||C=='E')&&(D=='6')){
		/********************************EAST*****************************/
		if(N==1){
			right_degrees(88);
			N=0;
			E=1;
			S=0;
			W=0;
		}
		else{
			if(E==1){

				N=0;
				E=1;
				S=0;
				W=0;
			}
			else{
				if (S==1)
				{
					left_degrees(88);
					N=0;
					E=1;
					S=0;
					W=0;
				}
				else{
					if (W==1)
					{
						right_degrees(181);
						N=0;
						E=1;
						S=0;
						W=0;
					}

				}

			}



		}
	}


	if(pick==0){

		if(C=='A'&&D=='1'){
			/*****NORT WEST*****/

			if(N==1){
				left_degrees(88/2);
				extended_grab_and_hold();
				right_degrees(88/2);
				pick=1;
			}
			else{
				if(E==1){

					left_degrees(135);
					extended_grab_and_hold();
					right_degrees(135);
					pick=1;
				}
				else{
					if (S==1)
					{
						right_degrees(135);
						extended_grab_and_hold();
						right_degrees(45);
						N=1;
						S=0;
						E=0;
						W=0;
						pick=1;
					}
					else{
						if (W==1)
						{
							right_degrees(45);
							extended_grab_and_hold();
							right_degrees(45);
							N=1;
							S=0;
							E=0;
							W=0;
							pick=1;
						}

					}

				}



			}
		}
		if(C=='A'&&D=='6'){
			/****************NORTH EAST*******************/
			if(N==1){
				right_degrees(45);
				extended_grab_and_hold();
				left_degrees(45);
				pick=1;


			}
			else{
				if(E==1){
					left_degrees(45);
					extended_grab_and_hold();
					right_degrees(45);
					pick=1;

				}
				else{
					if (S==1)
					{
							left_degrees(135);
							extended_grab_and_hold();
							right_degrees(135);

						pick=1;
					}
					else{
						if (W==1)
						{
							right_degrees(135);
							extended_grab_and_hold();
							left_degrees(135);
						pick=1;
						}

					}

				}



			}
		}

		if(C=='F'&&D=='6'){
			/*******************south east************************/
			if(N==1){
				right_degrees(130);
				extended_grab_and_hold();
				left_degrees(133);
					pick=1;

			}
			else{
				if(E==1){
					right_degrees(45);
					extended_grab_and_hold();
					left_degrees(45);
					pick=1;
				}
				else{
					if (S==1)
					{
						left_degrees(45);
						extended_grab_and_hold();
						right_degrees(45);
						pick=1;
					}
					else{
						if (W==1)
						{
							left_degrees(135);
							extended_grab_and_hold();
							right_degrees(135);
							pick=1;
						}

					}

				}



			}
		}
		if(C=='F'&&D=='1'){
			/*******South west*******/


			if(N==1){
				left_degrees(135);
				extended_grab_and_hold();
				right_degrees(135);
					pick=1;

			}
			else{
				if(E==1){
				right_degrees(135);
				extended_grab_and_hold();
				left_degrees(135);
					pick=1;

				}
				else{
					if (S==1)
					{
						right_degrees(45);
						extended_grab_and_hold();
						left_degrees(45);
						pick=1;

					}
					else{
						if (W==1)
						{
							left_degrees(45);
							extended_grab_and_hold();
							right_degrees(45);
							pick=1;
						}

					}

				}



			}
		}


		if(pick==0){
			pick=1;
			normal_grab_and_hold();
		}


		}else if(pick==1){




		/*mouth to north and 45 north east*/
			if(N==1){
				right_degrees(45);
				deposit();
				left_degrees(45);
				pick=0;
			}
			else{
				if(E==1){
					left_degrees(45);
					deposit();
					right_degrees(45);
					pick=0;
				}
				else{
					if (S==1)
					{
						left_degrees(135);
						deposit();
						right_degrees(135);
						pick=0;
					}
					else{
						if (W==1)
						{
							right_degrees(135);
							deposit();
							left_degrees(135);
							pick=0;
						}

					}

				}



			}


	}



}

/*
▪ * Function Name:parsestr()
▪ * Input: no parameters
▪ * Output: no return value
▪ * Logic: it takes the uart2 transmitted string and generates a new string to pass to travers function , that travers the arena 
   
▪ * Example Call: called after pressing boot switch
     
                 parsestr();
*/


void parsestr(){
	 char *k,*tok;
	char a,b,c,d;
	 char habitat[50]="";
	 char Final[100]="";
	 const char FINAL[100]="A1";
	 char decoded[2]="";
	 char pos[3],des[3];
	 int i=0,j=0;
	 int num;

	 i=0;
	 while(input_str[i]!='\n'){

		 habitat[i]=input_str[i];
		 i++;
	 }

	 tok=strtok(habitat,", ");
	 while (tok!= NULL)
	 {
		 //printf("%s ",tok);
		 strcat(habitat_node,decode(atoi(tok)));
		 tok = strtok (NULL, ", ");
	 }

	  while(input_str[i]!='#'){
		  //printf("%c",input_str[i]);
		  if(isalpha(input_str[i])||isalnum(input_str[i])){
			  animal[j]=input_str[i];
			  j++;
		  }
		  i++;

	  }
	  animal[i++]='\0';
	  i=0,j=0;
	  for(i=0;i<strlen(animal);i+=2){

		  Final[j]=animal[i];
		  Final[j+1]=animal[i+1];
		  Final[j+2]=animal[i];
		  Final[j+3]=animal[i+1];
		  Final[j+4]=habitat_node[i];
		  Final[j+5]=habitat_node[i+1];
		  Final[j+6]=habitat_node[i];
		  Final[j+7]=habitat_node[i+1];
		  j+=8;
	  }
	  Final[j-2]='\0';

	  strcat(FINAL,Final);
	  cli();
	 lcd_set_4bit();
	  lcd_init();
	  sei();
	  lcd_string(FINAL);
	_delay_ms(1000);

	  for(i=0;i<strlen(FINAL);i+=4){
		  pos[0]=FINAL[i];
		  pos[1]=FINAL[i+1];
	      pos[2]='\0';
		  des[0]=FINAL[i+2];
		  des[1]=FINAL[i+3];
		  des[2]='\0';

		  a=pos[0];
		  b=pos[1];

		  c=des[0];
		  d=des[1];
		  cli();
		  lcd_set_4bit();
		  lcd_init();
		  sei();

		  	lcd_cursor(2,4);
		  	lcd_string(pos);
			  lcd_cursor(2,6);
		  	lcd_string(des);
		  _delay_ms(1000);
		  traverse(a,b,c,d);

	  }
}

/**************************************************************************************************************/


/********************************************************INTURREPT INT7******************************/
void intrrupt(){

	DDRE  = DDRE & 0x80;  //Set the direction of the PORTE 7 pin as input
	PORTE = PORTE | 0x7F; //Enable internal pull-up for PORTE 7 pin

}

void init_boot_switch(){
	cli();
	EICRB = EICRB | 0x00;//0x80; // INT7 is set to trigger between two rising age
	EIMSK = EIMSK | 0x80; // Enable Interrupt INT7 for boot switch
	EIFR=  EIFR | 0X80;
	sei();

}
/*
Function Name :ISR
input: inturrept vector
output: none
logic : while external inturrept occurs it executes inturrept service routine;
examle call: called autometically during inturrept
*/
ISR(INT7_vect){

	straight_arm_free();

	buzzer_on();
	_delay_ms(1000);
	buzzer_off();
	_delay_ms(100);
	parsestr();
	_delay_ms(100);
	buzzer_on();
	_delay_ms(5000);
	buzzer_off();
	_delay_ms(100);

}
/*****************************************************************************************************/



int main(void)
{
    /* Replace with your application code */
	cli();
	buzzer_pin_config();
	ADC_init();
	intrrupt();
	init_boot_switch();
	lcd_port_init();
	uart2_init();
	init_servo();
	init_motion();
	sei();
	lcd_set_4bit();
	lcd_init();

		unsigned int value;///for ADC purpose
    while (1);

}

