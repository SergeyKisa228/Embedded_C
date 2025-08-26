/* INCLUDE */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

/* DEFINE */
// I2C communication parameters
#define I2C_BIT_RATE 100000UL
#define I2C_PRESCALER 1
#define I2C_TWBR_VALUE ((F_CPU / I2C_BIT_RATE) - 16) / (2 * I2C_PRESCALER)

// LM75 temperature sensor configuration
#define LM75_ADDRESS 0x48
#define LM75_TEMP_REGISTER 0x00

// PCF8574 I2C LCD backpack configuration
#define PCF8574_ADDRESS 0x27
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE 0x04
#define LCD_READ_WRITE 0x02
#define LCD_REGISTER_SELECT 0x01

// LCD command definitions
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_RETURN_HOME 0x02
#define LCD_ENTRY_MODE_SET 0x04
#define LCD_DISPLAY_CONTROL 0x08
#define LCD_FUNCTION_SET 0x20
#define LCD_SET_DDRAM_ADDR 0x80

// LCD display mode configuration
#define LCD_2_LINE 0x08
#define LCD_5x8_DOTS 0x00

/* ENUM */
typedef enum {
	I2C_OPERATION_SUCCESS,
	I2C_OPERATION_ERROR
} i2c_status_t;

/* FUNCTION PROTOTYPES */
static void init_i2c(void); // Initialize I2C/TWI interface
static i2c_status_t i2c_start(uint8_t address); // Send I2C start condition and address
static i2c_status_t i2c_write(uint8_t data); //  Write data to I2C bus
static void i2c_stop(void); // Send I2C stop condition
static void init_lcd(void); // Initialize LCD display
static void lcd_send_command(uint8_t command); // Send command to LCD
static void lcd_send_data(uint8_t data); // Send data to LCD
static void lcd_send_string(const char* string); // Send string to LCD
static void lcd_set_cursor(uint8_t row, uint8_t column); // Set LCD cursor position
static int16_t read_temperature(void); // Read temperature from LM75 sensor
static void display_temperature(int16_t temperature); // Display temperature on LCD

/* GLOBAL VARIABLES */
static volatile uint8_t i2c_error_state = 0;

/* MAIN FUNCTION */
int main(void) {
	int16_t temperature = 0;
	
	init_i2c();
	init_lcd();
	sei();
	
	lcd_send_string("Temperature:");
	
	while (1) {
		temperature = read_temperature();
		display_temperature(temperature);
		_delay_ms(500);
	}
}

/* FUNCTION DEFINITIONS */
static void init_i2c(void) {
	TWBR = (uint8_t)I2C_TWBR_VALUE;
	TWSR = 0x00;
	TWCR = (1 << TWEN);
}

static i2c_status_t i2c_start(uint8_t address) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT))) {}
	
	if ((TWSR & 0xF8) != 0x08) {
		return I2C_OPERATION_ERROR;
	}
	
	TWDR = address;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT))) {}
	
	if ((TWSR & 0xF8) != 0x18) {
		return I2C_OPERATION_ERROR;
	}
	
	return I2C_OPERATION_SUCCESS;
}

static i2c_status_t i2c_write(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT))) {}
	
	if ((TWSR & 0xF8) != 0x28) {
		return I2C_OPERATION_ERROR;
	}
	
	return I2C_OPERATION_SUCCESS;
}

static void i2c_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	while (TWCR & (1 << TWSTO)) {}
}

static void init_lcd(void) {
	_delay_ms(50);
	
	lcd_send_command(LCD_FUNCTION_SET | LCD_2_LINE | LCD_5x8_DOTS);
	_delay_ms(5);
	
	lcd_send_command(LCD_DISPLAY_CONTROL | 0x04);
	_delay_ms(5);
	
	lcd_send_command(LCD_CLEAR_DISPLAY);
	_delay_ms(5);
	
	lcd_send_command(LCD_ENTRY_MODE_SET | 0x02);
	_delay_ms(5);
}

static void lcd_send_command(uint8_t command) {
	uint8_t high_nibble = command & 0xF0;
	uint8_t low_nibble = (command << 4) & 0xF0;
	
	if (i2c_start(PCF8574_ADDRESS << 1) == I2C_OPERATION_SUCCESS) {
		i2c_write(high_nibble | LCD_BACKLIGHT | LCD_ENABLE);
		i2c_write(high_nibble | LCD_BACKLIGHT);
		i2c_stop();
	}
	
	if (i2c_start(PCF8574_ADDRESS << 1) == I2C_OPERATION_SUCCESS) {
		i2c_write(low_nibble | LCD_BACKLIGHT | LCD_ENABLE);
		i2c_write(low_nibble | LCD_BACKLIGHT);
		i2c_stop();
	}
}

static void lcd_send_data(uint8_t data) {
	uint8_t high_nibble = data & 0xF0;
	uint8_t low_nibble = (data << 4) & 0xF0;
	
	if (i2c_start(PCF8574_ADDRESS << 1) == I2C_OPERATION_SUCCESS) {
		i2c_write(high_nibble | LCD_BACKLIGHT | LCD_REGISTER_SELECT | LCD_ENABLE);
		i2c_write(high_nibble | LCD_BACKLIGHT | LCD_REGISTER_SELECT);
		i2c_stop();
	}
	
	if (i2c_start(PCF8574_ADDRESS << 1) == I2C_OPERATION_SUCCESS) {
		i2c_write(low_nibble | LCD_BACKLIGHT | LCD_REGISTER_SELECT | LCD_ENABLE);
		i2c_write(low_nibble | LCD_BACKLIGHT | LCD_REGISTER_SELECT);
		i2c_stop();
	}
}

static void lcd_send_string(const char* string) {
	while (*string) {
		lcd_send_data(*string++);
	}
}

static void lcd_set_cursor(uint8_t row, uint8_t column) {
	uint8_t address = (row == 0) ? 0x00 : 0x40;
	address += column;
	lcd_send_command(LCD_SET_DDRAM_ADDR | address);
}

static int16_t read_temperature(void) {
	int16_t temperature = 0;
	
	if (i2c_start((LM75_ADDRESS << 1) | 0x00) != I2C_OPERATION_SUCCESS) {
		i2c_stop();
		return 0x7FFF; // Error value
	}
	
	if (i2c_write(LM75_TEMP_REGISTER) != I2C_OPERATION_SUCCESS) {
		i2c_stop();
		return 0x7FFF; // Error value
	}
	
	if (i2c_start((LM75_ADDRESS << 1) | 0x01) != I2C_OPERATION_SUCCESS) {
		i2c_stop();
		return 0x7FFF; // Error value
	}
	
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT))) {}
	temperature = TWDR << 8;
	
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT))) {}
	temperature |= TWDR;
	
	i2c_stop();
	
	return temperature / 32;
}

static void display_temperature(int16_t temperature) {
	uint8_t is_negative = 0;
	uint8_t integer_part;
	uint8_t fractional_part;
	
	if (temperature == 0x7FFF) {
		lcd_set_cursor(1, 0);
		lcd_send_string("Error        ");
		return;
	}
	
	if (temperature < 0) {
		is_negative = 1;
		temperature = -temperature;
	}
	
	integer_part = (uint8_t)(temperature / 2);
	fractional_part = (uint8_t)((temperature % 2) * 5);
	
	lcd_set_cursor(1, 0);
	
	if (is_negative) {
		lcd_send_data('-');
		} else {
		lcd_send_data('+');
	}
	
	if (integer_part < 10) {
		lcd_send_data('0');
		lcd_send_data('0' + integer_part);
		} else if (integer_part < 100) {
		lcd_send_data('0' + integer_part / 10);
		lcd_send_data('0' + integer_part % 10);
		} else {
		lcd_send_string("XX");
	}
	
	lcd_send_data('.');
	lcd_send_data('0' + fractional_part);
	lcd_send_string(" C");
}