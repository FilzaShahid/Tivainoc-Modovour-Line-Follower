//Register definitions for ClockEnable
#define 		SYSCTLRCGCUARTR			(*((volatile unsigned long*)0x0x400FE618))
#define 		SYSCTLRCGCGPIOR			(*((volatile unsigned long*)0x0x400FE608))

//Register definitions for GPIOPortD
#define 	GPIOPORTDAFSELR			(*((volatile unsigned long*)0x40007420))
#define 	GPIOPORTDPCTLR			(*((volatile unsigned long*)0x4000752C))
#define 	GPIOPORTDDENR				(*((volatile unsigned long*)0x4000751C))
#define 	GPIOPORTDDIRR				(*((volatile unsigned long*)0x40007400))
#define 	GPIOPORTDLOCKR			(*((volatile unsigned long*)0x40007520))
#define 	GPIOPORTDCRR				(*((volatile unsigned long*)0x40007524))

//Register definitions for UART2 module
#define 	UART2CTLR						(*((volatile unsigned long*)0x4000E030))
#define  	UART2IBRDR					(*((volatile unsigned long*)0x4000E024))
#define  	UART2FBRDR					(*((volatile unsigned long*)0x4000E028))
#define 	UART2LCRHR					(*((volatile unsigned long*)0x4000E02C))
#define 	UART2CCR						(*((volatile unsigned long*)0x4000EFC8))
#define 	UART2FRR						(*((volatile unsigned long*)0x4000E018))
#define 	UART2DRR						(*((volatile unsigned long*)0x4000E000))

//Macros for initialization and configuration of UART2
#define 	UART2CLKEN					0x00000004								//Enable clock for UART2
#define 	GPIOPORTDCLKEN			0x00000008								//Enable clock for GPIO PORTD

#define GPIOPORTDUART2CFG			0x000000C0								//Digital enable
//Activate alternate function for PD6 and PD7
#define 	GPIOPCTLPD6U2RX			0x01000000								//Configure PD6 as U2RX
#define 	GPIOPCTLPD7U2TX			0x10000000								//Configure PD7 as U2TX
#define 	GPIOPORTDUNLOCKCR		0x4C4F434B								//Unlock Commit register
#define 	GPIOPORTDCREN				0x000000FF								//Disable write protection

#define 	UARTCSSysClk				0x00000000								//Use system as UART clock
#define 	UARTCSPIOSC					0x00000005								//Use PIOSC as UART clock
#define 	UARTLCRHWLEN8				0x00000060								//8bit word length
#define 	UARTLCRHFEN					0x00000010								//Enable UART FIFOs
#define 	UARTFRTXFF					0x00000020								//UART Transmit FIFO Full
#define 	UARTFRRXFE					0x00000010								//UART Receive FIFO Empty
#define 	UARTCTLUARTEN				0x00000001								//Enable UART
#define 	UARTLBEN					  0x00000080								//Use UART in Loop back mode

//// addresses for lcd display registers
//#define SYSCTL_RCGC_GPIO_R  *((volatile unsigned long *)0x400FE608)

//// register definitions for Port B, which is used as LCD data bus
//#define GPIO_PORTB_DATA_R  *((volatile unsigned long *)0x400053FC) 
//#define GPIO_PORTB_DIR_R   *((volatile unsigned long *)0x40005400)
//#define GPIO_PORTB_DEN_R   *((volatile unsigned long *)0x4000551C)

///* register definitions for Port E (pins 3, 4 and 5), which 
//is used as LCD control bus */
//#define GPIO_PORTE_DATA_R  *((volatile unsigned long *)0x400243FC) 
//#define GPIO_PORTE_DIR_R   *((volatile unsigned long *)0x40024400)
//#define GPIO_PORTE_DEN_R   *((volatile unsigned long *)0x4002451C)

//// LCD module control pins and data bus definitions 
//#define LCD_E_PIN          0x08
//#define LCR_RW_PIN         0x10
//#define LCD_RS_PIN         0x20
//#define LCD_DATA_BUS       GPIO_PORTB_DATA_R
//#define LCD_CONTROL_BUS    GPIO_PORTE_DATA_R


//Function definitions
unsigned char UARTRx(void);
void UARTTx(unsigned char data);
void UARTTxString(char*pt);
void UARTRxString(char*bufPt, unsigned short max);

//Intialize and configure UART
void UARTInit(void)
{
	//Enable clock for UART2 and GPIO PortD
	SYSCTLRCGCUARTR |= UART2CLKEN;						//Activate UART2
	SYSCTLRCGCGPIOR |= GPIOPORTDCLKEN;				//Activate PortD

	//Configuration to usePD6 and PD7 as UART
	GPIOPORTDLOCKR = GPIOPORTDUNLOCKCR;					//Unlock commit register
	GPIOPORTDCRR 	|= GPIOPORTDCREN;							//Enable U2Tx on PD7
	GPIOPORTDDENR |= GPIOPORTDUART2CFG;					//Enable digital I/O onPD6

	GPIOPORTDAFSELR |= GPIOPORTDUART2CFG;				//Enable alt.func. on PD6 and PD7
	GPIOPORTDPCTLR = (GPIOPCTLPD6U2RX | GPIOPCTLPD7U2TX);

	//ConfigurationofUART2module
	UART2CTLR &= ~UARTCTLUARTEN;							//Disable UART
	//IBRD=int(16,000,000/(16*9600))=int(104.167)
	UART2IBRDR=104;
	//FBRD=int(0.6805*64+0.5)=44
	UART2FBRDR=44;
	//8bit word length,no parity bit,one stop bit,FIFOs enable
	UART2LCRHR = (UARTLCRHWLEN8 | UARTLCRHFEN);
	UART2CCR = UARTCSSysClk;								//Use system clock as UART clock
	//UART2CTLR|=UARTLBEN;									//Enable loop back mode
	UART2CTLR |= UARTCTLUARTEN;							//Enable UART2

}

//Wait for input and returns its ASCII value
unsigned char UARTRx(void)
{
	while((UART2FRR & UARTFRRXFE)!=0);
	return((unsigned char)(UART2DRR & 0xFF));
}

/*Accepts ASCII characters from the serial port and
adds them to a string.It echoes each character as it
is inputted.*/
void UARTRxString (char *pt, unsigned short max)
{
	int length=0;
	char character;

	character = UARTRx();
	if(length<max)
	{
		*pt=character;
		pt++;
		length++;
		UARTTx(character+1);
	}

	*pt=0;
}

//Output 8bit to serial port
void UARTTx(unsigned char data)
{
	while((UART2FRR & UARTFRTXFF)!=0);
	UART2DRR=data;
}


//Output a character string to serial port
void UARTTxString(char *pt)
{
	while(*pt)
	{
		UARTTx(*pt);
		pt++;
	}
}


int main(void)
{

	char string[17];

	UARTInit();

	//The input given using keyboard is displayed on hyper terminal
	//.i.e.,data is echoed
	UARTTxString("EnterText:");

while(1)
	{
		UARTRxString(string,16);
	}
}