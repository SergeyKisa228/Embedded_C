/*INCLUDE*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

/*DEFINE*/
#define DEBOUNCE_TIME 50
#define ADC_MAX 1023
#define PWM_MAX 1023
#define FADE_DELAY 10

/*ENUM*/
typedef enum {
	MODE_MANUAL,
	MODE_BLINK,
	MODE_BREATHING,
	MODE_TOTAL
} operation_mode_t;

/*GLOBAL*/
static volatile uint16_t adc_value = 0;
static operation_mode_t current_mode = MODE_MANUAL;
static uint16_t blink_timer = 0;
static uint16_t fade_timer = 0;
static int16_t breathing_direction = 1;
static uint16_t breathing_value = 0;

/*PROTOTYPE FUNCTION*/
static void init_gpio(void); // Initialize GPIO pins
static void init_adc(void); // Configure ADC for potentiometer reading
static void init_timer1(void); // Setup TIM1 for PWM generation
static void init_interrupts(void); //  Configure interrupts for button and ADC
static void update_pwm(const uint16_t value); // Update PWM duty cycle value
static void handle_manual_mode(void); //  Process manual brightness control mode
static void handle_blink_mode(void); // Handle LED blinking mode operation
static void handle_breathing_mode(void); // Manage LED breathing effect mode

/*INTERRUPT SERVICE ROUTINES*/
ISR(ADC_vect) {
	adc_value = ADC;
}

ISR(INT0_vect) {
	static uint16_t last_interrupt_time = 0;
	const uint16_t current_time = blink_timer;
	
	if ((current_time - last_interrupt_time) >= DEBOUNCE_TIME) {
		current_mode = (operation_mode_t)((current_mode + 1) % MODE_TOTAL);
	}
	last_interrupt_time = current_time;
}

/*MAIN FUNCTION*/
int main(void) {
	init_gpio();
	init_adc();
	init_timer1();
	init_interrupts();
	
	sei();
	
	ADCSRA |= (1 << ADSC);
	
	while (1) {
		blink_timer++;
		fade_timer++;
		
		switch (current_mode) {
			case MODE_MANUAL:
			handle_manual_mode();
			break;
			case MODE_BLINK:
			handle_blink_mode();
			break;
			case MODE_BREATHING:
			handle_breathing_mode();
			break;
			default:
			current_mode = MODE_MANUAL;
			break;
		}
	}
}

/*FUNCTION DEFINITIONS*/
static void init_gpio(void) {
	DDRD |= (1 << DDD5);
	PORTD |= (1 << PORTD2);
}

static void init_adc(void) {
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADIE);
}

static void init_timer1(void) {
	TCCR1A = (1 << COM1A1) | (1 << WGM11) | (1 << WGM10);
	TCCR1B = (1 << WGM12) | (1 << CS10);
	OCR1A = 0;
}

static void init_interrupts(void) {
	MCUCR = (1 << ISC01);
	GICR = (1 << INT0);
}

static void update_pwm(const uint16_t value) {
	OCR1A = value;
}

static void handle_manual_mode(void) {
	update_pwm(adc_value);
}

static void handle_blink_mode(void) {
	static uint16_t last_time = 0;
	const uint16_t current_time = blink_timer;
	static uint8_t blink_state = 0;

	if ((current_time - last_time) > (ADC_MAX - adc_value) / 8) {
		last_time = current_time;
		blink_state = !blink_state;
		update_pwm(blink_state ? PWM_MAX : 0);
	}
}

static void handle_breathing_mode(void) {
	static uint16_t last_time = 0;
	const uint16_t current_time = fade_timer;

	if ((current_time - last_time) > FADE_DELAY) {
		last_time = current_time;
		
		breathing_value += breathing_direction * 4;
		
		if (breathing_value >= PWM_MAX) {
			breathing_value = PWM_MAX;
			breathing_direction = -1;
			} else if (breathing_value <= 0) {
			breathing_value = 0;
			breathing_direction = 1;
		}
		
		update_pwm(breathing_value);
	}
}