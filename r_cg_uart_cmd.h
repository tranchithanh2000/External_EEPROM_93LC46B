#ifndef R_CG_UART_CMD_H
#define R_CG_UART_CMD_H

#include <stdint.h>

//**************************************************//
//********************IU ADD************************\\
// Definitions
#define MAX_DATA_ENTRIES 35  // Maximum storage entries

// Function prototypes
void init_Uart(void);
void sendString(char comment);
void sendComment(const char *comment);
void sendCommentEnter(const char *comment);
void sendFormattedUART(const char* format, int length);
void sendFormattedUART_Data(const char* format, int length);

// BBC calculation functions
uint8_t calculate_bcc(const uint8_t *buffer, uint8_t length);
uint8_t calculateBCC_adj_addres(uint8_t stx, uint8_t question1, uint8_t question2, uint8_t cmd1, uint8_t cmd2, uint8_t etx);
uint8_t calculateBCC_GS(uint8_t stx, uint8_t question1, uint8_t question2, uint8_t cmd1, uint8_t cmd2, uint8_t data1, uint8_t data2, uint8_t etx);
uint8_t calculateBCC_B(uint8_t stx, uint8_t b, uint8_t address1, uint8_t address2, uint8_t cmd, uint8_t light1, uint8_t light2, uint8_t etx);
uint8_t calculateBCC_B_OFF(uint8_t stx, uint8_t b, uint8_t address1, uint8_t address2, uint8_t cmd1, uint8_t cmd2, uint8_t offData, uint8_t etx);
uint8_t calculateBCC_E(uint8_t stx, uint8_t address1, uint8_t address2, uint8_t cmd, uint8_t etx);
uint8_t calculateBCC_A(uint8_t stx, uint8_t address1, uint8_t address2, uint8_t cmd, uint8_t data[], uint8_t etx);

// UART Command Functions
void IU_Adj_Adres(uint8_t stx, uint8_t question[], uint8_t cmd[], uint8_t etx);
void IU_Adj_Adres_GS(uint8_t stx, uint8_t question[], uint8_t cmd[], uint8_t data[], uint8_t etx);
void IU_B_Cmd(uint8_t stx, uint8_t b, uint8_t address[], uint8_t cmd, uint8_t light[], uint8_t etx);
void IU_B_OFF_Cmd(uint8_t stx, uint8_t b, uint8_t address[], uint8_t cmd[], uint8_t offData, uint8_t etx);
void IU_E_Cmd(uint8_t stx, uint8_t address[], uint8_t cmd, uint8_t etx);
void IU_A_Cmd(uint8_t stx, uint8_t address[], uint8_t cmd, uint8_t data[], uint8_t etx);

// Address and Storage Management
void incrementAddress(uint8_t address[]);
void address_storage(uint8_t address[]);
//void address_storage_check(uint8_t address[]);
void clearDataStorage(void);

extern uint8_t stx;
extern uint8_t etx;

// Declarations for "??", "NR", "GE", etc.
extern uint8_t question[2];
extern uint8_t cmd_NR[2];
extern uint8_t cmd_GE[2];
extern uint8_t cmd_GS[2];
extern uint8_t cmd_GQ[2];
extern uint8_t address[2];

// B command-related declarations
extern uint8_t address_B[2];
extern uint8_t address_01[2];
extern uint8_t B;
extern uint8_t A;

// LED command declarations
extern uint8_t light_color[6][2];
extern uint8_t light_yellow[2];
extern uint8_t light_green[2];
extern uint8_t light_red[2];
extern uint8_t light_OFF[2];
extern uint8_t offData;

// E and A command declarations
extern uint8_t cmd_E;
extern uint8_t cmd_A;
extern int8_t ones;
extern int8_t tens;

// Data-related declarations
extern uint8_t data_A[6];

// Storage data declarations
#define MAX_DATA_ENTRIES 35
extern uint8_t data_storage[MAX_DATA_ENTRIES][2];
extern uint8_t data_storage_check[MAX_DATA_ENTRIES][2];
extern uint8_t data_index;
extern uint8_t data_index1;

//IU Add function
void iu_add();


//*******************************************************//
//********************IU Test****************************\\
//void checkDevicesAndSetStatus();
#define ADDRESS_RANGE_TENS_UPPER 0x36 //range 00-64 (Max is 6)
#define RX_BUFFER_SIZE 128
#define MAX_DEVICES 99

// External declarations
extern uint8_t address_A[2];
extern uint8_t address_E[2];
extern uint8_t terminateFlags[MAX_DEVICES];
extern uint8_t device_exists[MAX_DEVICES];
extern volatile int ledState;
extern volatile int ledState1;

extern volatile uint8_t currentMode;
extern volatile uint8_t terminateFlag01;
extern volatile uint8_t terminateFlag02;
extern volatile uint8_t terminateFlag03;

//Buffer Definition
#include "r_cg_uart_buffer.h"

// Function declarations
void checkDevicesAndSetStatus(void);
void IU_E_Cmd(uint8_t stx, uint8_t *address, uint8_t cmd, uint8_t etx);
void address_storage_check(uint8_t *address);
void E_CMD_01_99();
void executeE_01_64();
void executeE_65_99();
void executeA_01_64();
void executeA_65_99();
void B_CMD_01_64(uint8_t light_color[2]);
void B_CMD_65_99(uint8_t light_color[2]);

void iu_test();


//void delay(uint16_t ms);

#endif // R_CG_UART_CMD_H