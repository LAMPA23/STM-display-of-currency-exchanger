#include "main_header.h"

volatile unsigned long delay_counter = 0;

volatile unsigned long UAH_to_USD = 3523;// 1 
volatile unsigned long USD_to_UAH = 3424;// 2
volatile unsigned long UAH_to_EUR = 3515;// 3
volatile unsigned long EUR_to_UAH = 3476;// 4

volatile unsigned char change_mod = 0;



// #################################################
// #################################################
void interrupt_init(void)
{	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI11_PC;
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PC;
	
	EXTI->IMR |= EXTI_IMR_IM0;
	EXTI->IMR |= EXTI_IMR_IM11;
	EXTI->IMR |= EXTI_IMR_IM8;
	
	EXTI->RTSR |= EXTI_RTSR_TR0;
	EXTI->FTSR |= EXTI_RTSR_TR11;
	EXTI->FTSR |= EXTI_RTSR_TR8;
	
	NVIC_SetPriority(EXTI0_IRQn,1);
	NVIC_SetPriority(EXTI15_10_IRQn,2);
	NVIC_SetPriority(EXTI9_5_IRQn,3);
	
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	
	__enable_irq();
}

void EXTI0_IRQHandler(void) 
{
	EXTI->IMR &= ~EXTI_IMR_IM0;
	
	change_mod++;
	if(change_mod > 4) change_mod = 0;
	lcd_print();
	
	EXTI->PR|= EXTI_PR_PR0;
	EXTI->IMR |= EXTI_IMR_IM0;
}

void EXTI15_10_IRQHandler(void)  
{
	EXTI->IMR &= ~EXTI_IMR_IM11;
	
	if(change_mod!=0)
	{
		switch(change_mod)
		{
			case 1:
				UAH_to_USD++;
			break;
			case 2:
				USD_to_UAH++;
			break;
			case 3:
				UAH_to_EUR++;
			break;
			case 4:
				EUR_to_UAH++;
			break;
		}
	}
	lcd_print();
	
	EXTI->PR|= EXTI_PR_PR11;
	EXTI->IMR |= EXTI_IMR_IM11;
}

void EXTI9_5_IRQHandler(void) 
{
	EXTI->IMR &= ~EXTI_IMR_IM9;
	
	if(change_mod!=0)
	{
		switch(change_mod)
		{
			case 1:
				UAH_to_USD--;
			break;
			case 2:
				USD_to_UAH--;
			break;
			case 3:
				UAH_to_EUR--;
			break;
			case 4:
				EUR_to_UAH--;
			break;
		}
	}
	lcd_print();
	
	EXTI->PR|= EXTI_PR_PR9;
	EXTI->IMR |= EXTI_IMR_IM9;
}





// #################################################
// #################################################
void lcd_print(void)
{
	
	lcd_cmd(0x01);// clear
	lcd_data(0x55);
	lcd_data(0x53);
	lcd_data(0x44);
	
	
	lcd_cmd(0x84);// mov 1
	if(UAH_to_USD <= 9999) 
	{
		lcd_data(0x30 + (UAH_to_USD - (UAH_to_USD%1000))/1000);
		lcd_data(0x30 + ((UAH_to_USD - (UAH_to_USD%100))/100)%10);
		lcd_data(0xD0);
		lcd_data(0x30 + ((UAH_to_USD - (UAH_to_USD%10))/10)%10);
		lcd_data(0x30 + (UAH_to_USD%10));
	}
	else 
	{
		lcd_data(0x78);
		lcd_data(0x78);
		lcd_data(0xD0);
		lcd_data(0x78);
		lcd_data(0x78);
	}
	
	lcd_cmd(0x8B);// mov 2
	if( USD_to_UAH <= 9999) 
	{
		lcd_data(0x30 + (USD_to_UAH - (USD_to_UAH%1000))/1000);
		lcd_data(0x30 + ((USD_to_UAH - (USD_to_UAH%100))/100)%10);
		lcd_data(0xD0);
		lcd_data(0x30 + ((USD_to_UAH - (USD_to_UAH%10))/10)%10);
		lcd_data(0x30 + (USD_to_UAH%10));
	}
	else 
	{
		lcd_data(0x78);
		lcd_data(0x78);
		lcd_data(0xD0);
		lcd_data(0x78);
		lcd_data(0x78);
	}
	
	lcd_cmd(0xC0);// mov 
	lcd_data(0x45);
	lcd_data(0x55);
	lcd_data(0x52);
	
	lcd_cmd(0xC4);// mov 3
	if( UAH_to_EUR <= 9999) 
	{
		lcd_data(0x30 + (UAH_to_EUR - (UAH_to_EUR%1000))/1000);
		lcd_data(0x30 + ((UAH_to_EUR - (UAH_to_EUR%100))/100)%10);
		lcd_data(0xD0);
		lcd_data(0x30 + ((UAH_to_EUR - (UAH_to_EUR%10))/10)%10);
		lcd_data(0x30 + (UAH_to_EUR%10));
	}
	else 
	{
		lcd_data(0x78);
		lcd_data(0x78);
		lcd_data(0xD0);
		lcd_data(0x78);
		lcd_data(0x78);
	}
	
	lcd_cmd(0xCB);// mov 4
	if( UAH_to_EUR <= 9999) 
	{
		lcd_data(0x30 + (EUR_to_UAH - (EUR_to_UAH%1000))/1000);
		lcd_data(0x30 + ((EUR_to_UAH - (EUR_to_UAH%100))/100)%10);
		lcd_data(0xD0);
		lcd_data(0x30 + ((EUR_to_UAH - (EUR_to_UAH%10))/10)%10);
		lcd_data(0x30 + (EUR_to_UAH%10));
	}
	else 
	{
		lcd_data(0x78);
		lcd_data(0x78);
		lcd_data(0xD0);
		lcd_data(0x78);
		lcd_data(0x78);
	}
	
	if(change_mod!=0)
	{
		switch(change_mod)
		{
			case 1:
				lcd_cmd(0x86);// mov 1
				lcd_data(0x3F);
				lcd_cmd(0x86);// mov 1
			break;
			case 2:
				lcd_cmd(0x8D);// mov 2
				lcd_data(0x3F);
				lcd_cmd(0x8D);// mov 2
			break;
			case 3:
				lcd_cmd(0xC6);// mov 3
				lcd_data(0x3F);
				lcd_cmd(0xC6);// mov 3
			break;
			case 4:
				lcd_cmd(0xCD);// mov 4
				lcd_data(0x3F);
				lcd_cmd(0xCD);// mov 4
			break;
		}
	}
	
	my_delay(1000);
}

void lcd_init(void)
{
	lcd_cmd(0x28);// init
	lcd_cmd(0x01);// clear
	lcd_cmd(0x0D);// cursor on
	my_delay(1000);
}

void lcd_cmd(unsigned long cmd)
{
	GPIOE->ODR &= ~GPIO_ODR_OD7;
	GPIOE->ODR &= ~GPIO_ODR_OD11;
	
	if(cmd&(1<<4)) GPIOE->ODR |= GPIO_ODR_OD12; 
		else GPIOE->ODR &= ~GPIO_ODR_OD12;
	if(cmd&(1<<5)) GPIOE->ODR |= GPIO_ODR_OD13; 
		else GPIOE->ODR &= ~GPIO_ODR_OD13;
	if(cmd&(1<<6)) GPIOE->ODR |= GPIO_ODR_OD14; 
		else GPIOE->ODR &= ~GPIO_ODR_OD14;
	if(cmd&(1<<7)) GPIOE->ODR |= GPIO_ODR_OD15; 
		else GPIOE->ODR &= ~GPIO_ODR_OD15;

	GPIOE->ODR |= GPIO_ODR_OD11;
	my_delay(400);
	GPIOE->ODR &= ~GPIO_ODR_OD11;
	my_delay(400);
	
	if(cmd&(1<<0)) GPIOE->ODR |= GPIO_ODR_OD12; 
		else GPIOE->ODR &= ~GPIO_ODR_OD12;
	if(cmd&(1<<1)) GPIOE->ODR |= GPIO_ODR_OD13; 
		else GPIOE->ODR &= ~GPIO_ODR_OD13;
	if(cmd&(1<<2)) GPIOE->ODR |= GPIO_ODR_OD14; 
		else GPIOE->ODR &= ~GPIO_ODR_OD14;
	if(cmd&(1<<3)) GPIOE->ODR |= GPIO_ODR_OD15; 
		else GPIOE->ODR &= ~GPIO_ODR_OD15;
	
	GPIOE->ODR |= GPIO_ODR_OD11;
	my_delay(400);
	GPIOE->ODR &= ~GPIO_ODR_OD11;	
	my_delay(4000);
}

void lcd_data(unsigned long cmd)
{
	GPIOE->ODR |= GPIO_ODR_OD7;
	GPIOE->ODR &= ~GPIO_ODR_OD11;
	
	if(cmd&(1<<4)) GPIOE->ODR |= GPIO_ODR_OD12; 
		else GPIOE->ODR &= ~GPIO_ODR_OD12;
	if(cmd&(1<<5)) GPIOE->ODR |= GPIO_ODR_OD13; 
		else GPIOE->ODR &= ~GPIO_ODR_OD13;
	if(cmd&(1<<6)) GPIOE->ODR |= GPIO_ODR_OD14; 
		else GPIOE->ODR &= ~GPIO_ODR_OD14;
	if(cmd&(1<<7)) GPIOE->ODR |= GPIO_ODR_OD15; 
		else GPIOE->ODR &= ~GPIO_ODR_OD15;

	GPIOE->ODR |= GPIO_ODR_OD11;
	my_delay(400);
	GPIOE->ODR &= ~GPIO_ODR_OD11;
	my_delay(400);
	
	if(cmd&(1<<0)) GPIOE->ODR |= GPIO_ODR_OD12; 
		else GPIOE->ODR &= ~GPIO_ODR_OD12;
	if(cmd&(1<<1)) GPIOE->ODR |= GPIO_ODR_OD13; 
		else GPIOE->ODR &= ~GPIO_ODR_OD13;
	if(cmd&(1<<2)) GPIOE->ODR |= GPIO_ODR_OD14; 
		else GPIOE->ODR &= ~GPIO_ODR_OD14;
	if(cmd&(1<<3)) GPIOE->ODR |= GPIO_ODR_OD15; 
		else GPIOE->ODR &= ~GPIO_ODR_OD15;
	
	GPIOE->ODR |= GPIO_ODR_OD11;
	my_delay(400);
	GPIOE->ODR &= ~GPIO_ODR_OD11;	
	my_delay(4000);
}




// #################################################
// #################################################
void GPIO_init(void)
{
	// AHB1ENR
	RCC->AHB1ENR = RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR = RCC_AHB1ENR_GPIOCEN;
	RCC->AHB1ENR = RCC_AHB1ENR_GPIOEEN;

		
	// MODER A
	GPIOC->MODER &= ~GPIO_MODER_MODE8_0;
	GPIOC->MODER &= ~GPIO_MODER_MODE8_1;
	GPIOC->MODER &= ~GPIO_MODER_MODE9_0;
	GPIOC->MODER &= ~GPIO_MODER_MODE9_1;
	GPIOC->MODER &= ~GPIO_MODER_MODE11_0;
	GPIOC->MODER &= ~GPIO_MODER_MODE11_1;
	
	
	// MODER A
	GPIOA->MODER &= ~GPIO_MODER_MODE0_0;
	GPIOA->MODER &= ~GPIO_MODER_MODE0_1;	
	
	// MODER E
	GPIOE->MODER |= GPIO_MODER_MODE7_0;
	GPIOE->MODER &= ~GPIO_MODER_MODE7_1;
	GPIOE->MODER |= GPIO_MODER_MODE10_0;
	GPIOE->MODER &= ~GPIO_MODER_MODE10_1;
	GPIOE->MODER |= GPIO_MODER_MODE11_0;
	GPIOE->MODER &= ~GPIO_MODER_MODE11_1;
	GPIOE->MODER |= GPIO_MODER_MODE12_0;
	GPIOE->MODER &= ~GPIO_MODER_MODE12_1;
	GPIOE->MODER |= GPIO_MODER_MODE13_0;
	GPIOE->MODER &= ~GPIO_MODER_MODE13_1;
	GPIOE->MODER |= GPIO_MODER_MODE14_0;
	GPIOE->MODER &= ~GPIO_MODER_MODE14_1;
	GPIOE->MODER |= GPIO_MODER_MODE15_0;
	GPIOE->MODER &= ~GPIO_MODER_MODE15_1;
	
	
	// OTYPER E
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT7;
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT10;
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT11;
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT12;
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT13;
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT14;
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT15;
	
	
	// ODR
	GPIOE->ODR &= ~GPIO_ODR_OD10;
}

void my_delay(unsigned long delay)
{	
	delay_counter = 0;
	for(delay_counter=0;delay_counter<delay;delay_counter++);
}

