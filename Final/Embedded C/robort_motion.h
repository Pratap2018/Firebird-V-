/*
 * Team Id: HC#145
 * Author List: Pratap
 * Filename: robot_motion.h
 * Theme: HOMECOMMING(HC)
 * Functions: Left_sensor,Center_sensor,Right_sensor,left90,right90,left180,right180,run
 * Global Variables: ALL required global varriables are in main.c
 */

#ifndef ROBORT_MOTION_H_
#define ROBORT_MOTION_H_

/*
 * Function Name:sensor_value
 * Input: NONE
 * Output: NONE
 * Logic: reads the the white line sensor values and stores in corresponding variable 

 * Example Call: sensor_value()
 */

void sensor_value(){
    
    Left_sensor=return_sensor(3);// here in main.c Left_sensor is a macro the file robot_motion.h file is included after declaring macros
    Center_sensor=return_sensor(2);//here in main.c Center_sensor is a macro the file robot_motion.h file is included after declaring macros
    Right_sensor=return_sensor(1);//here in main.c Right_sensor is a macro the file robot_motion.h file is included after declaring macros
}

/*
 * Function Name:left90
 * Input: NONE
 * Output: NONE
 * Logic:rotate to left until it find black line

 * Example Call: left90()
 */

void left90(){
    velocity(VELOCITY_MAX,VELOCITY_MAX);
    left();
    _delay_ms(400);
    while (return_sensor(2)<THRESHOLD)//3
    {   
        
        velocity(105,105);
        left();
        
        //_delay_ms(1000);
            
        /*
        lcd_print (2,1,return_sensor(3),3);
        lcd_print(2,5,return_sensor(2),3);
        lcd_print(2,9,return_sensor(1),3);*/
    }
    stop();
    _delay_ms(100);////////////////////////////////////////
    velocity(VELOCITY_MAX,VELOCITY_MAX);
}
/*
 * Function Name:right90
 * Input: NONE
 * Output: NONE
 * Logic:rotate to right until it find black line

 * Example Call: right90()
 */
void right90(){
    velocity(VELOCITY_MAX,VELOCITY_MAX);
    right();
    _delay_ms(400);
    while (return_sensor(2)<THRESHOLD) //1
    {
        
        velocity(105,105);
        right();
        
        //_delay_ms(1000);
        
        
       /* lcd_print (2,1,return_sensor(3),3);
        lcd_print(2,5,return_sensor(2),3);
        lcd_print(2,9,return_sensor(1),3);*/
    }
    stop();
    _delay_ms(100);////////////////////////////////////////
    velocity(VELOCITY_MAX,VELOCITY_MAX+5);
}

/*
 * Function Name:left180
 * Input: NONE
 * Output: NONE
 * Logic:rotate to left until  it find black line for two times

 * Example Call: left90()
 */

void left180(){
    left90();
    left90();
}

/*
 * Function Name:right180
 * Input: NONE
 * Output: NONE
 * Logic:rotate to right until  it find black line for two times

 * Example Call: right90()
 */
void right180(){
    right90();
    right90();
}

/*
 * Function Name:run
 * Input: int k
 * Output: No return values
 * Logic: robot moves forward using black line till number(k) of nodes 
here k is the parameter 

 * Example Call: run(abs[0])
 */
void run(int k){
    unsigned char flag ;
    int count=0,count_flag=0;
    cli();
    init_motion();
    sei();
    while(1){
        sensor_value();
                                    
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
            
        /*  velocity(VELOCITY_MIN,60);      // Run robot right wheel at max velocity and left wheel 
            lcd_print (2,1,VELOCITY_MIN,3);           // at min velocity
            lcd_print (2,5,VELOCITY_MAX,3);*/
        }
        sensor_value();
        if(Left_sensor>60  && Left_sensor>60 && Center_sensor >50 && Center_sensor <90 &&(flag==0)){
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
            velocity(VELOCITY_MIN-5,VELOCITY_MAX+10); 
            
        /*  velocity(60,VELOCITY_MIN);      // Run robot left wheel at max velocity and right wheel 
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
        
        count_flag=0;
        sensor_value();
        if((Center_sensor>THRESHOLD && Left_sensor>THRESHOLD) || (Right_sensor>THRESHOLD && Center_sensor>THRESHOLD)||(Right_sensor>THRESHOLD && Center_sensor>THRESHOLD&&Left_sensor>THRESHOLD) )
        // if all blackline sensor are not within threshold limit
        {
            count_flag=1;
            //sensor_value();
            _delay_ms(400);
            
        }
        count=count_flag+count;
        lcd_print(2,10,count,2);
        if(count==k){
        count=0;
        
        _delay_ms(60);
        stop();
        _delay_ms(500);
        //right90();
    
         break;
    
            
        
        
    /*do    {
        right();
        
        if(return_sensor(1)+return_sensor(2)+return_sensor(3)>120)
        break;          
        }while(1);

        stop();
        _delay_ms(200);*/
    }
        
    }
}



#endif /* ROBORT_MOTION_H_ */