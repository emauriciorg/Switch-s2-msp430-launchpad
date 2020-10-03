/*--------------------------------------------------------
GEORGE MASON UNIVERSITY
ECE 447 - Lab3 In-Lab Exercise
  P2.1 button
  P2.2 button
  P2.3 button
  P2.4 button

  P2.6 Row Clock
  P2.7 Row Initialization

  P9.0 LED column 0
  ...
  P9.3 LED column 3

Date:   Fall 2020
Author: Jens-Peter Kaps

Change Log:
20200917 Initial Version, Jens-Peter Kaps
--------------------------------------------------------*/

// create global variable to "remember" which columns are on and which are off

#include <msp430.h>
#include <stdbool.h>


volatile unsigned char columns[8] = {0};

//Variables to hold the direction of the dot according to the button
volatile unsigned char  up=0;
volatile unsigned char  down=0;
volatile unsigned char  left=0;
volatile unsigned char  right=0;



void delay(void) {
    volatile unsigned loops = 250; // Start the delay counter at 25,000
    while (--loops > 0);             // Count down until the delay counter reaches 0
}


void set_maxtrix_columns(char value){
    P9OUT &=~0x7f; //clears the column port before setting it again
    P8OUT &=~(BIT7);
    
    P9OUT |=value & 0X7F;          //sets the port 9 with the value of the column array
    P8OUT |=(BIT7 & (value&0x80)); //only sets the bit7 of the port 8


}

void init_led_matrix(void){
    /*setting  the led matrix column pins as output*/

    P9DIR |= (BIT0| BIT1 |BIT2|BIT3|BIT4|BIT5|BIT6);  // P6DIR=0x7f;
    P9OUT &=~(BIT0| BIT1 |BIT2|BIT3|BIT4|BIT5|BIT6);  // P6DIR=0x7f;

    P8DIR|= (BIT7);
    P8OUT&=~(BIT7);

    /*Setting port alternate function to be I/O*/
    P9SEL0 &=~(BIT0| BIT1 |BIT2|BIT3|BIT4|BIT5|BIT6);
    P9SEL1 &=~(BIT0| BIT1 |BIT2|BIT3|BIT4|BIT5|BIT6);

    P8SEL0 &=~(BIT7);
    P8SEL1 &=~(BIT7);

}

void init_buttons(void){

    P2DIR&=~(BIT1 | BIT2 | BIT3 |BIT4);
    P2OUT|=(BIT1 | BIT2 | BIT3 |BIT4);
    P2REN|=(BIT1 | BIT2 | BIT3 |BIT4);

 /*INTERRUPT , Edge selection ,:1 high to low,0: low to high*/
    P2IES&=~(BIT1 | BIT2 | BIT3 |BIT4);
/*INTERRUPT , enable individual interrupt*/
    P2IE |=(BIT1 | BIT2 | BIT3 |BIT4); 
    P2IFG = 0X00; /*INTERRUPT , clear flags*/

}



unsigned char current_col_pos  =0x08;
unsigned char current_arrow_pos=0;

//This routine would read the process the pressed button and update the column array accordingly
void update_dot_pos(void){

//Updates the columns and dont let them pass go out boundaries

    if (left)  {
        current_col_pos = current_col_pos>>1;
        if (current_col_pos==0)current_col_pos=0x01;
    }

    if (right) {
        current_col_pos = current_col_pos<<1;
        if (current_col_pos==0)current_col_pos=0x80;
    }

//Updates the arrows and dont let them pass the limit of the array size
    if(up){
        if( current_arrow_pos < 7)
            current_arrow_pos++;
    }

    if(down){
        if( current_arrow_pos > 0)
            current_arrow_pos--;
    }

    up = 0;
    down = 0;
    left = 0;
    right = 0;

//clear all the columns and only update the current position
    columns[0] = 0;
    columns[1] = 0;
    columns[2] = 0;
    columns[3] = 0;
    columns[4] = 0;
    columns[5] = 0;
    columns[6] = 0;
    columns[7] = 0;
    
    columns[current_arrow_pos] = current_col_pos;

}


void update_column(void){
    if (up)    columns[4] = (~columns[4])  & 0xff;
    if (down)  columns[5] = (~columns[5])  & 0xff;
    if (left)  columns[6] = (~columns[6])  & 0xff;
    if (right) columns[7] = (~columns[7])  & 0xff;
    up = 0;
    down = 0;
    left = 0;
    right = 0;
}

void enable_interrupts(){
    _BIS_SR(GIE);
    __enable_interrupt();
    __bis_SR_register(GIE);
}


int main(void)
{
   unsigned char rowcnt =0; // row counter
    unsigned char port_output = 0;
    unsigned char row_aux = 0;

    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;     // Unlock ports from power manager

    P2DIR &= ~(BIT1 | BIT2 | BIT3 |BIT4); // buttons as inputs
    P2REN |= (BIT1 | BIT2 | BIT3 |BIT4);  // enable resistor
    P2OUT |= (BIT1 | BIT2 | BIT3 |BIT4);  // pull up resistor

    P2DIR |= (BIT6 | BIT7);   // Make Port 2.6 and 2.7 outputs
    P2OUT &= ~(BIT6 | BIT7);  // Set ports to 0

    init_led_matrix();

    // Insert Interrupt configuration for buttons here
    init_buttons();

    rowcnt = 0;
    
    // for part 3 of the lab
    columns[5]=0x08; //start point for last exercise

    // Enable interrupts
    enable_interrupts();

    while(1)                    // continuous loop
    {

        delay();

        if(P2OUT & BIT6) // If row clock 1 -> place breakpoint here
            P2OUT &= ~BIT6;  //   Set row clock 0
        else {

            /*maps the current shift register row to FW row */
            if (rowcnt== 0){
                row_aux = 7;
            }else{
                row_aux =rowcnt-1;
            }

            port_output = columns[row_aux];

            if(rowcnt == 7) {// if on row 7

                P2OUT |= BIT6;  //   Set row clock and row init 1
                P2OUT |= BIT7;  //   set row init to 0

                rowcnt = 0;  //   set row counter back to 0

            } else { // for all other rows
                P2OUT |= BIT6;   //   only set row clock 1
                P2OUT &= ~BIT7;  //   set row init to 0


                rowcnt++;//   increment row counter
            }
            set_maxtrix_columns(port_output);
        }
    }

    return 0;
}

// Add code to the ISR below to set and reset which columns should be on
// and which should be off.
// More than on column can be on at the same time.

// P2 interrupt service routine

#pragma vector = PORT2_VECTOR      // associate funct. w/ interrupt vector
__interrupt void Port_2(void)      // name of ISR
{
    // accessing P2IV automatically clears the pending interrupt flag with
    // the highest priority with P2IFG.0 being the highest and P2IFG.7 the lowest.
    

    switch(__even_in_range(P2IV,P2IV_P2IFG7))
    {
    case P2IV_NONE:   break;   // Vector  0: no interrupt
    case P2IV_P2IFG0: break;   // Vector  2: P2.0
    case P2IV_P2IFG1:  left  = true; break;   // Vector  4: P2.1
    case P2IV_P2IFG2:  up    = true; break;   // Vector  6: P2.2
    case P2IV_P2IFG3:  down  = true; break;   // Vector  8: P2.3
    case P2IV_P2IFG4:  right = true; break;   // Vector 10: P2.4
    case P2IV_P2IFG5: break;   // Vector 12: P2.5
    case P2IV_P2IFG6: break;   // Vector 14: P2.6
    case P2IV_P2IFG7: break;   // Vector 16: P2.7
    default: break;
    }

// To test part 3 of the lab
    update_dot_pos();

    //To test part2 of the lab
    //update_column();



}