/*
 * Team Id: HC#145
 * Author List: Pratap
 * Filename: solve_arena.h
 * Theme: HOMECOMING(HC)
 * Functions:traverse(char A,char B,char C,char D),parsestr()
 * Global Variables: NONE
 */


#ifndef SOLVE_ARENA_H_
#define SOLVE_ARENA_H_



/*
 * Function Name:traverse
 * Input: char A,char B,char C,char D 
 * Output:none
 * Logic: the function takes 4 character of the pick location to deposit location,this function takes care of how the path will be solved
 * Example Call: traverse('A','1','C','1')
and it will traverse from A1---->C1
 */
void traverse(char A,char B,char C,char D){
    int route[2];// array of size two
    int i=0,j=0;

    j=0;
    cli();
    lcd_set_4bit();
    lcd_init();
    
    sei();

        route[0]=A-C; //calculating nodal distance
        route[1]=B-D;//calculating nodal distance
        
        
    

    /*/route[0]=-2;
    route[1]=0;
    
    j=0;*/
    
    if(route[0]<0){ //if nodal distance is negative the robot will rotate to east
        /**********rotate to east***************/
            if(N==1){
                right90();
                N=0; //North flag
                E=1; //East flag
                S=0;//south flag
                W=0;//west flag
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
                        left90();
                        N=0;
                        E=1;
                        S=0;
                        W=0;
                        }
                            else{
                            if (W==1)
                            {
                            right180();
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
        else if(route[0]>0){//if nodal distance is positive the robot will rotate to west
            /***********************rotate to west*************************/
            if(N==1){
                left90();
                N=0;
                E=0;
                S=0;
                W=1;
            }
            else{
                if(E==1){
                    right180();
                    N=0;
                    E=0;
                    S=0;
                    W=1;
                }
                else{
                    if (S==1)
                    {
                        right90();
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
        run(abs(route[0])); //run nodal distance
    }
    /******************************************/
    if(route[1]<0){//if second nodal distance is negative the robot will rotate to NORTH
        /****************NORTH***************************/
        if(N==1){
            N=1;
            E=0;
            S=0;
            W=0;
        }
        else{
            if(E==1){
                left90();
                N=1;
                E=0;
                S=0;
                W=0;
            }
            else{
                if (S==1)
                {
                    right180();
                    N=1;
                    E=0;
                    S=0;
                    W=0;
                }
                else{
                    if (W==1)
                    {   
                        right90();
                        N=1;
                        E=0;
                        S=0;
                        W=0;
                    }
                    
                }
                
            }
            
        }
        
        }else if(route[1]>0){//if second nodal distance is positive the robot will rotate to south
            /*****************south************************************/
        if(N==1){
            left180();
            N=0;
            E=0;
            S=1;
            W=0;
        }
        else{
            if(E==1){
                right90();
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
                        left90();
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
            left90();
            N=0;
            E=0;
            S=0;
            W=1;
        }
        else{
            if(E==1){
                left180();
                N=0;
                E=0;
                S=0;
                W=1;
            }
            else{
                if (S==1)
                {
                    right90();
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
        if(pick==0){
        back_mm(15);/////////////////////////////
        //right_degrees(5);
        //_delay_ms(600);
        }
    }
    if((D=='2'||D=='3'||D=='4'||D=='5')&&(C=='F')){
            /******************************SOUTH*********************************/
        if(N==1){
            left180();
            N=0;
            E=0;
            S=1;
            W=0;
			back_mm(20);
        }
        else{
            if(E==1){
                right90();////changed at 12:45 am
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
                        left90();
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
                left90();
                N=1;
                E=0;
                S=0;
                W=0;
            }
            else{
                if (S==1)
                {
                    right180();
                    N=1;
                    E=0;
                    S=0;
                    W=0;
                }
                else{
                    if (W==1)
                    {
                        right90();
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
            right90();
			back_mm(10);
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
                    left90();
                    N=0;
                    E=1;
                    S=0;
                    W=0;
                }
                else{
                    if (W==1)
                    {
					
                        right180();
						back_mm(10);
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

	velocity(VELOCITY_MAX-40,VELOCITY_MAX-40);		
        
        if(C=='A'&&D=='1'){
            /*****NORT WEST*****/
            
            if(N==1){
                left_degrees(45);
                extended_grab_and_hold();
                right_degrees(45);
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
                        right_degrees(125);
                        extended_grab_and_hold();
                        right_degrees(50);
                        N=1;
                        S=0;
                        E=0;
                        W=0;
                        pick=1;
                    }
                    else{
                        if (W==1)
                        {
                            right_degrees(40);
                            extended_grab_and_hold();
                            right_degrees(40);
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
                right_degrees(40);
                extended_grab_and_hold();
                left_degrees(40);
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
                left_degrees(130);
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
                right_degrees(120);
                extended_grab_and_hold();
                left_degrees(120);  
                    pick=1;
                    
                }
                else{
                    if (S==1)
                    {
                        right_degrees(40);
                        extended_grab_and_hold();
                        left_degrees(40);
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
        	velocity(VELOCITY_MAX-40,VELOCITY_MAX-40);

        
        
        
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
                        right_degrees(203);/////////////////L
                        deposit();
                        left_degrees(203);/////////R
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
 * Function Name:parsestr
 * Input: NONE
 * Output:NONE
 * Logic: this fuction takes the string that is received through uart2 communication
	converts the string to pick and deposit location

 * Example Call: parestr()
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



#endif /* SOLVE_ARENA_H_ */