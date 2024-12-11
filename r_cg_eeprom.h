#ifndef R_CG_EEPROM_H
#define R_CG_EEPROM_H

// Pin Definitions
#define EEPROM_CS  P14_bit.no2   // Chip Select (CS) - Output
#define EEPROM_CLK P14_bit.no3   // Clock (CLK) - Output
#define EEPROM_DI  P14_bit.no4   // Data Input (DI) - Output
#define EEPROM_DO  P3_bit.no0    // Data Output (DO) - Input
// EEPROM Opcodes
#define start                             0x01
#define opcode_erase_all                  0x20
#define opcode_write_all                  0x10
#define opcode_write_and_erase_enable     0x30
#define opcode_write_and_erase_disable    0x00        
#define opcode_write                      0x40
#define opcode_read                       0x80
#define opcode_erase                      0xC0
// Function Prototypes
void EE_93LC46B_init(void);
void write_value(unsigned char value);
unsigned char read_value(void);
void EE_93LC46B_write(unsigned char address, unsigned long value);
unsigned long EE_93LC46B_read(unsigned char address);
void EE_93LC46B_erase_and_write_enable(void);
void EE_93LC46B_erase_and_write_disable(void);
void EE_93LC46B_erase(unsigned char address);
void EE_93LC46B_erase_all(void);


#endif // R_CG_EEPROM_H