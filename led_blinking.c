ANSEL = 0;
ANSELH = 0;

TRISA = 0;
TRISB = 0;
TRISC = 0;
TRISD = 0;

while(1)
{
	PORTA = 0;
	PORTB = 0;
	PORTC = 0;
	PORTD = 0;

	Delay_ms(250);

	PORTA = 255;
	PORTB = 255;
	PORTC = 255;
	PORTD = 255;

	Delay_ms(250);		
}