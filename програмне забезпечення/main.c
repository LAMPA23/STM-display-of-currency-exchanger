#include "main_header.h"

int main()
{
	GPIO_init();
	interrupt_init();
	lcd_init();	
	while(1);
}