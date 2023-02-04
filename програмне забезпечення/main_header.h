#include <stm32f407xx.h>

extern volatile unsigned long delay_counter;

extern volatile unsigned long UAH_to_USD;// 1 
extern volatile unsigned long USD_to_UAH;// 2
extern volatile unsigned long UAH_to_EUR;// 3
extern volatile unsigned long EUR_to_UAH;// 4

extern volatile unsigned char change_mod;



//-----------------for lcd
void lcd_init(void);
void lcd_cmd(unsigned long cmd);
void lcd_data(unsigned long cmd);
void lcd_print(void);


//-----------------for interrupt
void interrupt_init(void);
void EXTI0_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void EXTI9_5_IRQHandler(void);


//-----------------for other
void GPIO_init(void);
void my_delay(unsigned long delay);
