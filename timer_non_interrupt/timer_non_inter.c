#include <msp430g2553.h>
#include"CDC.h"

volatile char DES=0,flag11;
volatile char sec_t=0,min_t=0,hour_t=0,day_t=0;
volatile char timerCount=0;
volatile unsigned int COUNTT=40,countt=0,COUNTT1=10,countt1=0,count_t=0;
	void uart_init()
		{
					P1SEL |= ( BIT1|BIT2);
			P1SEL2 |= ( BIT1|	BIT2);
			UCA0CTL1 = UCSWRST;
			UCA0CTL1 |= 0x80;                     // SMCLK
			UCA0BR0 = 130;                            // 16MHz 9600 PREESCALAR
			UCA0BR1 = 6;                             //(UCAxBR0 + UCAxBR1 � 256)
			UCA0MCTL =0x0c;//UCBRS0;                        // Modulation UCBRSx = 1
			UCA0CTL1 &= ~UCSWRST;
			IE2 = UCA0RXIE;
		}


 void Timer_block()

{

	   TACTL	 = ID_1|TASSEL_2|MC_1; //UP mode
	   TACCR0	 = 100;
	   TACCTL0 =OUTMOD_7| CCIE;
	   TACCR1=1000;



	   TACTL	 = ID_1|TASSEL_2|MC_1; //UP mode
	   TACCR0	 = 100;
	   TACCTL0 =OUTMOD_7| CCIE;
	   TACCR1=1000;



	   TA1CTL	 = ID_1|TASSEL_2|MC_1; //UP mode
	   TA1CCR0	 = 10000;
	   TA1CCTL1 =OUTMOD_7| CCIE;
	   TA1CCR1=10000;


	   }

 void timer0_en(){

 	  TACTL	 = ID_3|TASSEL_2|MC_1; //UP mode
 		   TACCR0	 = 40;
 		   TACCTL0 =OUTMOD_7;
 		   TACCR1=0;

 }
 void enable_timer()
		{
		    TA1CTL	 = ID_3|TASSEL_2|MC_1; //UP mode
		    TA1CCR0	 = 2000;
		    TA1CCTL1 =OUTMOD_7| CCIE;
		    TA1CCR1=000;
		}





 void set_time_sec(unsigned int second_s){}


		void enable_interrupts()
 		{
 			_BIS_SR(GIE);
 			__enable_interrupt();
 			__bis_SR_register(GIE);
 		}
	void clk_init()
		{
			WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT
			BCSCTL1 =CALBC1_16MHZ;
			BCSCTL2 =0x00;
			DCOCTL = CALDCO_16MHZ;
		}



	int main(void)
	{
		clk_init();

		//uart_init();





					P2SEL2=0X00;
					P2SEL=0X00;
					P2DIR=0XFF;
					P2OUT=0;
					P1DIR|=BIT7;
					P1SEL&=~BIT7;
					P1SEL2&=~BIT7;

					// enable_timer();

				 	enable_interrupts();
				 					while(1){



				 						 timer0_en();
						   do{

							 while((TACCTL0&CCIFG)!=CCIFG);
							        TACCTL0&=~CCIFG;

							    if(countt<COUNTT){P2OUT|=BIT2;}else{P2OUT&=~BIT2;}
								if(countt<1001){countt++;}else{countt=0;timerCount++;}



						   }while(timerCount<4);
						   TACCTL0|=TACLR;
						   timerCount=0;

						   _delay_cycles(4000000);
				/*SetCursorPosition(0, 0);
				PrintStr("Temp ");
				lcd_print_uint(25);
				SetCursorPosition(2, 0);
				lcd_print_uint(day_t);putme(':'); lcd_print_uint(hour_t);putme(':'); lcd_print_uint(min_t);putme(':'); lcd_print_uint(sec_t);//c_lc();
		_delay_cycles(4000000);
				 */
				 					}


	}







#pragma vector=USCIAB0RX_VECTOR
	__interrupt void RXserial_isr(void)
	{
		IFG2&=~UCA0RXIFG;

		DES=UCA0RXBUF;




	}


#pragma vector=TIMER1_A1_VECTOR
	__interrupt void Timer1a1_isr(void)
	{

		TA1CCTL1&=~CCIFG;
		if(countt1<COUNTT1){   P1OUT|=BIT7; }else{   P1OUT&=~BIT7; }
        if(countt1< 20 ){   countt1++;    }else{   countt1=0;     }


        if(count_t==1000)
        {
          count_t=0;

           if(sec_t==59)
			 {
			  sec_t=0;

			    if(min_t==59)
				 {
					min_t=0;

					if(hour_t==24)
					{
						hour_t=0;
						day_t+=1;

					}else{hour_t+=1;}

				 }else{ min_t+=1;}

			 }else{sec_t+=1;}

       }else{count_t++;}


	}

#pragma vector=TIMER0_A0_VECTOR
	__interrupt void Timer0a0_isr(void)
	{

		TACCTL0&=~CCIFG;

		if(countt<COUNTT){P2OUT|=BIT2;}else{P2OUT&=~BIT2;}
        if(countt<1001){countt++;}else{countt=0;}



	}
