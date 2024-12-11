#include "r_cg_eeprom.h"
#include "r_cg_macrodriver.h"
#include "r_cg_port.h"

// Author: Tran Chi Thanh
// Date: 2024/12/11
// EEPROM model: 93LC46B
// -\This library is using for read, write and earse 16 bit data on EEPROM \-

void wait_05us( unsigned short cnt )
{
	unsigned short intlp;
	
	for( intlp = 0; intlp < cnt; intlp-- ){
		NOP();	//NOPŽÀsŽžŠÔ:0.03125us(32MHz)  NOP16ŒÂ‚Å0.5us(500ns:2MHz)
		NOP();
		NOP();
		NOP();

		NOP();
		NOP();
		NOP();
		NOP();
		
		NOP();
		NOP();
		NOP();
		NOP();

		NOP();
		NOP();
		NOP();
		NOP();
	}
}

void nop_wait( unsigned short cnt )
{
	unsigned short intlp;
	
	for( intlp = 0; intlp < cnt; intlp++){
		wait_05us(2000);
	}
}


// Initialize EEPROM Pins
void EE_93LC46B_init(void) {
    // Configure pins as output or input
    EEPROM_CS = 0; 					// CS low
    EEPROM_CLK = 0; 					// CLK low
    EEPROM_DI = 0; 					// DI low
    EEPROM_DO = 0; 					//pull up set to 0
}

// Send a Byte of Data to the EEPROM
void write_value(unsigned char value) {
    unsigned char clk;	
    for ( clk = 8; clk > 0; clk--) {
        if (value & 0x80) {
            EEPROM_DI = 1; 				// Set DI High
        } else {
            EEPROM_DI = 0; 				// Set DI Low
        }
        EEPROM_CLK = 1; 				// Set Clock High
        nop_wait(1);
        EEPROM_CLK = 0; 				// Set Clock Low
        nop_wait(1);
        value <<= 1; 					// Shift value for next bit
    }
}

// Read a Byte of Data from the EEPROM
unsigned char read_value(void) {
    unsigned char clk;	
    unsigned char temp = 0x00;
    for (clk = 8; clk > 0; clk--) {
        temp <<= 1; 					// Make room for next bit
        EEPROM_CLK = 1; 				// Set Clock High
        nop_wait(1);
        if (EEPROM_DO) { 				// Read bit from DO
            temp |= 0x01;
        }
        EEPROM_CLK = 0; 				// Set Clock Low
        nop_wait(1);
    }
    return temp;
}

// Write a 16-bit Value to the EEPROM
void EE_93LC46B_write(unsigned char address, unsigned long value) {
    unsigned char hb = (value >> 8) & 0xFF;
    unsigned char lb = value & 0xFF;
    unsigned long timeout = 0;
    
    EE_93LC46B_erase_and_write_enable();
    EEPROM_CS = 1; 					// Activate Chip Select
    write_value(start);
    write_value(opcode_write | address);
    write_value(hb);
    write_value(lb);
    EEPROM_CS = 0; 					// Deactivate Chip Select
    nop_wait(20); 					// Wait for Write Cycle to Complete
    EEPROM_CS = 1;
    
    while ((!EEPROM_DO) && (timeout++ < 10000)) {
        nop_wait(1);
    }
    EEPROM_CS = 0;
    EE_93LC46B_erase_and_write_disable();
}

// Read a 16-bit Value from the EEPROM
unsigned long EE_93LC46B_read(unsigned char address) {
    unsigned char hb, lb;
    EEPROM_CS = 1; 					// Activate Chip Select
    write_value(start);
    write_value(opcode_read | address);
    hb = read_value();
    lb = read_value();
    EEPROM_CS = 0; 					// Deactivate Chip Select
    return ((hb << 8) | lb);
}

// Enable Write and Erase Operations
void EE_93LC46B_erase_and_write_enable(void) {
    EEPROM_CS = 1;
    write_value(start);
    write_value(opcode_write_and_erase_enable);
    EEPROM_CS = 0;
    nop_wait(10);
}

// Disable Write and Erase Operations
void EE_93LC46B_erase_and_write_disable(void) {
    EEPROM_CS = 1;
    write_value(start);
    write_value(opcode_write_and_erase_disable);
    EEPROM_CS = 0;
    nop_wait(10);
}

// Erase a Specific Address
void EE_93LC46B_erase(unsigned char address) {
    EEPROM_CS = 1;
    write_value(start);
    write_value(opcode_erase | address);
    EEPROM_CS = 0;
    nop_wait(10);
}

// Erase All Data in the EEPROM
void EE_93LC46B_erase_all(void) {
    EEPROM_CS = 1;
    write_value(start);
    write_value(opcode_erase_all);
    EEPROM_CS = 0;
    nop_wait(10);
}

