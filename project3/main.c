#define RCGCGPIO (*((volatile unsigned long *) 0x400FE608))
#define RCGCUART (*((volatile unsigned long *) 0x400FE618))

#define GPIO_PORTA_LOCK (*((volatile unsigned long *) 0x40004520))////
#define GPIO_PORTA_CR (*((volatile unsigned long *) 0x40004524))////
#define GPIO_PORTA_PUR (*((volatile unsigned long *) 0x40004514))

#define GPIO_PORTA_EN (*((volatile unsigned long *) 0x4000451C))
#define GPIO_PORTA_DIR (*((volatile unsigned long *) 0x40004400))
#define GPIO_PORTA_DATA (*((volatile unsigned long *) 0x40004070)) //1110000

#define GPIO_PORTA_IS (*((volatile unsigned long *) 0x40004404))
#define GPIO_PORTA_EV (*((volatile unsigned long *) 0x4000440C))
#define GPIO_PORTA_IM (*((volatile unsigned long *) 0x40004410))
//#define GPIO_PORTA_BE (*((volatile unsigned long *) 0x40004408))////
//#define GPIO_PORTA_ICR (*((volatile unsigned long *) 0x4000441C))////
#define GPIO_PORTA_MIS (*((volatile unsigned long *) 0x40004418))////

#define PORTA_ISEN0 (*((volatile unsigned long *) 0xE000E100))//interrupt set enable, EN0 offset 100
#define PORTA_IP0 (*((volatile unsigned long *) 0xE000E400))

////////////////////////////////////////////////////////////////////////////////////////////////

#define RCGCTIMER (*((volatile unsigned long *) 0x400FE604))

#define GPTMCTL (*((volatile unsigned long *) 0x4003000C))//enable is 1 disable is 0
#define GPTMCFG (*((volatile unsigned long *) 0x40030000))//4 is 16 bit
#define GPTMTAMR (*((volatile unsigned long *) 0x40030004))
#define GPTMTAPR (*((volatile unsigned long *) 0x40030038))
#define GPTMTAILR (*((volatile unsigned long *) 0x40030028))
#define GPTMRIS (*((volatile unsigned long *) 0x4003001C))
#define GPTMICR (*((volatile unsigned long *) 0x40030024))
#define GPTMIM (*((volatile unsigned long *) 0x40030018))


/////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char keypad[4][4] = {{'1' ,'2' ,'3' ,'A' }, {'4' ,'5' ,'6' ,'B' }, {'7' ,'8' ,'9' ,'C' }, {'*' ,'0' ,'#' ,'D' }};

#define GPIOCR_PORTE (*((volatile unsigned long *) 0x40024524))
#define GPIOCR_PORTC (*((volatile unsigned long *) 0x40006524))
#define GPIOPDR_PORTE (*((volatile unsigned long *) 0x40024514))
#define GPIOPDR_PORTC (*((volatile unsigned long *) 0x40006514))
#define GPIOEN_PORTE (*((volatile unsigned long *) 0x4002451C))
#define GPIOEN_PORTC (*((volatile unsigned long *) 0x4000651C))
#define GPIODATA_PORTE (*((volatile unsigned long *) 0x4002403C))
#define GPIODATA_PORTC (*((volatile unsigned long *) 0x400063C0))
#define GPIOLOCK_PORTE (*((volatile unsigned long *) 0x40024520))
#define GPIOLOCK_PORTC (*((volatile unsigned long *) 0x40006520))
#define GPIODIR_PORTE  (*((volatile unsigned long *) 0x40024400))
#define GPIODIR_PORTC  (*((volatile unsigned long *) 0x40006400))
char get_k(void);

#define UARTDR (*((volatile unsigned long *) 0x4000C000))
#define UARTIBRD (*((volatile unsigned long *) 0x4000C024))
#define UARTFBRD (*((volatile unsigned long *) 0x4000C028))
#define UARTCTL (*((volatile unsigned long *) 0x4000C030))
#define UARTLCRH (*((volatile unsigned long *) 0x4000C02C))
#define GPIOAFSEL (*((volatile unsigned long *) 0x40004420))
void UART_Tx(char data);



void Delay(long i);
/**
 * main.c
 */
int main(void)
{
    RCGCUART |= 0x01;
    RCGCGPIO |= 0x15;//e0c0a => 10101

    RCGCTIMER = 0x01;

    GPIO_PORTA_LOCK = 0x4C4F434B;////
    GPIO_PORTA_CR |= 0x01;////
    GPIO_PORTA_PUR = 0x10;//10000


    GPIO_PORTA_DIR = 0x0C;//01100
    GPIO_PORTA_EN = 0x1F;//11100 + 00011

    GPIO_PORTA_IS |= 0x10;//10000
    //GPIO_PORTA_BE |= 0x10;////
    GPIO_PORTA_EV |= 0x10;
    //GPIO_PORTA_ICR |= 0x10;////
    GPIO_PORTA_IM |= 0x10;

    PORTA_ISEN0 |= 0x80001;//PORTA interrupt number 0
    //PORTA_IP0 |= 0x03;

    GPIO_PORTA_DATA = 0x00;

///////////////////////////////////////////////////////////////////////////////

    GPTMCTL = 0x00;
    GPTMIM = 0x01;
    GPTMCFG = 0x04;
    GPTMTAMR = 0x02;
    GPTMTAPR = 250;
    GPTMTAILR = 60000;
    GPTMICR = 0x1;
    GPTMCTL = 0x01;

//////////////////////////////////////////////////////////////////////////////

    GPIOAFSEL = 0x03;//00011
    UARTCTL = 1;
    UARTIBRD = 104;
    UARTFBRD = 11;
    UARTLCRH = 0x60;
    UARTCTL = 0x301;//Rx Tx 00000001

///////////////////////////////////////////////////////////////////////////////

    Delay(1000);

    GPIOCR_PORTE |= 0x0F;
    GPIOCR_PORTC |= 0xF0;
    GPIODIR_PORTE |= 0x0F;
    GPIODIR_PORTC |= 0x00;
    GPIOPDR_PORTE |= 0x0F;
    GPIOPDR_PORTC |= 0xF0;
    GPIOEN_PORTE |= 0x0F;
    GPIOEN_PORTC |= 0xF0;



    GPIO_PORTA_DATA = 0x0;
    while(1){
        UART_Tx(get_k());
    }

}

GPIO_PORTA_Interrupt(void){
    GPIO_PORTA_DATA = 0x08;
    if(GPIO_PORTA_DATA & 0x08){
        GPIO_PORTA_DATA = 0x00;
    }
}

TIMER_PORTA_Interrupt(void){
    if(GPTMRIS & 0x01){
        GPIO_PORTA_DATA ^= 0x04;
        GPTMICR = 0x01;
    }
}




char get_k(void)
{
    int i = 0;
    int j = 0;
    while(1){
        for(i = 0; i < 4; i++){
            GPIODATA_PORTE = (0x01 << i);
            for(j = 0; j < 4; j++){
                if((GPIODATA_PORTC & 0xF0) == (0x10 << j))
                    return keypad[i][j];
            }
        }
    }
}



void UART_Tx(char data){
    UARTDR = data;
}

void Delay(long i){
    while(i>0)
        i=i-1;
}
