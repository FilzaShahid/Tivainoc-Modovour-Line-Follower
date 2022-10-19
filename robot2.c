void SystemInit()
{}

#define TM_BASE 0x40031000
#define RCGC_TIMER_R 	(*((volatile unsigned long *)0x400FE604))
#define RCGC2_GPIO_R   (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGCGPIO_R *((volatile unsigned long *) 0x400FE108)
#define CLOCK_GPIOF  0x00000020
#define SYS_CLOCK_FREQUENCY  16000000


#define GPTM_CONFIG_R    *(volatile long *)(TM_BASE+0x000)
#define GPTM_TA_MODE_R  *(volatile long *)(TM_BASE +0x004)
#define GPTM_TB_MODE_R  *(volatile long *)(TM_BASE +0x008)
#define GPTM_CONTROL_R   *(volatile long *)(TM_BASE +0x00C)
#define GPTM_INT_MASK_R   *(volatile long *)(TM_BASE +0x018)
#define GPTM_INT_CLEAR_R  *(volatile long *)(TM_BASE +0x024)
#define GPTM_TA_IL_R  	*(volatile long *)(TM_BASE +0x028)
#define GPTM_TB_IL_R    *(volatile long *)(TM_BASE +0x02C)
#define GPTM_TA_MATCH_R  *(volatile long *)(TM_BASE +0x030)
#define GPTM_TB_MATCH_R  *(volatile long *)(TM_BASE +0x034)

#define GPIO_PORTF_AFSEL_R  *((volatile unsigned long *) 0x40025420)
#define GPIO_PORTA_DATA_R  *((volatile unsigned long *) 0x400043FC)
#define GPIO_PORTA_DIR_R   *((volatile unsigned long *) 0x40004400)
#define GPIO_PORTA_DEN_R   *((volatile unsigned long *) 0x4000451C)
#define GPIO_PORTF_PCTL_R  *((volatile unsigned long *) 0x4002552C)
#define GPIO_PORTF_DEN_R  *((volatile unsigned long *) 0x4002551C)

#define TIM_16_BIT_CONFIG  0x00000004
#define TIM_PERIODIC_MODE  0x00000002
#define TIM_A_ENABLE  0x00000001
#define TIM_B_ENABLE  0x00000100

#define TIM_PWM_MODE 0x0000000A
#define TIM_CAPTURE_MODE  0x00000004

#define TIM_A_INTERVAL  100
#define TIM_B_INTERVAL  100

int MOTOR_A;
int MOTOR_B;

void Timer1AB_Init(void)
{
  
	RCGC2_GPIO_R |= CLOCK_GPIOF;
	RCGC_TIMER_R |= 0x02;
	
	GPIO_PORTF_AFSEL_R |= 0x0C;
	GPIO_PORTF_DEN_R |= 0x0C;
	GPIO_PORTF_PCTL_R |= 0x00007700;
	
	GPTM_CONTROL_R &= ~(TIM_A_ENABLE);
	GPTM_CONFIG_R |= TIM_16_BIT_CONFIG;
	
	GPTM_TA_MODE_R |= TIM_PWM_MODE;
	GPTM_TA_MODE_R &= ~(TIM_CAPTURE_MODE);
	GPTM_TA_IL_R = TIM_A_INTERVAL;
	GPTM_CONTROL_R |= TIM_A_ENABLE;
	
	
	GPTM_TA_MATCH_R = MOTOR_A;
	
	GPTM_CONTROL_R &= ~(TIM_B_ENABLE);
	GPTM_CONFIG_R |= TIM_16_BIT_CONFIG;
	
	GPTM_TB_MODE_R |= TIM_PWM_MODE;
	GPTM_TB_MODE_R &= ~(TIM_CAPTURE_MODE);
	GPTM_TB_IL_R = TIM_B_INTERVAL;
	GPTM_CONTROL_R |= TIM_B_ENABLE;
	
	
	GPTM_TB_MATCH_R = MOTOR_B;
}

void delay1(unsigned long value)
{
	unsigned long ulLoop;
	{
		for (ulLoop=0;ulLoop<value;ulLoop++);
	}
}

//void PortA_Handler()



void GPIO_PORTA_Init()
{
	SYSCTL_RCGCGPIO_R |= 0x01;
	GPIO_PORTA_DIR_R |= 0x00;
	GPIO_PORTA_DEN_R |= 0x7C;
}
int main(void)
{

	int MOTOR_A;
	int MOTOR_B;
	Timer1AB_Init();
	//Timer1B_Init();
	
	while(1)
	{
			if ((GPIO_PORTA_DATA_R & 0x0E)== 0x0E) //straight
			{//ontrack
				MOTOR_A = 81;
				MOTOR_B = 87;
			}
			if ((GPIO_PORTA_DATA_R & 0x03) == 0x03) //turning left
			{
				MOTOR_A= 61;
				MOTOR_B= 87;
			}
			if ((GPIO_PORTA_DATA_R & 0x18) == 0x18) //turning right
			{
				MOTOR_A= 81;
				MOTOR_B= 61;
			}
			if ((GPIO_PORTA_DATA_R & 0x1C) == 0x1C) //turnsharpright
			{
				MOTOR_A= 81;
				MOTOR_B= 51;
			}
			if ((GPIO_PORTA_DATA_R & 0x07)==0x07) //turnsharpleft
			{
				MOTOR_A= 51;
				MOTOR_B= 87;
			}
			if ((GPIO_PORTA_DATA_R & 0x0C)==0x0C) //ifslightright
			{
				MOTOR_A= 91;
				MOTOR_B= 87;
			}
			if ((GPIO_PORTA_DATA_R & 0x06 )== 0x06) //ifslightleft
			{
				MOTOR_A= 81;
				MOTOR_B= 91;
			}
			if ((GPIO_PORTA_DATA_R & 0x7C) == 0x7C)
  		{
				MOTOR_A= 95;
				MOTOR_B= 95;
			}
			
			GPTM_TA_MATCH_R = MOTOR_A;
			GPTM_TB_MATCH_R = MOTOR_B;
	}
}
	
