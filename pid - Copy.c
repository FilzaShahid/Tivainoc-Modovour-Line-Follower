#define TM_BASE 0x40031000
// Peripheral clock enabling for timer and GPIO
#define RCGC_TIMER_R (*((volatile unsigned long *)0x400FE604))  // clock for timer
#define RCGC2_GPIO_R (*((volatile unsigned long *)0x400FE108))  // clock for gpio
#define SYSCTL_RCGCGPIO_R (*((volatile unsigned long *)0x400FE608))
#define CLOCK_GPIOB 0x00000002 // Port F clock control
#define SYS_CLOCK_FREQUENCY 16000000

#define GPTM_CONFIG_R (*(( volatile long *)(TM_BASE + 0x000)))// 16bit or 32bit
#define GPTM_TA_MODE_R (*(( volatile long *)( TM_BASE + 0x004 )))//perodic or pwm
#define GPTM_CONTROL_R (*(( volatile long *)( TM_BASE + 0x00C )))//select A or B
#define GPTM_INT_MASK_R (*(( volatile long *)( TM_BASE + 0x018 )))//intrupt
#define GPTM_INT_CLEAR_R (*(( volatile long *)( TM_BASE + 0x024 )))//intrupt clear
#define GPTM_TA_IL_R  (*(( volatile long *)( TM_BASE + 0x028)))// Reload
#define GPTM_TA_MATCH_R (*(( volatile long *)( TM_BASE + 0x030 )))//Pwm Duty Cycle
// GPIO PF2 alternate function configuration
#define GPIO_PORTB_AFSEL_R (*(( volatile unsigned long *)0x40005420))
#define GPIO_PORTB_PCTL_R (*(( volatile unsigned long *)0x4000552C))
#define GPIO_PORTB_DEN_R (*((volatile unsigned long *)0x4000551C))
#define TIM_16_BIT_CONFIG 0x00000004
#define TIM_PERIODIC_MODE 0x00000002
#define TIM_B_ENABLE 0x00000100
#define TIM_A_ENABLE 0x00000001
#define TIM_PWM_MODE 0x0000000A
#define TIM_CAPTURE_MODE 0x00000004
#define TIM_A_INTERVAL 16000
#define TIM_A_MATCH 12000
//port A 
#define basespeed 12000
#define GPIO_PORTA_DATA_R (*((volatile long *)0x400043FC))
#define GPIO_PORTA_DIR_R (*((volatile long *) 0x40004400))
#define GPIO_PORTA_DEN_R (*((volatile long *) 0x4000451C))


#define TM_BASE2 0x40031000
// Peripheral clock enabling for timer and GPIO
#define RCGC_TIMER_R (*((volatile unsigned long *)0x400FE604))
#define RCGC2_GPIO_R (*((volatile unsigned long *)0x400FE108))

#define SYS_CLOCK_FREQUENCY 16000000
#define GPTM_CONFIG_R2 (*(( volatile long *)(TM_BASE2 + 0x000)))// 16bit or 32bit
#define GPTM_TA_MODE_R2 (*(( volatile long *)( TM_BASE2 + 0x008 )))//perodic or pwm
#define GPTM_CONTROL_R2 (*(( volatile long *)( TM_BASE2 + 0x00C )))//select A or B
#define GPTM_INT_MASK_R2 (*(( volatile long *)( TM_BASE2 + 0x018 )))//intrupt
#define GPTM_INT_CLEAR_R2 (*(( volatile long *)( TM_BASE2 + 0x024 )))//intrupt clear
#define GPTM_TA_IL_R2  (*(( volatile long *)( TM_BASE2 + 0x02C)))// Reload
#define GPTM_TA_MATCH_R2 (*(( volatile long *)( TM_BASE2 + 0x034 )))//Pwm Duty Cycle
// GPIO PF2 alternate function configuration
#define TIM_A_INTERVAL2 16000
#define TIM_A_MATCH2 12000

// Timer and GPIO intialization and configuration


void PortA_Init()
{
	SYSCTL_RCGCGPIO_R |= 0x01;
	GPIO_PORTA_DIR_R &= ~(0xFF);
	GPIO_PORTA_DEN_R |= 0x7C;
}
	
void Timer1A_Init ( void )
{
	
// Enable the clock for port F and Timer1
RCGC2_GPIO_R |= CLOCK_GPIOB ;
RCGC_TIMER_R |= 0x02 ;
// Configure PortF pin 2 as Timer1 A output
GPIO_PORTB_AFSEL_R |= 0x00000030 ;
GPIO_PORTB_PCTL_R |= 0x00770000 ; 
GPIO_PORTB_DEN_R |= 0x00000030 ;
GPTM_CONTROL_R &= ~( TIM_A_ENABLE );
GPTM_CONTROL_R2 &= ~( TIM_B_ENABLE );
// Timer1 A configured as 16- bit timer
GPTM_CONFIG_R |= TIM_16_BIT_CONFIG ;
GPTM_CONFIG_R2 |= TIM_16_BIT_CONFIG ;
// Timer1 A in periodic timer , edge count and PWM mode
GPTM_TA_MODE_R |= TIM_PWM_MODE ;
GPTM_TA_MODE_R2 |= TIM_PWM_MODE ;
GPTM_TA_MODE_R &= ~( TIM_CAPTURE_MODE );
GPTM_TA_MODE_R2 &= ~( TIM_CAPTURE_MODE );
// Make PWM frequency 1 kHz using reload value of 16000
GPTM_TA_IL_R = TIM_A_INTERVAL ;
GPTM_TA_IL_R2 = TIM_A_INTERVAL2 ;
// Configure PWM duty cycle value ( should be less than 16000)
GPTM_TA_MATCH_R = TIM_A_MATCH ;
GPTM_TA_MATCH_R2 = TIM_A_MATCH2 ;
// Enable timer1 A
GPTM_CONTROL_R |= TIM_A_ENABLE ;
GPTM_CONTROL_R2 |= TIM_B_ENABLE ;

}


	 
	
// Application main function
int main ( void ){
	
int Kp=700,Kd=300,Ki=20;   //750,30 perfect at  12000
//int Kp=450,Kd=70,Ki=0;
int pos = 3;
int error = 0;   //derivative, integral errors are initalized to zero 
int dev = 0;
int inte = 0;
int setpoint = 3;  //for going straight
int motor = 0;
int j=3;  //flag for going straight
//int prev=0;
int previouserror=0;
	
	Timer1A_Init (); // Initialize the timer
PortA_Init();
	//	if(error==previouserror)
	//	flag=0;
		
//	if(error!=previouserror)
	//	flag=1;
while (1) {

	

	if ((GPIO_PORTA_DATA_R) == 0x44){
		pos=3;

}
	if ((GPIO_PORTA_DATA_R) == 0x0C){
		pos=4;

}
	if ((GPIO_PORTA_DATA_R) == 0x60){
		pos=2;

}
	if ((GPIO_PORTA_DATA_R) == 0x78){
		pos=0;
	}
	if ((GPIO_PORTA_DATA_R) == 0x3C){
		pos=6;

}
	if ((GPIO_PORTA_DATA_R) == 0x70){
		pos=1;

}
	if ((GPIO_PORTA_DATA_R) == 0x1C){
		pos=5;
	}

	if(pos!=j){
		error=setpoint-pos;
	dev=error-previouserror;
	inte=inte+error;
	motor=(Kp*error)+(Kd*dev)+(Ki*inte);
	if(motor>3999)  
		//check for motor error if it goes above the interval value
		motor=3999;
	if(motor<-3999)
		motor=-3999;//
	if(pos==0)   //for sharp turns
		motor=3999;
	if(pos==6)
		motor=-3999;
	previouserror=error;
}
	
	GPTM_TA_MATCH_R  = basespeed-motor;
	GPTM_TA_MATCH_R2 = basespeed+motor;

	j=pos;  //this flag will be updated after each iteration!

}
}
