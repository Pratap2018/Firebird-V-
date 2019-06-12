/*
 * Team Id: HC#145
 * Author List: Pratap
 * Filename:UART2_COM.h
 * Theme: HOMECOMING(HC)
 * Functions: uart2_init, display
 * Global Variables: NONE
 */


#ifndef UART2_COM_H_
#define UART2_COM_H_


/*
 * Function Name:uart2_init
 * Input: None
 * Output: None
 * Logic: initialize uart2 communication
 * Example Call:uart2_init()
 */
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

/*
 * Function Name:display
 * Input: None
 * Output: None
 * Logic: displays the string received via uart2 communication
 * Example Call:uart2_init()
 */
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





#endif /* UART2_COM_H_ */