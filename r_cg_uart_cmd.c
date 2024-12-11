#include "r_cg_uart_cmd.h"
#include "r_cg_macrodriver.h"
#include "r_cg_lcd.h"

#include <stdlib.h>
#include <string.h>

// Init Uart
void init_Uart(){
    // Step 1: Initialize UART0
    R_SAU0_Create();
    
    // Step 2: Start UART0 operation
    R_UART0_Start();
};
/*
// Send Character
void sendString(char comment) {
    // Send the character through UART
    R_UART0_Send((uint8_t *)&comment, 1);  // Send one byte
    
    P1 |= (1 << 0);  // Set P10 for transmit (ADM4853)
    delay(100000);   // Delay to ensure transmission is complete
}

// Send String
void sendComment(const char *comment) {
    int length = 0;
    
    // Calculate the length of the string (excluding null terminator)
    while (comment[length] != '\0') {
        length++;
    }
    
    // Send the string through UART
    R_UART0_Send((uint8_t *)comment, length);  
    
    P1 |= (1 << 0);  // Set P10 for transmit (ADM4853)
    delay(100000);    // Delay to ensure transmission is complete
}

// Send String With Comment
void sendCommentEnter(const char *comment) {
    int length = 0;
    char *buffer;
    uint8_t i; 
    
    // Calculate the length of the string
    while (comment[length] != '\0') {
        length++;
    }

    // Dynamically allocate memory for the comment plus the newline and null terminator
    buffer = (char *)malloc(length + 2);  // One extra for '\n' and one for '\0'
    
    if (buffer == NULL) {
        // Handle memory allocation failure
        return;
    }

    // Copy the comment into the buffer
    for (i = 0; i < length; i++) {
        buffer[i] = comment[i];
    }

    // Add newline and null terminator at the end
    buffer[length] = '\n';  // Add the newline character
    buffer[length + 1] = '\0';  // Null terminator

    // Send the string with newline through UART
    R_UART0_Send((uint8_t *)buffer, length + 1);  // +1 for the newline character

    P1 |= (1 << 0);  // Set P10 for transmit (ADM4853)
    delay(100000);    // Delay to ensure transmission is complete

    // Free the allocated memory
    free(buffer);
}
*/
uint8_t calculate_bcc(const uint8_t *buffer, uint8_t length) {
    uint8_t bcc = 0;
    uint8_t i; 
    // Ensure that we only calculate for the first 'length' bytes
    for (i = 0; i < length; i++) {
        bcc ^= buffer[i]; // Perform XOR operation
    }

    return bcc; // Return the calculated BCC
}
/*
// Function to send formatted data through UART
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
*/
// BBC calculation (NR and GE)
uint8_t calculateBCC_adj_addres(uint8_t stx, uint8_t question1, uint8_t question2, uint8_t cmd1, uint8_t cmd2, uint8_t etx) {
    return stx ^ question1 ^ question2 ^ cmd1 ^ cmd2 ^ etx;
}

// BBC calculation (GS)
uint8_t calculateBCC_GS(uint8_t stx, uint8_t question1, uint8_t question2, uint8_t cmd1, uint8_t cmd2, uint8_t data1, uint8_t data2, uint8_t etx) {
    return stx ^ question1 ^ question2 ^ cmd1 ^ cmd2 ^ data1 ^ data2 ^ etx;
}

// BBC calculation (B ON)
uint8_t calculateBCC_B(uint8_t stx, uint8_t b, uint8_t address1, uint8_t address2, uint8_t cmd, uint8_t light1, uint8_t light2, uint8_t etx) {
    return stx ^ b ^ address1 ^ address2 ^ cmd ^ light1 ^ light2 ^ etx;
}

// BBC calculation (B OFF)
uint8_t calculateBCC_B_OFF(uint8_t stx, uint8_t b, uint8_t address1, uint8_t address2, uint8_t cmd1, uint8_t cmd2, uint8_t offData, uint8_t etx) {
    return stx ^ b ^ address1 ^ address2 ^ cmd1 ^ cmd2 ^ offData ^ etx;
}

// BBC calculation (E)
uint8_t calculateBCC_E(uint8_t stx, uint8_t address1, uint8_t address2, uint8_t cmd, uint8_t etx) {
    return stx ^ address1 ^ address2 ^ cmd ^ etx;
}

// BBC calculation (A)
uint8_t calculateBCC_A(uint8_t stx, uint8_t address1, uint8_t address2, uint8_t cmd, uint8_t data[], uint8_t etx) {
    uint8_t bcc = stx ^ address1 ^ address2 ^ cmd;
    uint8_t i; 
    
    for (i = 0; i < 6; i++) {  // XOR each byte in the data array
        bcc ^= data[i];
    }
    bcc ^= etx;
    return bcc;
}

// Send NR and GE to UART
void IU_Adj_Adres(uint8_t stx, uint8_t question[], uint8_t cmd[], uint8_t etx) {
    // Calculate BCC based on the given bytes
    uint8_t bcc = calculateBCC_adj_addres(stx, question[0], question[1], cmd[0], cmd[1], etx);
    
    // Prepare the message to send
    char message[7] = {stx, question[0], question[1], cmd[0], cmd[1], etx, bcc};
    
    // Send the formatted message through UART
    sendFormattedUART(message, 7);
//    P1 &= ~(1 << 0);  // Set P10 for receive (ADM4853)

    
}

// Send GS to UART
void IU_Adj_Adres_GS(uint8_t stx, uint8_t question[], uint8_t cmd[], uint8_t data[], uint8_t etx) {
    // Calculate BCC based on the given bytes
    uint8_t bcc1 = calculateBCC_GS(stx, question[0], question[1], cmd[0], cmd[1], data[0], data[1], etx);
    
    // Prepare the message to send
    char message[9] = {stx, question[0], question[1], cmd[0], cmd[1], data[0], data[1], etx, bcc1};
    
    // Send the formatted message through UART
    sendFormattedUART(message, 9);
    delay(100000);

}

// Send B ON to UART
void IU_B_Cmd(uint8_t stx, uint8_t b, uint8_t address[], uint8_t cmd, uint8_t light[], uint8_t etx) {
    // Calculate BCC based on the given bytes
    uint8_t bcc = calculateBCC_B(stx, b, address[0], address[1], cmd, light[0], light[1], etx);
    
    // Prepare the message to send
    char message[9] = {stx, b, address[0], address[1], cmd, light[0], light[1], etx, bcc};

    // Send the formatted message through UART
    sendFormattedUART(message, 9);
    //delay(3000);
    delay(10000);
}

// Send B OFF to UART
void IU_B_OFF_Cmd(uint8_t stx, uint8_t b, uint8_t address[], uint8_t cmd[], uint8_t offData, uint8_t etx) {
    // Calculate BCC based on the given bytes
    uint8_t bcc = calculateBCC_B_OFF(stx, b, address[0], address[1], cmd[0], cmd[1], offData, etx);
    
    // Prepare the message to send
    char message[9] = {stx, b, address[0], address[1], cmd[0], cmd[1], offData, etx, bcc};

    // Send the formatted message through UART
    sendFormattedUART(message, 9);
    delay(10000);  // Adjust delay as needed
}

// Send E to UART
void IU_E_Cmd(uint8_t stx, uint8_t address[], uint8_t cmd, uint8_t etx) {
    // Calculate BCC for the E command
    uint8_t bcc = calculateBCC_E(stx, address[0], address[1], cmd, etx);
    
    // Prepare the message to send
    char message[6] = {stx, address[0], address[1], cmd, etx, bcc};
    
    // Send the formatted message through UART
    sendFormattedUART(message, 6);
}

// Send A to UART
void IU_A_Cmd(uint8_t stx, uint8_t address[], uint8_t cmd, uint8_t data[], uint8_t etx) {
    // Calculate BCC for the A command
    uint8_t bcc = calculateBCC_A(stx, address[0], address[1], cmd, data, etx);
    
    // Prepare the message to send
    char message[12] = {stx, address[0], address[1], cmd,
                        data[0], data[1], data[2], data[3], data[4], data[5],
                        etx, bcc};
    
    // Send the formatted message through UART
    sendFormattedUART(message, 12);
    delay(3000);
}


// Function to increment the address in ASCII hex format 
void incrementAddress(uint8_t address[]) {
    // Convert ASCII hex to a numeric value
    uint8_t highNibble = address[0] - 0x30;
    uint8_t lowNibble = address[1] - 0x30;
    uint8_t value = highNibble * 10 + lowNibble;

    // Increment the value, wrap around to 0 after reaching 99
    value = (value + 1) % 100;

    // Convert back to ASCII hex representation
    address[0] = (value / 10) + 0x30;  // Tens place
    address[1] = (value % 10) + 0x30;  // Ones place
}


// Storage Array After Scan
void address_storage(uint8_t address[]) {
    // Store the converted value in hexadecimal format in data_storage
    if (data_index < MAX_DATA_ENTRIES) {
        data_storage[data_index][0] = address[0];                // Optional leading byte for padding
        data_storage[data_index][1] = address[1];               // Tens place in hexadecimal
//        data_storage[data_index][2] = (value % 10);           // Ones place in hexadecimal
        data_index++;                                           // Move to the next entry for the next call
    }
}

// Storage Array Verify Again
void address_storage_check(uint8_t address[]) {  
    
    if (data_index < MAX_DATA_ENTRIES) {
        data_storage_check[data_index1][0] = address[0];        // Optional leading byte for padding
        data_storage_check[data_index1][1] = address[1];        // Tens place in hexadecimal
//        data_storage[data_index][2] = (value % 10);           // Ones place in hexadecimal
        data_index1++;                                          // Move to the next entry for the next call
    }
}

//IU ADD Routine
void iu_add(){
		uint8_t calculated_BCC = 0;
		uint8_t address[2] = {0x30, 0x31};      // Address "01"
		uint8_t address1[2] = {0x30, 0x31};      // Address "01" 
//		terminateFlag01 = 0;
//		terminateFlag02 = 0;
//		terminateFlag03 = 0;
		
		//Reset device data:
		RX_Buffer[0] = '\0';   // Clear the buffer
		data_index = 0;  // Reset index to start from the first entry
   		memset(data_storage, 0, sizeof(data_storage));
		
		//off other programs
		ledState = 0;
		
		//=========Light ON==========
		IU_B_Cmd(stx, B, address, A, light_green, etx);
		
		// LCD
		Clr_LCD4();  // Clear the screen
		MoveHome_LCD4();  // Move cursor to home position   
		Puts_LCD4("Adjust Address");  // Display Mode 1 with cursor
		Set_DDA(0x40);  // Move cursor to second line
		Puts_LCD4("Current: ");  // Display Mode 2 without cursor*/

		//Device Number on LCD
		Set_DDA(0x40);
		Cursor_RN(9);	
		Putc_LCD4(address[0]);
		Set_DDA(0x40);
		Cursor_RN(10);	
		Putc_LCD4(address[1]);
		  

		//========ON LED=====
		led_ON();
		
		//Change Address
		IU_Adj_Adres(stx, question, cmd_NR, etx);
		
		//Send GE
		IU_Adj_Adres(stx, question, cmd_GE, etx);
		
		//Wait for the device get ready
		delay(1000000); 
		
		//Send GS init 01
		IU_Adj_Adres_GS(stx, question, cmd_GS, address, etx);

	while(1){
	 	delay(200000);
		
//		Send QG
		IU_Adj_Adres(stx, address, cmd_GQ, etx);
		P1 &= ~(1 << 0);  // Set P10 for receive (ADM4853)
	        delay(6000);
		
//		Calculate BCC From QG		
		calculated_BCC = calculate_bcc(RX_Buffer, 5);
		
		//Incresea Address
		if(RX_Data == calculated_BCC && RX_Buffer[2] == address[1]){
			
		  // Increase Address
		  incrementAddress(address);
		  
		  // LCD
		Clr_LCD4();  // Clear the screen
		MoveHome_LCD4();  // Move cursor to home position   
		Puts_LCD4("Adjust Address");  // Display Mode 1 with cursor
		Set_DDA(0x40);  // Move cursor to second line
		Puts_LCD4("Current: ");  // Display Mode 2 without cursor*/

		//Device Number on LCD
		Set_DDA(0x40);
		Cursor_RN(9);	
		Putc_LCD4(address[0]);
		Set_DDA(0x40);
		Cursor_RN(10);	
		Putc_LCD4(address[1]);
		  
		//Update Address +1  
		IU_Adj_Adres_GS(stx, question, cmd_GS, address, etx);
  
	       }
	       else{
			//Do nothing
	        }
		
		//Reset RX buffer and data
	        RX_Index = 0;          // Reset buffer index
	        RX_Buffer[0] = '\0';   // Clear the buffer
		memset(RX_Buffer, 0, sizeof(RX_Buffer));
		calculated_BCC = 0;
		RX_Data = 0;
		//RX_Buffer[0] = '\0';   // Clear the buffer
		if(!ledState1 || ledState) break;
	};
		
		//Send GE and LED Turn OFF 
		IU_Adj_Adres(stx, question, cmd_GE, etx);
		led_OFF();
	        IU_B_OFF_Cmd(stx, B, address1, light_OFF, offData, etx);
	 	//delay(3000000);

		//Return Home Screen
		Clr_LCD4(); // clear screen  
//		displayModeSelection(currentMode);
}

//*******************************************************//
//********************IU Test****************************\\ 


// Function to check devices and set their status
void checkDevicesAndSetStatus() {
    uint8_t i;
    uint8_t calculated_BCC;
   
     for (i = 0; i < 16; i++) {
    	// Reset buffer and variables after processing
        RX_Index = 0;
	calculated_BCC = 0;
        memset(RX_Buffer, 0, sizeof(RX_Buffer));   
        
	// Skip devices that are already marked as terminated
        if (terminateFlags[i] == 1) continue;

        // Retrieve address for the device
        address_E[0] = data_storage[i][0];
        address_E[1] = data_storage[i][1];

        // Send E command and compare received BCC with calculated BC
        IU_E_Cmd(stx, address_E, cmd_E, etx);  // Send E command
        P1 &= ~(1 << 0);  // Set P10 for receive (ADM4853)
        delay(5000);
	
        // Calculate BCC for the device address and compare with RX_Data
        calculated_BCC = calculate_bcc(RX_Buffer, 6);
        if (calculated_BCC == RX_Data) {
            device_exists[i] = 1;  // Device found, mark as found (1)	  
	    address_storage_check(address_E);
        } else {
            // If BCC does not match, mark the device as terminated
            terminateFlags[i] = 1;  // Set termination flag
            device_exists[i] = 0;   // Mark as not found (0)
	    
	    address_storage_check(address_E);	    
        }
        
    }
}

void E_CMD_01_99(){
   uint8_t tens;
   uint8_t ones;
   uint8_t calculated_BCC;	
   uint8_t address_E[] = {0x30,0x31};	
   for (tens = 0x30; tens <= 0x39; tens++) {
        for (ones = 0x30; ones <= 0x39; ones++) {
            address_E[0] = tens;
            address_E[1] = ones;
  	   // LCD
           Clr_LCD4();  // Clear the screen
	   MoveHome_LCD4();  // Move cursor to home position   
	   Puts_LCD4("Send E 01-99");  // Display Mode 1 with cursor
	   Set_DDA(0x40);  // Move cursor to second line
	   Puts_LCD4("Device : ");  // Display Mode 2 without cursor
	   
	   //Device Number on LCD
	   Set_DDA(0x40);
	   Cursor_RN(9);	
	   Putc_LCD4(address_E[0]);
	   Set_DDA(0x40);
	   Cursor_RN(10);	
	   Putc_LCD4(address_E[1]);
	    
	    IU_E_Cmd(stx, address_E, cmd_E, etx);
            P1 &= ~(1 << 0);  // Set P10 for receive (ADM4853)
            delay(4000);
	    
	    delay(100); //adjust time when send E

	    calculated_BCC = calculate_bcc(RX_Buffer, 6);	
           
//	   Check if exist any device and save it	
	   if(RX_Data == calculated_BCC && RX_Buffer[2] == address_E[1]){
		   
	      address_storage(address_E);
	      
	   }	
	   
		// Reset buffer and variables after processing
	        RX_Index = 0;          // Reset buffer index
	        RX_Buffer[0] = '\0';   // Clear the buffer
		memset(RX_Buffer, 0, sizeof(RX_Buffer));
		calculated_BCC = 0;
		RX_Data = 0;
		
	   
//	    if(!ledState || ledState1) break; 
        }
//	if(!ledState || ledState1) break;
    }

}

void executeE_01_64(){
   //uint8_t address_A[] = {0x30,0x31};	
   uint8_t i = 0; 
   uint8_t tens, ones;
        for (i = 0; i < MAX_DATA_ENTRIES; i++) {
          tens = data_storage[i][0];  // Tens place
          ones = data_storage[i][1];  // Ones place
	  address_E[0] = tens;
	  address_E[1] = ones;
	
        // Check if the address is within the range 01 to 64
        if ((tens < ADDRESS_RANGE_TENS_UPPER) || (tens == ADDRESS_RANGE_TENS_UPPER && ones <= 0x34)) {
          if (device_exists[i] == 1 && terminateFlags[i] == 0 && address_E[0] > 0x00 && address_E[1] > 0x00) { 
	   delay(1150000); 
  	   break;
	  }
         }
      }
   
    for (i = 0; i < MAX_DATA_ENTRIES; i++) {
        tens = data_storage[i][0];  // Tens place
        ones = data_storage[i][1];  // Ones place
	address_E[0] = tens;
	address_E[1] = ones;
	
        // Check if the address is within the range 01 to 64
      if ((tens < ADDRESS_RANGE_TENS_UPPER) || (tens == ADDRESS_RANGE_TENS_UPPER && ones <= 0x34)) {
         if (device_exists[i] == 1 && terminateFlags[i] == 0 && address_E[0] > 0x00 && address_E[1] > 0x00) {
	   IU_E_Cmd(stx, address_E, cmd_E, etx);
           P1 &= ~(1 << 0);  // Set P10 for receive (ADM4853)
           delay(4000);
	    // LCD
           Clr_LCD4();  // Clear the screen
	   MoveHome_LCD4();  // Move cursor to home position   
//	   Puts_LCD4("Send E 01-64");  // Display Mode 1 with cursor
	   Set_DDA(0x40);  // Move cursor to second line
//	   Puts_LCD4("Device : ");  // Display Mode 2 without cursor
	   
	   //Device Number on LCD
	   Set_DDA(0x40);
	   Cursor_RN(9);	
//	   Putc_LCD4(address_E[0]);
	   Set_DDA(0x40);
	   Cursor_RN(10);	
//	   Putc_LCD4(address_E[1]);
	   }  

        }
	if( data_storage[i][0] == 0x00 &&  data_storage[i][0] == 0x00) break;
//	if(!ledState || ledState1) break;
    }
    
      for (i = 0; i < MAX_DATA_ENTRIES; i++) {
        tens = data_storage[i][0];  // Tens place
        ones = data_storage[i][1];  // Ones place
	address_E[0] = tens;
	address_E[1] = ones;
	
        // Check if the address is within the range 01 to 64
      if ((tens < ADDRESS_RANGE_TENS_UPPER) || (tens == ADDRESS_RANGE_TENS_UPPER && ones <= 0x34)) {
         if (device_exists[i] == 1 && terminateFlags[i] == 0 && address_E[0] > 0x00 && address_E[1] > 0x00) { 
//	  IU_B_OFF_Cmd(stx, b, address, light_OFF, offData, etx);    	 
//	  delay(1100000); 
	  break;
	 }
        }
      }

}

void executeE_65_99(){
//   uint8_t address_E[] = {0x36,0x35};	//65
   uint8_t i = 0;
   uint8_t tens, ones;
    for (i = 0; i < MAX_DATA_ENTRIES; i++) {
        tens = data_storage[i][0];  // Tens place
        ones = data_storage[i][1];  // Ones place
	address_E[0] = tens;
        address_E[1] = ones;
       if ((tens > 0x36 || (tens == 0x36 && ones >= 0x35)) && (tens <= 0x39 && ones <= 0x39)) {
 	  if (device_exists[i] == 1 && terminateFlags[i] == 0 && address_E[0] > 0x00 && address_E[1] > 0x00) { 
	  	delay(1200000); 
	 	break;
	  }
       }
    }
	
    for (i = 0; i < MAX_DATA_ENTRIES; i++) {
        tens = data_storage[i][0];  // Tens place
        ones = data_storage[i][1];  // Ones place
	address_E[0] = tens;
        address_E[1] = ones;
        // Check if the address is within the range 01 to 64
      if ((tens > 0x36 || (tens == 0x36 && ones >= 0x35)) && (tens <= 0x39 && ones <= 0x39)) {
	  if (device_exists[i] == 1 && terminateFlags[i] == 0 && address_E[0] > 0x00 && address_E[1] > 0x00) {  
   	   IU_E_Cmd(stx, address_E, cmd_E, etx);
           P1 &= ~(1 << 0);  // Set P10 for receive (ADM4853)
           delay(4000);
	  
	   // LCD
           Clr_LCD4();  // Clear the screen
	   MoveHome_LCD4();  // Move cursor to home position   
//	   Puts_LCD4("Send E 64-99");  // Display Mode 1 with cursor
	   Set_DDA(0x40);  // Move cursor to second line
//	   Puts_LCD4("Device : ");  // Display Mode 2 without cursor
	   
	   //Device Number on LCD
	   Set_DDA(0x40);
	   Cursor_RN(9);	
//	   Putc_LCD4(address_E[0]);
	   Set_DDA(0x40);
	   Cursor_RN(10);	
//	   Putc_LCD4(address_E[1]);
	 }   	    
	   if(!ledState || ledState1) break; 
        }
	if( data_storage[i][0] == 0x00 &&  data_storage[i][0] == 0x00) break;
//	if(!ledState || ledState1) break;
    }
    
    for (i = 0; i < MAX_DATA_ENTRIES; i++) {
        tens = data_storage[i][0];  // Tens place
        ones = data_storage[i][1];  // Ones place
	address_E[0] = tens;
        address_E[1] = ones;
       if ((tens > 0x36 || (tens == 0x36 && ones >= 0x35)) && (tens <= 0x39 && ones <= 0x39)) {
 	  if (device_exists[i] == 1 && terminateFlags[i] == 0 && address_E[0] > 0x00 && address_E[1] > 0x00) { 
//	  	IU_B_OFF_Cmd(stx, b, address, light_OFF, offData, etx);              
//	  	delay(800000); 
	 	break;
	  }
       }
    }
}

void executeA_01_64() {
    uint8_t i;
    delay(240000);
    for (i = 0; i < MAX_DATA_ENTRIES; i++) {
        uint8_t tens = data_storage[i][0];  // Tens place
        uint8_t ones = data_storage[i][1];  // Ones place
        address_A[0] = tens;
        address_A[1] = ones;
        data_A[4] = tens;
        data_A[5] = ones;

        // Check if the address is within the range 01 to 64
        if ((tens < ADDRESS_RANGE_TENS_UPPER) || (tens == ADDRESS_RANGE_TENS_UPPER && ones <= 0x34)) {
            // Execute A command only for active devices that exist
            if (device_exists[i] == 1 && terminateFlags[i] == 0 && address_A[0] > 0x00 && address_A[1] > 0x00) {
                IU_A_Cmd(stx, address_A, cmd_A, data_A, etx);
                P1 &= ~(1 << 0);  // Set P10 for receive (ADM4853)

	    //LCD
		Clr_LCD4();  // Clear the screen
		MoveHome_LCD4();  // Move cursor to home position   
		Puts_LCD4("IU Test 01-64");  // Display Mode 1 with cursor
		Set_DDA(0x40);  // Move cursor to second line
		Puts_LCD4("Device: ");  // Display Mode 2 without cursor

		//Device Number on LCD
		Set_DDA(0x40);
		Cursor_RN(9);	
		Putc_LCD4(address_A[0]);
		Set_DDA(0x40);
		Cursor_RN(10);	
		Putc_LCD4(address_A[1]);
            }

        }
    }
}

void executeA_65_99() {
    uint8_t i;
    delay(240000);
    for (i = 0; i < MAX_DATA_ENTRIES; i++) {
        uint8_t tens = data_storage[i][0];  // Tens place
        uint8_t ones = data_storage[i][1];  // Ones place
        address_A[0] = tens;
        address_A[1] = ones;
        data_A[4] = tens;
        data_A[5] = ones;
	
	// Check if the address is within the range 65-99
        if ((tens > 0x36 || (tens == 0x36 && ones >= 0x35)) && (tens <= 0x39 && ones <= 0x39)) {
            // Execute A command only for active devices that exist
            if (device_exists[i] == 1 && terminateFlags[i] == 0 && address_A[0] > 0x00 && address_A[1] > 0x00) {
                IU_A_Cmd(stx, address_A, cmd_A, data_A, etx);
                P1 &= ~(1 << 0);  // Set P10 for receive (ADM4853)

	    //LCD
		Clr_LCD4();  // Clear the screen
		MoveHome_LCD4();  // Move cursor to home position   
		Puts_LCD4("IU Test 64-99");  // Display Mode 1 with cursor
		Set_DDA(0x40);  // Move cursor to second line
		Puts_LCD4("Device: ");  // Display Mode 2 without cursor

		//Device Number on LCD
		Set_DDA(0x40);
		Cursor_RN(9);	
		Putc_LCD4(address_A[0]);
		Set_DDA(0x40);
		Cursor_RN(10);	
		Putc_LCD4(address_A[1]);

            }
        }
    }
}

void B_CMD_01_64(uint8_t light_color[2]) {
     uint8_t i = 0; 
   uint8_t tens, ones;

    for (i = 0; i < MAX_DATA_ENTRIES; i++) {
        tens = data_storage[i][0];  // Tens place
        ones = data_storage[i][1];  // Ones place

        // Check if the address is within the range 01 to 64
        if ((tens < ADDRESS_RANGE_TENS_UPPER) || (tens == ADDRESS_RANGE_TENS_UPPER && ones <= 0x34)) {
            address_B[0] = tens;
            address_B[1] = ones;
            
            if (address_B[0] > 0x00 || address_B[1] > 0x00) {  // Ensure address is valid
              if (device_exists[i] == 1 && terminateFlags[i] == 0){
	       // Send B command 
                IU_B_Cmd(stx, B, address_01, A, light_color, etx);
		//delay(240000);
		}
	     //  else{IU_B_Cmd(stx, b, address, cmd, light_OFF, etx);}
            }
        }
     	if( data_storage[i][0] == 0x00 &&  data_storage[i][0] == 0x00) break;
//	if(!ledState || ledState1) break;
    }  

}

void B_CMD_65_99(uint8_t light_color[2]) {
     uint8_t i = 0; 
   uint8_t tens, ones;

    for (i = 0; i < MAX_DATA_ENTRIES; i++) {
        tens = data_storage[i][0];  // Tens place
        ones = data_storage[i][1];  // Ones place

        // Check if the address is within the range 65-99
          if ((tens > 0x36 || (tens == 0x36 && ones >= 0x35)) && (tens <= 0x39 && ones <= 0x39)) {
            address_B[0] = tens;
            address_B[1] = ones;
            
            if (address_B[0] > 0x00 || address_B[1] > 0x00) {  // Ensure address is valid
		if (device_exists[i] == 1 && terminateFlags[i] == 0 ){
	      		 // Send B command with the provided light color
               		 IU_B_Cmd(stx, B, address_01, A, light_color, etx);
//			 delay(240000);
		}
	       // else{IU_B_Cmd(stx, b, address, cmd, light_OFF, etx);}
            }
        }

     	if( data_storage[i][0] == 0x00 &&  data_storage[i][0] == 0x00) break;
//	if(!ledState || ledState1) break;
    }  
}

//IU TEST Fuction
void iu_test(){
	    uint8_t i = 0;
	    ledState1 = 0;
	    //========LCD Monitoring=====
		Clr_LCD4(); // clear screen
		MoveHome_LCD4();
		Set_DDA(0x00);             
		Cursor_RN(2); 
		Puts_LCD4("IU Test Runing");

		//========ON LED=====
		led_ON();
		  
	      // Step 4,5: Sent E from 00-99 and check (01-64) & (65-99) 
	      E_CMD_01_99(stx, address_E, cmd_E, etx);	      
	      
        while(1){	     
	  for (i = 0; i < 6; i++) {	
	      checkDevicesAndSetStatus(); //check if exist device
	      if(data_storage_check[i][0] != 0x00 && data_storage_check[i][1] != 0x00){      	  
		  checkDevicesAndSetStatus(); //check if exist device		  
		  if(data_storage_check[i][0] != 0x00 && data_storage_check[i][1] != 0x00){
		      // Step 6: Send A from 01-64    
		      executeA_01_64();
		      
		      // Step 7: Send B 01-64
		      B_CMD_01_64(light_color[i]);
		      
		      // Step 9: Send E OFF device (01-64)
		      executeE_01_64();		     
		      i++;		      	 
		  }
		      
		   if(data_storage_check[i][0] != 0x00 && data_storage_check[i][1] != 0x00){
		      // Step 10: Send A from 65-99 If Exist from step 4,5
		      executeA_65_99();
		      
		      // Step 11: Send B 64-99
		      B_CMD_65_99(light_color[i]);
		      
		      // Step 12: Send B and E OFF device (65-99) 
		      executeE_65_99(); 
		      
		           
		    }
	          }
	        else if(data_storage_check[i][0] == 0x00 && data_storage_check[i][1] == 0x00){
	          // LCD no device
	          data_index1 = 0;  // Reset buffer index after sending
		  memset(data_storage_check, 0, sizeof(data_storage_check));
		  memset(device_exists, 0, sizeof(device_exists));
		  checkDevicesAndSetStatus(); //check if exist device
	          if(data_storage_check[0][0] == 0x00 && data_storage_check[0][1] == 0x00){
			Clr_LCD4();  // Clear the screen
			MoveHome_LCD4();  // Move cursor to home position   
			Puts_LCD4("IU Test");  // Display Mode 1 with cursor
			Set_DDA(0x40);  // Move cursor to second line
			Puts_LCD4("No device found ");  // Display Mode 2 without cursor
	      
		         // 3 Light
			for(i = 0; i<3 ; i++){
			    IU_B_Cmd(stx, B, address_01, A, light_color[i], etx);
			    delay(2200000);
			}
			i=0;
		    }
	      }
	       //Reset storage
	       data_index1 = 0;  // Reset buffer index after sending
//	       data_storage_check[0][0] = '\0';   // Clear the buffer
	       memset(data_storage_check, 0, sizeof(data_storage_check));
	       memset(device_exists, 0, sizeof(device_exists));
	       
//	       if(!ledState || ledState1) break;
	}
	      	    
//	      if(!ledState || ledState1) break;
       };	        
 
	      
	      //OFF Led and device
	      IU_B_OFF_Cmd(stx, B, address_01, light_OFF, offData, etx);
	      
	      // Reset data_storage from E
	      data_index = 0;  // Reset index to start from the first entry
	      memset(data_storage, 0, sizeof(data_storage));
	      
	      //Return Home Screen
	      ledState = 0;
	      Clr_LCD4(); // clear screen  
//	      displayModeSelection(currentMode);

}