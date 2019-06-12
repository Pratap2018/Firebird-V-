/*
 * Team Id: HC#145
 * Author List: BOOT_SWITCH.h
 * Filename: LCD.h
 * Theme: HOMECOMING(HC)
 * Functions: intrrupt,init_boot_switch
 * Global Variables: NONE
 */

#ifndef BOOT_SWITCH_H_
#define BOOT_SWITCH_H_


/*
 * Function Name:intrrupt
 * Input: NONE
 * Output: NONE
 * Logic: sets PE7 pin as input
 * Example Call: interrupt() 
 */

void intrrupt(){
	
	DDRE  = DDRE & 0x80;  //Set the direction of the PORTE 7 pin as input
	PORTE = PORTE | 0x7F; //Enable internal pull-up for PORTE 7 pin
	
}
/*
 * Function Name:init_boot_switch
 * Input: NONE
 * Output: NONE
 * Logic: initialize boot_switch 
 * Example Call: init_boot_switch()
 */
void init_boot_switch(){
	cli();
	EICRB = EICRB | 0x00;//0x80; // INT7 is set to trigger between two rising age
	EIMSK = EIMSK | 0x80; // Enable Interrupt INT7 for boot switch
	EIFR=  EIFR | 0X80;
	sei();
	
}



#endif /* BOOT_SWITCH_H_ */