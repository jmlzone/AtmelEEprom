/*----------------------------------------------------------------------
  Small Command Line Interpter (and basic command line editing)
  12/27/2011 James M. Lee N1DDK
  Unrestrited for ham radio use
  ----------------------------------------------------------------------*/
#include <stdio.h>
#include "cli.h"

char cliBuf[CLIBUFLEN]; // Small buffer for input who wanto to type long lines
char *cliPtr;
int cliPos;

char cliGetC() {
  while(!Serial.available()){
    delay(1);
  }
  return(Serial.read());
}
  
  

void cliGetLine(char *prompt){
  int cliPos = 0;
  int cliGettingLine=1;
  char c;
  cliPrint(prompt);
  while(cliGettingLine) {
    c = cliGetC();
    switch(c){
      case '\n':
      case '\r':
	cliGettingLine = 0; // done
	cliBuf[cliPos] = 0; // terminte line
	cliPrint(cliCRLF);
	break;
      case '\033': // escape
	cliGettingLine = 0; // done
        cliPos = 0; // null the line
	cliBuf[cliPos] = 0; // terminte line
	cliPrint(cliEscaped);
	break;
      case '\010':
      case '\177':
	if(cliPos >=1) {
	  cliPos--;
	  cliPrint(cliRubout);
	}
	break;
      default:
	if(cliPos < CLIBUFLEN){
	  cliPutc(c); // echo charater
	  cliBuf[cliPos] = c; // take character
	  cliPos++;
	}
    }
  }
  cliPtr = cliBuf;
}

void cliPrint(char *msg){
  while(*msg != 0) {
    if(*msg == '\n') {
      cliPutc('\r');
    }
    cliPutc(*msg);
    *msg++;
  }
}

int cliGetCmd(char *buf, cmd_t *table){
  int i;
  //kill leading spaces in buffer
  while( *buf == ' '  || *cliPtr == '\r' || *cliPtr == '\n') buf++;
  if(*cliPtr == '\0') {
    return(cliNULLCMD);
  } else {
    for(i=0; table[i].cmd != -1; i++){
      //    cliPrint(cmdTable[i].cmdName);
      //    printf("trying '%s' against '%s'\n",cmdTable[i].cmdName, buf);
      //if(!strcmp(table[i].cmdName,buf)) return(table[i].cmd);
      if(cliMatch(table[i].cmdName,buf)) return(table[i].cmd);
    }
    return(cliNOTFOUND);
  }
}
int cliMatch(char *cmd, char *buf) {
  while(*buf != ' ' && *buf != '\0' && *cmd != '\0') {
    if(*buf == *cmd) {
      buf++; 
      cmd++;
    } else {
      return(0);
    }
  }
  //  if(*cmd == '\0') return(0);
  cliPtr = buf; // update global parse point 
  return(1);
}

char *cliGetOption(char *prompt) {
  // if there is stuff on the line return that 
  // otherwise prompt for a new line and return new stuff
  while(1) {
    if(*cliPtr == ' ' || *cliPtr == '\r' || *cliPtr == '\n' ) {
      cliPtr++;
    } else {
      if(*cliPtr == '\0'){
	cliGetLine(prompt);
      } else {
	return(cliPtr);
      }
    }
  }
}


/* gets an ascii value to a number; if finds a '.' scales
   by 1000 for khz based values */
int cliGetNum(char *prompt) {
  char *opt;
  char c;
  int result = 0;
  int scale = 0;
  opt = cliGetOption(prompt);
  while(1) {
    c=*opt;
    switch(c){
    case ' ':
    case '\n':
    case '\r':
    case '\0':
      cliPtr = opt; // update the pointer
      return(result);
      break; // not actually needed
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      if(scale==0) {
	result = 10*result + (c - '0');
      }else{
	result = result + ((c - '0') * scale);
	scale = scale / 10;
      }
      opt++;
	break;
    case '.':
	result = result * 1000;
	scale = 100;
	opt++;
      break;
    default:
      cliPrint("Please enter only Digits\n");
      result = 0;
      scale = 0;
      cliPtr = cliBuf;
      cliBuf[0] = 0;
      opt = cliGetOption(prompt);
      break;
    }
  }
} 
void cliUpdatePtr(char *ptr) {
     cliPtr = ptr; // update the pointer}
}
int cliGetHex(char *prompt) {
  char *opt;
  char c;
  int result = 0;
  opt = cliGetOption(prompt);
  while(1) {
    c=*opt;
    switch(c){
    case ' ':
    case '\n':
    case '\r':
    case '\0':
      cliPtr = opt; // update the pointer
      return(result);
      break; // not actually needed
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      result = 16*result + (c - '0');
      opt++;
	break;
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
      result = 16*result + (10+(c - 'a'));
      opt++;
	break;
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
      result = 16*result + (10+(c - 'A'));
      opt++;
	break;
    default:
      cliPrint("Please enter only Digits\n");
      result = 0;
      cliPtr = cliBuf;
      cliBuf[0] = 0;
      opt = cliGetOption(prompt);
      break;
    }
  }
} 
