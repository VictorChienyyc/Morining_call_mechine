#include <msp430.h> 

#define PWM_PERIOD_1 2000
//#define PWM_PERIOD_2 600
#define PWM_PERIOD_2 2000

/**
 * main.c
 */

//int song = 0; // tells us which song of the two to play
int time_bank=0;
int cnt1=0;
int cnt2=0;
int reset=0;
int sound_cnt=0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    // P2DIR = (BIT3 | BIT7 | BIT2 | BIT5);
    P2DIR = (BIT1 | BIT3 | BIT2 | BIT4 | BIT5);

    // setting P1.3 for button interrupt
    P1DIR = BIT0; // set only bit0 to output
    P1IE |= BIT3; // enables interrupt
    P1IES |= BIT3; // go from high to low
    P1IFG &= ~BIT3; // clear the interrupt

     BCSCTL3 |= LFXT1S_2;      // ACLK = VLO

      TA0CCTL0 = CCIE;                   // TA0CCR0 interrupt enabled   // for BIT1 & BIT3
      TA0CCTL1 = CCIE;                   // TA0CCR0 interrupt enabled
      TA0CCTL2 = CCIE;                   // TA0CCR0 interrupt enabled
//      TA0CTL = TASSEL_1+ MC_1 + ID_0;    // ACLK, up-mode, divide by 8//-----
      TA0CTL = TASSEL_1+ MC_1 + ID_0;    // ACLK, up-mode, divide by 8//-----
      TA0CCR0 = PWM_PERIOD_2;             // Set PWM Period ~ 10 msec
      TA0CCR1 = 0;                      // -> 6 : left max(vibrate) // Start TA0.1 at 0 point of a period
      TA0CCR2 = 0;           // -> 30: right max            // Start TA0.1 at 0 point of a period //10 is proper, TACCR1 or TACCR2 shouldn't be too big


      TA1CTL = TASSEL_1 + MC_1;         // ACLK, upmode for TA0 // for BIT2 & BIT4
      TA1CCR0 = PWM_PERIOD_1;             // Set PWM Period ~ 10 msec
      TA1CCR1 = 0;                      // Start TA0.1 at 0 point of a period   //6
      TA1CCTL1 = OUTMOD_7;              // Set PWM mode for TA0.1
      TA1CCR2 = 0;                      // Start TA0.1 at 0 point of a period  //31
      TA1CCTL2 = OUTMOD_7;              // Set PWM mode for TA0.1

//    P2SEL |= BIT2 + BIT5;     // Select functions for specific Port1 Pins
//    P2SEL2 &= ~(BIT2 + BIT5);      //

      P2SEL |= BIT2 + BIT4 + BIT5;     // Select functions for specific Port1 Pins
      P2SEL2 &= ~(BIT2 + BIT4 + BIT5);      //




      __bis_SR_register(GIE + LPM3_bits); // enables global interrupts    // wait until the button to exit the LPM
      while (1) {
          if (cnt2 == 1 && 70 > time_bank)
          {
              TA0CCR1 = 0;                      // -> 6 : left max(vibrate) // Start TA0.1 at 0 point of a period
              TA0CCR2 = 0;           // -> 30: right max            // Start TA0.1 at 0 point of a period //10 is proper, TACCR1 or TACCR2 shouldn't be too big

              TA1CCR1 = 0;                      // Start TA0.1 at 0 point of a period   //6
              TA1CCR2 = 0;                      // Start TA0.1 at 0 point of a period  //31
          }
          else if (70 < time_bank < 90 && cnt2 == 1 )
          {
              TA1CCR1 = 8;                          // motor 3
              TA1CCTL1 = OUTMOD_7;
              TA1CCR2 = 8;                         // motor 2
              TA1CCTL2 = OUTMOD_7;
              TA1CTL = TASSEL_1 + MC_1;
              TA0CCTL1 = CCIE;
              TA0CCTL2 = CCIE;
              TA0CCR1 = 6;                         //motor 1
              TA0CCR2 = 30;
              TA0CTL = TASSEL_1+ MC_1 + ID_0;
              __delay_cycles(700);

              TA1CCR1 = 8;                          // motor 3
              TA1CCTL1 = OUTMOD_7;
              TA1CCR2 = 20;                         // motor 2
              TA1CCTL2 = OUTMOD_7;
              TA1CTL = TASSEL_1 + MC_1;
              TA0CCTL1 = CCIE;
              TA0CCTL2 = CCIE;
              TA0CCR1 = 6;                          // motor 1
              TA0CCR2 = 30;
              TA0CTL = TASSEL_1+ MC_1 + ID_0;
              __delay_cycles(500);

          }

          else
          {
              TA0CCR1 = 0;
              TA0CCR2 = 0;

              TA1CCR1 = 0;
              TA1CCR2 = 0;
          }
      }

}

#pragma  vector=TIMER0_A0_VECTOR                // Define TIMER0_A0 Interrupt Vector    //TA0.0
 __interrupt  void TIMER0_A0_ISR(void)          // TIMER0_A0 Interrupt Service Routine
 {
     if (cnt2 == 1)
     {
         time_bank++;

     }

     if (time_bank >= 50 && cnt2 == 1 )
     {
         P2OUT |= (BIT1 | BIT3);
     }
     else
     {
         P2OUT &= ~(BIT1 | BIT3);
     }


//     _bic_SR_register_on_exit(LPM3_bits);       // Return in Wake-Up Condition
 }

#pragma  vector=TIMER0_A1_VECTOR                // Define TIMER0_A1 Interrupt Vector    //TA0.1
 __interrupt  void TIMER0_A1_ISR(void)          // TIMER0_A1 Interrupt Service Routine
 {
     unsigned char temp;                        // key
     temp = TAIV;                               // key

     if (time_bank >= 50 )
     {
         if (temp == TA0IV_TACCR1){                 // Only if TA0 Interrupt Came From CCR1 Compare
             P2OUT &= ~(BIT1);
         }
         else if (temp == TA0IV_TACCR2){                 // Only if TA0 Interrupt Came From CCR1 Compare
             P2OUT &= ~(BIT3);
         }
     }
     else
     {
         P2OUT &= ~(BIT1);
         P2OUT &= ~(BIT3);

     }


//     _bic_SR_register_on_exit(LPM3_bits);       // Return in Wake-Up Condition
 }

 // ISR for button press
 #pragma vector=PORT1_VECTOR
 __interrupt void PORT1_ISR(void)
 {
     int i;
     int j;
     reset = 0;
     for(i = 0; i < 10; ++i){} // burn cycles to deal with debouncing
//     song ^= 1; // toggle song to play each button press
     cnt1++;

     cnt2 = cnt1 % 2;

     if (cnt2 == 0)
     {
         TA0CCR1 = 0;                      // -> 6 : left max(vibrate) // Start TA0.1 at 0 point of a period
         TA0CCR2 = 0;           // -> 30: right max            // Start TA0.1 at 0 point of a period //10 is proper, TACCR1 or TACCR2 shouldn't be too big

         TA1CCR1 = 0;                      // Start TA0.1 at 0 point of a period   //6
         TA1CCR2 = 0;                      // Start TA0.1 at 0 point of a period  //31

         time_bank = 0;

     }

     P1IFG &= ~BIT3; // clear the interrupt
     __bic_SR_register_on_exit(LPM3_bits); // enable low power mode
 }

//============================================================================================
 //============================================================================================
 //============================================================================================
