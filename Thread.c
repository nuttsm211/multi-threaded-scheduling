#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include <math.h>
#include <stdio.h>

int testcount; 
int x, n, r;   
int A = 0, B = 1, C = 0, D = 0, E = 0; 
int bf=1, df=1; // Task factorial storage 
double PI = 3.14159265359; 

/* 	THREAD DECLARATION */ 
/////////////////////////////////////////////////////
osThreadId tid_ThreadA; 
void ThreadA (void const *argument); 
osThreadDef (ThreadA, osPriorityAboveNormal, 1, 0);                  

osThreadId tid_ThreadB; 
void ThreadB (void const *argument); 
osThreadDef (ThreadB, osPriorityNormal, 1, 0);    

osThreadId tid_ThreadC; 
void ThreadC (void const *argument);
osThreadDef (ThreadC, osPriorityHigh, 1, 0); 

osThreadId tid_ThreadD; 
void ThreadD (void const *argument); 
osThreadDef (ThreadD, osPriorityAboveNormal, 1, 0);

osThreadId tid_ThreadE; 
void ThreadE (void const *argument);
osThreadDef (ThreadE, osPriorityNormal, 1, 0); 

/* THREAD INITIALIZATION */ 
////////////////////////////////////////////////////////
int Init_Thread (void) {
  tid_ThreadA = osThreadCreate (osThread(ThreadA), NULL);
	tid_ThreadB = osThreadCreate (osThread(ThreadB), NULL);
	tid_ThreadC = osThreadCreate (osThread(ThreadC), NULL); 
	tid_ThreadD = osThreadCreate (osThread(ThreadD), NULL);
	tid_ThreadE = osThreadCreate (osThread(ThreadE), NULL); 
  if(!tid_ThreadA) return(-1);
  return(0);
}

void ThreadA (void const *argument) {
	while(x < 256){
		A = (A + (x + (x + 2))); 
		x++; 
	}	
}	

void ThreadB (void const *argument) {
	while(n < 16) { 
		bf*=n; 
		B = (B + (pow(2,n)/bf)); 
		n++; 
	}	
}

void ThreadC (void const *argument) {
	n = 0; 
	while(n < 16) {
		C += ((n+1) / n); 
		n++; 
	}	
}

void ThreadD (void const *argument) {
	n = 0; 
	while(n < 6){
		if (n == 0) {
			D = 1;
		}	else {
			df*=n;
			D += (pow(5,n)/df);
		}
		n++; 
	}	
}

void ThreadE (void const *argument) {
	r = 2; 
	for(x = 1; x <= 12; x++){
		E = E + (x*PI*pow(r,2)); 
	}		
}
