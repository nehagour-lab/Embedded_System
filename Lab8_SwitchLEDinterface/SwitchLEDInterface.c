// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.


// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****
unsigned long SW1;

// ****** Constant Declaration  to access port register using symbolic names instead of addresses
// Constant declarations to access port registers using 
// symbolic names instead of addresses
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(unsigned long msec); 
void PortE_Init(void);



// ***** 3. Subroutines Section *****

void PortE_Init(void){
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x10;     // 1) E clock
  delay = SYSCTL_RCGC2_R;           // delay         
  GPIO_PORTE_AMSEL_R &= 0x00;        // 3) disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R = 0x02;          // 5) PE1 output   
  GPIO_PORTE_AFSEL_R &= 0x00;        // 6) no alternate function       
  GPIO_PORTE_DEN_R |= 0x03;          // 7) enable digital pins PE0, PE1
  GPIO_PORTE_DATA_R |= 0x03;	
}

void Delay100ms(unsigned long msec){
	unsigned long i;
    while(msec > 0){
      i = 1333333; 
        while(i > 0){
          i--;
        }
      msec--;
    }
}



// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).
int main(void){ 
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);  // activate grader and set system clock to 80 MHz
  PortE_Init(); //initialization of PORT E
  EnableInterrupts();           // enable interrupts for the grader
  while(1){
		SW1 = GPIO_PORTE_DATA_R&0X01;
		if(SW1 == 0X01) {
			  Delay100ms(1);
				GPIO_PORTE_DATA_R ^= 0X02; //Toggle 
		}else {
				GPIO_PORTE_DATA_R |= 0X02; // LED "ON"
		}
	}	
}

// Color    LED(s) PORTE
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06

