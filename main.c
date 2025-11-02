#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include <stdio.h>
#include "LPC17xx.h"                       
#include "GLCD.h"
#include "LED.h" 
#include <math.h>
#include "string.h"

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0										/* Uncomment to use the LCD */

//ITM Stimulus Port definitions for printf //////////////////
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

struct __FILE { int handle;  };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}
///////////////////////////////////////////////////////////////////////////////

void delay(int ctr){
	int t;
	for(t = 0; t < ctr; t++){
		t++; 
	}
}	


int Init_Thread (void); 

// Bit Band Macros used to calculate the alias address at run time
#define ADDRESS(x)    (*((volatile unsigned long *)(x)))
#define BitBand(x, y) 	ADDRESS(((unsigned long)(x) & 0xF0000000) | 0x02000000 |(((unsigned long)(x) & 0x000FFFFF) << 5) | ((y) << 2))
	
//Pointer declaration
volatile unsigned long * bit;
int a=0, b=2, c=0; // Barrel Shifting variables

int x; 

int  mem=0,dev=0, app=0, cpu=0, usr=0;
char var_logger[15];

/* 	THREAD DECLARATION */ 
/////////////////////////////////////////////////////
osThreadId tid_MemoryThread; 
void MemoryThread (void const *argument); 
osThreadDef (MemoryThread, osPriorityHigh, 1, 0);                  

osThreadId tid_CPUThread; 
void CPUThread (void const *argument); 
osThreadDef (CPUThread, osPriorityHigh, 1, 0);    

osThreadId tid_AppThread; 
void AppThread (void const *argument);
osThreadDef (AppThread, osPriorityAboveNormal, 1, 0); 

osThreadId tid_DeviceThread; 
void DeviceThread (void const *argument); 
osThreadDef (DeviceThread, osPriorityAboveNormal, 1, 0);

osThreadId tid_UIThread; 
void UIThread (void const *argument);
osThreadDef (UIThread, osPriorityNormal, 1, 0); 


/* MAIN INITIALIZE AND START THE SYSTEM */
/////////////////////////////////////////////////////////
int main (void) {
LED_Init(); 
#ifdef __USE_LCD
	GLCD_Init();                               /* Initialize graphical LCD (if enabled */
	GLCD_Clear(White);                         /* Clear graphical LCD display   */
	GLCD_SetTextColor(White);
	GLCD_SetBackColor(Red);
	GLCD_DisplayString(0, 0, __FI, "   Lab 3b Q2 Demo   ");
	GLCD_DisplayString(1, 0, __FI, "                    ");
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(3, 0, __FI, "Executing Thread:   "); 
	GLCD_DisplayString(6, 0, __FI, "Logger Variable:    "); 
	
#endif
  osKernelInitialize ();                    // initialize CMSIS-RTOS
 	Init_Thread ();
  osKernelStart ();                         // start thread execution 
	osDelay(osWaitForever);
}

/* THREAD INITIALIZATION */ 
////////////////////////////////////////////////////////
int Init_Thread (void) {
  tid_MemoryThread = osThreadCreate (osThread(MemoryThread), NULL);
	tid_CPUThread = osThreadCreate (osThread(CPUThread), NULL);
	tid_AppThread = osThreadCreate (osThread(AppThread), NULL); 
	tid_DeviceThread = osThreadCreate (osThread(DeviceThread), NULL);
	tid_UIThread = osThreadCreate (osThread(UIThread), NULL); 
  if(!tid_MemoryThread) return(-1);
  return(0);
} 


/* THREAD EXECUTION */
/////////////////////////////////////////////////////////
void MemoryThread (void const *argument) {
	LED_On(0); 
	mem++; 
	bit = &BitBand(&LPC_GPIO1->FIOPIN, 28); 
	GLCD_DisplayString(4, 0, __FI, "Mem Thread     "); 
	delay(100000000);
	LED_Off(0);
	osThreadYield();
	while(1){
		osSignalWait(0x01, osWaitForever);
			osDelay(1);
			osThreadTerminate(tid_MemoryThread); 	
	} 
}	 

void CPUThread (void const *argument) {
	LED_On(1); 
	cpu++;
		while(c <= 5){
		a = pow(b,c);
		c++;
	}
	c=0;
	GLCD_DisplayString(4, 0, __FI, "CPU Thread     ");
	delay(100000000);
	LED_Off(1); 
	osSignalSet(tid_MemoryThread, 0x01); 
	osThreadTerminate(tid_CPUThread); 
}

void AppThread (void const *argument) {
	LED_On(2); 
	strcpy(var_logger,"part 1");
	GLCD_DisplayString(4, 0, __FI, "App Thread     ");
	GLCD_DisplayString(7, 0, __FI, var_logger);
	delay(100000000);
	LED_Off(2);
	osThreadYield(); 
	while(1){
		osSignalWait(0x02, osWaitForever); 
			app++; 
			osDelay(1); 
			osThreadTerminate(tid_AppThread); 
	}	 
}

void DeviceThread (void const *argument) {
	LED_On(3);
	var_logger[7] = 'p';		
	var_logger[8] = 'a';
	var_logger[9] = 'r';
	var_logger[10] = 't';
	var_logger[11] = ' ';
	var_logger[12] = '2';	
	osSignalSet(tid_AppThread, 0x02); 
	dev++;
	GLCD_DisplayString(4, 0, __FI, "Device Thread   ");
	GLCD_DisplayString(7, 0, __FI, var_logger); 
	delay(100000000);
	LED_Off(3);  
	osThreadTerminate(tid_DeviceThread); 
}

void UIThread (void const *argument) {
	LED_On(4); 
	usr++; 
	GLCD_DisplayString(4, 0, __FI, "UI Thread      ");
	delay(100000000);
	LED_Off(4);
	osThreadTerminate(tid_UIThread);  
}
	

