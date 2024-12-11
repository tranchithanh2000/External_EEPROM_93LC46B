/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2023 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G13 V2.05.07.02 [17 Nov 2023]
* Device(s)    : R5F100MG
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 12/10/2024
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "fpga.h"
#include "eeprom.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
#define FPGA_PROG_B		P5_bit.no0
#define FPGA_INIT_B		P0_bit.no1
#define FPGA_DONE		P1_bit.no0	//(P1 & 0x01)
#define FPGA_DIN		P1_bit.no2
#define FPGA_CCLK	P1_bit.no0

#define PROG_B_H	P5 |= 0x01
#define PROG_B_L	P5 &= 0xfe

#define DONE_H		P1 |= 0x01
#define DONE_L		P1 &= 0xfe

#define DIN_H		P1 |= 0x04
#define DIN_L		P1 &= 0xfb

#define CCLK_H		P1 |= 0x01
#define CCLK_L		P1 &= 0xfe

#define SW		P14 & 0x03

#define mySizeof(var) ((char*)(&var + 1) - (char*)(&var))



#define RX_BUFFER_SIZE 7  // Size of the expected message: <STX>01a<ETX>
char rx_buffer[RX_BUFFER_SIZE];  // Buffer to hold the received message

/*
volatile uint8_t RX_Data = 0;
volatile uint8_t RX_Buffer[7] = {0};// 20
volatile uint8_t RX_Index = 0;
*/


void fpga_config(void);

/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
   
    
      
    while (1U)
    {
	    fpga_config();
    }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */



	    EI();	
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
extern volatile uint16_t  g_csi00_tx_count;            /* csi00 send data count */
extern unsigned int g_Sendend;

extern void wait_05us( unsigned short cnt );

#include <stdlib.h>                       // For malloc and free
#include <string.h>


void delay(volatile uint32_t count) {
    while (count--) {

    }
}



/*
void delay_ms(unsigned int ms) {
    volatile unsigned int i;
    while (ms--) {
        for (i = 0; i < 1000; i++);  
    }
}
*/ 
#include <stdlib.h>                 // For malloc and free
#include <string.h>

//temp define
volatile int ledState = 0;
volatile int ledState1 = 0 ;


//********************************************************
//*******************Change IU Address********************
//Author: TRAN CHI THANH
//Date: 2024/12/02
//Description: This library is use for Uart Send Comment
//********************************************************
// Function to send formatted data through UART
#include "r_cg_uart_cmd.h"
// Calculate BCC
uint8_t calculated_BCC;

// STX and ETX
uint8_t stx = 0x02;
uint8_t etx = 0x03;

// Change IU Address Define
uint8_t i;
uint8_t j;
uint8_t question[2] = {0x3F, 0x3F};       //  "??" (ASCII)
uint8_t cmd_NR[2] = {0x4E, 0x52};         //  "NR" (ASCII)
uint8_t cmd_GE[2] = {0x47, 0x45};         //  "GE" (ASCII)
uint8_t cmd_GS[2] = {0x47, 0x53};         //  "GS" (ASCII)
uint8_t cmd_GQ[2] = {0x47, 0x51};         //  "GQ" (ASCII)

// B cmd
uint8_t address_B[2] = {0x30, 0x31};      // Device B_01 
uint8_t address_01[2] = {0x30, 0x31};     //  "01" (ASCII)
uint8_t B = 0x42;                         //  "B"  (ASCII)
uint8_t A = 0x41;                         //  "A"  (ASCII)

// Light Color Array
uint8_t light_color[6][2] = 
{
      {0x33, 0x31},  // yellow
      {0x32, 0x31},  // green
      {0x31, 0x31},   // red
      {0x33, 0x31},   
      {0x32, 0x31},  
      {0x31, 0x31} 
};

//LED cmd
uint8_t light_yellow[2] = {0x33, 0x31};   //  "11" (ASCII)
uint8_t light_green[2] = {0x32, 0x31};    //  "21" (ASCII)
uint8_t light_red[2] = {0x31, 0x31};      //  "31" (ASCII)
uint8_t light_OFF[2] = {0x52, 0x53};      //  "RS" (ASCII)
uint8_t offData = 0x30;

//E and A command
uint8_t cmd_E = 0x45;                     // Command for E
uint8_t cmd_A = 0x41;                     // Command for A
int8_t ones;
int8_t tens;

// Data
uint8_t data_A[6] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30};  // "000000" for A command

//Store Data
#define MAX_DATA_ENTRIES 35                               // Max devices
uint8_t data_storage[MAX_DATA_ENTRIES][2];                //Storage array
uint8_t data_storage_check[MAX_DATA_ENTRIES][2];          //Storage array
uint8_t data_index = 0;                                   // number of stored entries
uint8_t data_index1 = 0;  

void sendFormattedUART(const char* format, int length) {
    R_UART0_Send(format, length);  // Send the string through UART
    P1 |= (1 << 0);  // Set P10 for transmit (ADM4853)
    delay(3000);    // Delay
}

// Function to send formatted data through UART
void sendFormattedUART_Data(const char* format, int length) {
    R_UART0_Send(format, length);  // Send the string through UART
    P1 |= (1 << 0);  // Set P10 for transmit (ADM4853)
    delay(6000);    // Delay
}



//****************************************************
//********************IU Testing**********************
//Author: TRAN CHI THANH
//Date: 2024/12/02
//Description: This library is use for IU TESTING
//****************************************************

// Flags for device termination
volatile uint8_t terminateFlag01 = 0;
volatile uint8_t terminateFlag02 = 0;
volatile uint8_t terminateFlag03 = 0;

//Address for E and A commands
#define ADDRESS_RANGE_TENS_UPPER 0x36 //range 00-64 (Max is 6)
#define MAX_DEVICES 99

uint8_t address_E[2] = {0x30, 0x31};  // Address "01" for E command
uint8_t address_A[2] = {0x30, 0x31};  // Address "02" for A command
uint8_t terminateFlags[MAX_DEVICES] = {0};  // Array to store termination status of each device (0 = active, 1 = terminated)
uint8_t device_exists[MAX_DEVICES];  // Array to track device existence status
uint8_t device_exist[MAX_DATA_ENTRIES][2]; //Check exist address


//**************************************************
//***************Button Handling********************
//**************************************************
#include "r_cg_button_handling.h"

#pragma interrupt r_tau0_channel0_interrupt(vect=INTTM00)
static void __near r_tau0_channel0_interrupt(void) {
    //processButtons();
}

//**************************************************
//******************LCD 16x2************************
//**************************************************
#include "r_cg_lcd.h"

//button test
void ledControl(){
    // Check if P14.1 (button) is pressed (adjusted for pull-up logic)
    if ((P12 & (1 << 0))) { 
/*    
        P6 |= (1 << 4);  // Set P6.4 high (LED on)
        P6 |= (1 << 5);  // Set P6.5 high (LED on)
        P6 |= (1 << 6);  // Set P6.6 high (LED on)
        P6 |= (1 << 7);  // Set P6.7 high (LED on)
*/        
//        delay(100000);  // Delay for some time    
     
        P6 &= ~(1 << 4);  // Set P6.4 low (LED off)
        P6 &= ~(1 << 5);  // Set P6.5 low (LED off)
        P6 &= ~(1 << 6);  // Set P6.6 low (LED off)
        P6 &= ~(1 << 7);  // Set P6.7 low (LED off)
        
        delay(100000);  // Delay for some time

       
    } else {
        // Button not pressed
	P6 |= (1 << 4);  // Set P6.4 high (LED on)
        P6 |= (1 << 5);  // Set P6.5 high (LED on)
        P6 |= (1 << 6);  // Set P6.6 high (LED on)
        P6 |= (1 << 7);  // Set P6.7 high (LED on)
    }
}

//**************************************************
//******************IU TEST ADD************************
//**************************************************
//#include "r_cg_uart_cmd.h"

void lcd_eeprom(unsigned long data){
    	Clr_LCD4();
	MoveHome_LCD4();                     
	Puts_LCD4(data);
	delay_ms(400000);
}

#include "r_cg_eeprom.h"
//uint8_t data = 0;
void fpga_config(void)
{   	
	unsigned short i;
	
	/*
	init_Uart();
	
	//Timer interupt init
	R_TAU0_Channel0_Start();
	
	
	initializeButtons(); // Initialize buttons
	*/
	// Data 1
	unsigned char address = 0x10; 			//Address
	unsigned long write_data = "ThanhOT";
	unsigned long read_data;

	// Data 2
	unsigned char address1 = 0x20; 			//Address
	unsigned long write_data1 = "Next address";
	unsigned long read_data1;
	unsigned long write_data2 = "Next address thanh";

	//===============Read and Write===========
	//Format colum: <address> <data>
	EE_93LC46B_init();
	
	// Write data
	EE_93LC46B_write(address, write_data);
	
	nop_wait(10);  					// Delay
	
	// Read data
	read_data = EE_93LC46B_read(address);
	
	lcd_eeprom(read_data); 				//LCD
	
	// Write data
	EE_93LC46B_write(address1, write_data1);
	// EE_93LC46B_write(address1, write_data2);
	
	nop_wait(10);  					// Delay
	
	// Read data
	read_data1 = EE_93LC46B_read(address1);
	
	lcd_eeprom(read_data1);				//LCD
	
	//==================Erase================
	//Enable read and write
	EE_93LC46B_erase_and_write_enable();
	EE_93LC46B_erase_all();				//Erase All Address
	
	//EE_93LC46B_erase(address);			//Erase Specific Address
	
	read_data = EE_93LC46B_read(address);         
	
	lcd_eeprom(read_data);  			//LCD
	
	
	//EE_93LC46B_erase(address1); 			//Erase Specific Address
	
	read_data1 = EE_93LC46B_read(address1);

	lcd_eeprom(read_data1);  			//LCD
	
	//Disable read and write
	EE_93LC46B_erase_and_write_disable();
	
while (1) {
	//processButtons();
	
	//ledControl();
	
	};	   
}

#define FPGA_DATA_CNT 256		//FPGA転送バイト数
#define FPGA_DATA_OFF 0x67		//データオフセット

unsigned short fpga_config2(void)
{
	
}


/* End user code. Do not edit comment generated here */
