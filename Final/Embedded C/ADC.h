
/*
 * Team Id: HC#145
 * Author List: Pratap Mridha, Sujan Bag
 * Filename: ADC.h
 * Theme: Homecoming (HC)
 * Functions: adc_pin_config,adc_init,ADC_init,ADC_Conversion,print_sensor,
 * Global Variables: pick,habitat_node[100],animal[100],N,S,E,W,Left_sensor,center_sensor,right_sensor,ShaftCountLeft,ShaftCountRight,data,Degrees,input_str[100],ADC_Value,sharp,distance,adc_reading
 */



#ifndef ADC_H_
#define ADC_H_


//ADC pin configuration

/*
 * Function Name:adc_pin_config()
 * Input: No parameter 
 * Output: no return values
 * Logic:it sets  ports for adc

 * Example Call: adc_pin_config()
 */
void adc_pin_config (void)
{
	DDRF = 0x00; //set PORTF direction as input
	PORTF = 0x00; //set PORTF pins floating
	DDRK = 0x00; //set PORTK direction as input
	PORTK = 0x00; //set PORTK pins floating
}
/*
 * Function Name:adc_init()
 * Input: No Parameters
 * Output: No return values
 * Logic: setting register values for adc
 * Example Call: adc_init()
 */
void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;      //MUX5 = 0
	ADMUX = 0x20;       //Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;      //ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}
/*
 * Function Name:ADC_init()
 * Input: No Parameters
 * Output: No return values
 * Logic: Initialising Adc
 * Example Call:ADC_init()
 */
void ADC_init()
{
	adc_pin_config();
	adc_init();
}
/*
 * Function Name:ADC_Conversion
 * Input:unsigned char ch
 * Output: unsigned char
 * Logic: it takes a sensor number and returns the value from the sensor
 * Example Call:ADC_Conversion(channel)
 */


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
	ADCSRA = ADCSRA | 0x40;     //Set start conversion bit
	while((ADCSRA&0x10)==0);    //Wait for ADC conversion to complete
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


#endif /* ADC_H_ */