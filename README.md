# Morning call machine
The project is a simple morning call machine with a speaker and a 5 motors' manipulator(using only 4 of the motors)

There're 2 status in the design, one is cancel mode and the other one is calling mode. The system will stay in the low power mode when the morning call countdown is canceled. Whenever you push the P1.3 interrupt button, the countdown would be triggered. After ten seconds, the speaker would alarm, and 5 seconds later the alarm would be louder and the manipulator would start a repeated movement to wake you up as well, if you press the P1.3 button anytime in the calling mode, the process would stop and enter the cancel mode. 

test
## Pin connection
P2.1, P2.2, P2.4, P2.5 -> motors' signal input pin 

P2.3 -> speaker's input pin

## PWM
For P2.4 & P2.5 they are using the same PWM width.
For other pins they can generate different width depends on the user.

## Clock
For P2.1 & P2.3 pins, they are PWM output pins self-generated using the TA0 interrupt.
For P2.2, P2.4 & P2.5 they are PWM output pins using the designed TA1.2 clock interrupt.


## Video Demo

[Watch the video demo](video/demo.MOV)

