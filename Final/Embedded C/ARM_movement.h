/*
 * Team Id:HC#145
 * Author List: Pratap
 * Filename: <Filename>
 * Theme:HOMECOMING(HC)
 * Functions: straight_arm_free,normal_grab_and_hold,extended_grab_and_hold
 * Global Variables: NONE
 */

#ifndef ARM_MOVEMENT_H_
#define ARM_MOVEMENT_H_


/*
 * Function Name:straight_arm_free
 * Input: NONE
 * Output:NONE
 * Logic: keeps the arm straight and it frees the servos
 * Example Call: straight_arm_free()
 */

void straight_arm_free(){
    servo_3(33);
    _delay_ms(300);
    servo_3_free();
    servo_1(105);
    _delay_ms(500);
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
/*
 * Function Name:normal_grab_and_hold
 * Input: NONE
 * Output:NONE
 * Logic: picks the blocks
 * Example Call: normal_grab_and_hold()
 */


void normal_grab_and_hold(){
    
    servo_1(105);
    _delay_ms(100);
    servo_2(87);
    _delay_ms(200);
    servo_3(28);
    _delay_ms(1000);
    servo_2(25);
    /*servo_1(105);
    _delay_ms(100);
    servo_2(27);*/
    
    
    _delay_ms(1000);
    servo_3(54);///////////////////////////////
    
    _delay_ms(1000);
    servo_1(105);
    _delay_ms(100);
    servo_2(95);
    _delay_ms(1500);
    servo_1_free();
    servo_2_free();
    
}
/*
 * Function Name:extended_grab_and_hold
 * Input: NONE
 * Output:NONE
 * Logic: picks the blocks from corner
 * Example Call: extended_grab_and_hold()
 */

void extended_grab_and_hold(){
    servo_1(105);
    _delay_ms(100);
    servo_2(95);
    _delay_ms(200);
    servo_3(28);
    _delay_ms(500);
    servo_2(45);
    _delay_ms(200);
    servo_1(82);
    
    _delay_ms(1000);
	
    servo_3(52);/////////////////////////////////////
    _delay_ms(600);
    servo_2_free();
    _delay_ms(100);
    servo_1(105);
    _delay_ms(500);
    servo_2(95);
    _delay_ms(1200);
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
 * Function Name:deposit
 * Input: NONE
 * Output:NONE
 * Logic: deposit the blocks
 * Example Call: deposit()
 */

void deposit(){
    
    servo_1(75);///105
    _delay_ms(500);
    servo_2(53);
    _delay_ms(1000);
    servo_3(32);
    _delay_ms(700);
    
    servo_1_free();
    servo_2_free();
    servo_3_free();
    _delay_ms(300);
    straight_arm_free();
    
}


#endif /* ARM_MOVEMENT_H_ */