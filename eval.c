# define SYSCTL_RCGC2_R *(( volatile unsigned long *)0x400FE108 )
# define SYSCTL_RCGC2_GPIOF 0x00000020 // Port F clock control
# define GPIO_PORTF_DATA_RD *(( volatile unsigned long *)0x40025044 )
# define GPIO_PORTF_DATA_WR *(( volatile unsigned long *)0x400253FC )
# define GPIO_PORTF_DIR_R *(( volatile unsigned long *)0x40025400 )
# define GPIO_PORTF_DEN_R *(( volatile unsigned long *)0x4002551C )
# define GPIO_PORTF_PUR_R *(( volatile unsigned long *)0x40025510 )
# define NVIC_EN0_INT30 0x40000000 // Interrupt 30 enable

// IRQ 0 to 31 Set Enable Register
# define NVIC_EN0_R *(( volatile unsigned long *)0xE000E100 )
// IRQ 28 to 31 Priority Register
# define NVIC_PRI7_R *(( volatile unsigned long *)0xE000E41C )
# define GPIO_PORTF_IS_R *(( volatile unsigned long *)0x40025404 )
# define GPIO_PORTF_IBE_R *(( volatile unsigned long *)0x40025408 )
# define GPIO_PORTF_IEV_R *(( volatile unsigned long *)0x4002540C )
# define GPIO_PORTF_IM_R *(( volatile unsigned long *)0x40025410 )
# define GPIO_PORTF_ICR_R *(( volatile unsigned long *)0x4002541C )

// Port pin definitions
# define GPIO_PORTF_PIN3 0x08
# define GPIO_PORTF_PIN2 0x04
# define GPIO_PORTF_PIN4 0x10
# define GPIO_PORTF_PIN0 0x01

// Default clock frequency and delay definition
# define SYSTEM_CLOCK_FREQUENCY 16000000
# define DEALY_COUNT SYSTEM_CLOCK_FREQUENCY /1000

// Function prototypes
void Delay ( unsigned long counter );

// User main program
int main ()
{
// Enable the clock for port F
SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
	
// Configure PF3 as digital output and PF4 as digitalinput
GPIO_PORTF_DEN_R |= GPIO_PORTF_PIN0 + GPIO_PORTF_PIN2 + GPIO_PORTF_PIN3 + GPIO_PORTF_PIN4 ;
	
GPIO_PORTF_DIR_R |= GPIO_PORTF_PIN3 + GPIO_PORTF_PIN2 ; // PF3 as output
	
GPIO_PORTF_DIR_R &= (~ GPIO_PORTF_PIN4 ) + (~ GPIO_PORTF_PIN0 ); // PF4 as input
GPIO_PORTF_PUR_R |= GPIO_PORTF_PIN4 + GPIO_PORTF_PIN0; // Enable pull -upon PF4
GPIO_PORTF_IS_R &= (~ GPIO_PORTF_PIN4 )+ (~ GPIO_PORTF_PIN0 ); // PF4 is edge -sensitive
GPIO_PORTF_IBE_R &= (~ GPIO_PORTF_PIN4 ) + (~ GPIO_PORTF_PIN0 ); // PF4 is notboth edges
GPIO_PORTF_IEV_R &= (~ GPIO_PORTF_PIN4 ) + (~ GPIO_PORTF_PIN0 ); // PF4 fallingedge
// event
GPIO_PORTF_ICR_R = GPIO_PORTF_PIN4 + GPIO_PORTF_PIN0; // Clear flag forpin 4
GPIO_PORTF_IM_R |= GPIO_PORTF_PIN4 + GPIO_PORTF_PIN0; // Unmask interruptfor PF4
	
NVIC_PRI7_R |= 0x00A00000 ; // Priority is 5
NVIC_EN0_R = NVIC_EN0_INT30 ; // Enable INT 30 in NVIC
	
while (1)
{
}
}

// Interrupt service routine for Port F
void GPIOPortF_Handler ( void ){
	
// Insert delay for switch debouncing
Delay ( DEALY_COUNT );
if (( GPIO_PORTF_DATA_RD ) == 0)
{
GPIO_PORTF_DATA_WR ^= GPIO_PORTF_PIN3 ;
GPIO_PORTF_ICR_R = 0x10; // acknowledge flag4
}


if (( GPIO_PORTF_DATA_RD ) == 0)
{
GPIO_PORTF_DATA_WR ^= GPIO_PORTF_PIN2 ;
GPIO_PORTF_ICR_R = 0x01; // acknowledge flag
}

}

// This function implements the delay
void Delay ( unsigned long counter )
{
unsigned long i = 0;
for(i=0; i< counter ; i++);
}