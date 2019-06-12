/*
 * Robot.c
 *
 * Created: 01-02-2019 21:17:00
 * Author : Pratap ,Sujan 
 */ 

/*
 * Team Id: HC#145
 * Author List: Pratap Mridha, Sujan Bag
 * Filename: main.c
 * Theme: Homecoming (HC) 
 * Functions: SIGNAL(USART2_RX_vect),ISR(INT7_vect),main()
 * Global Variables: pick,habitat_node[100],animal[100],N,S,E,W,Left_sensor,center_sensor,right_sensor,ShaftCountLeft,ShaftCountRight,data,Degrees,input_str[100],ADC_Value,sharp,distance,adc_reading
 */

#define F_CPU 14745600


#include <avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;//to store sensor values
unsigned char sharp, distance, adc_reading;//to store sensor values
unsigned int value;//to store sensor values
float BATT_Voltage, BATT_V;//battery voltage



int pick=0;// pick/deposit flag
char habitat_node[100]="",animal[100]=""; //char array of habitat and animal
int N=1,S=0,E=0,W=0; // N for north ,S for south ,E for East , W for West

#define     THRESHOLD       90 //Threshold for blackline
#define     VELOCITY_MAX    180//max velocity of wheel
#define     VELOCITY_MIN    105//min velocity of wheel
#define     VELOCITY_LOW    0//stop velocity
unsigned char Left_sensor = 0;//left black line sensor values
unsigned char Center_sensor = 0;//center black line sensor values
unsigned char Right_sensor = 0;//right black line sensor values 

volatile unsigned long int ShaftCountLeft = 0; //to keep track of left position encoder
volatile unsigned long int ShaftCountRight = 0; //to keep track of right position encoder
volatile unsigned int Degrees; //to accept angle in degrees for turning



unsigned char data; //to store received data from UDR2
char input_str[100]="";//to hold the entire received string

#include "LCD.h"
#include "ADC.h"



/*********************************MY headers********************************************/
#include "MOTION.h"
#include "retrurn_node.h"
#include "SERVO.h"

#include "ARM_movement.h"
#include "robort_motion.h"
#include "solve_arena.h"
#include "BOOT_SWITCH.h"
#include "UART2_COM.h"

/*******************************************************************************************/


/*
 * Function Name:SIGNAL
 * Input: USART rx vector
 * Output: NO return values 
 * Logic: it receives the data via UART2 communication 
 * char by char string is received and appended to the input_str variable
 * Example Call: It is called automatically when Uart2 signal receives.
 */


SIGNAL(USART2_RX_vect) {
    data=UDR2;

    strcat(input_str,&data);
    
    if(data=='#'){
            
        display();
    }
}   

/*
 * Function Name:ISR
 * Input: Inturrept vector
 * Output: NO return values
 * Logic: while boot switch is pressed this interrupt occurs it starts executing the inturrept service routine
 *
 * Example Call: It is called automatically when boot switch pressed
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
        
    /*left180();
    _delay_ms(1000);
    right180();
    _delay_ms(1000);
    left90();
    _delay_ms(1000);
    right90();
    */

    
}
/*****************************************************************************************************/
/*
 * Function Name:main
 * Input: no input
 * Output: no return
 * Logic:this is the main entry poinf the code
 *
 * Example Call: called by operating system
 */


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

