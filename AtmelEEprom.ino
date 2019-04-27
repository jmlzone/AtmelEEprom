// eeprom twiddler with simple cli
#include <cli.h>
#include <atmelbbio.h>

enum commandNames {
    DUMP,
    WRITE,
    RESET,
    ALL,
    HELP
};
static cmd_t cmdTable[] = {
"dump",DUMP,
  "write",WRITE,
  "reset",RESET,
  "all",ALL,
  "help",HELP,
  "?",HELP,
  "",-1 /* end of table */
};

int cmd;
char pbuf[32];

//void dump(void);
//void write(void);
//void help(void);

void setup() {
  Serial.begin(9600);
  Serial.println(F("Flash EEPROM widget type ? for help"));
  atmelbbio_setup();
}

void loop() {
   cliGetLine("EE Prom> ");
   cmd = cliGetCmd(cliBuf, cmdTable);
   if(cmd != cliNULLCMD) {
     switch(cmd) {
     case DUMP:
       dump();
       break;
     case WRITE:
       eewrite();
       break;
     case RESET:
       reset30();
       break;
     case ALL:
       dumpAll();
       break;
     case HELP:
       help();
       break;
     default:
       unknown();
     }
   }
}

void help(void) {
  int i;
  cliPrint("Cli Help Message\nCommands are:\n");
  for(i=0; cmdTable[i].cmd != -1; i++){
    cliPrint(cmdTable[i].cmdName);
    cliPrint("\n");
  }
}
void unknown(void) {
    cliPrint("Unknown command was:\n  ");
    cliPrint(cliBuf);
    cliPrint("\n");
}

void dump() {
  int sa, ea, first;
  uint8_t data;
  sa=cliGetHex("Start Address? ");
  ea=cliGetHex("End Address? ");
  first = 1;
  for(int i=sa; i <= ea; i++){
      if(first || (i % 16 == 0)) {
        sprintf(pbuf, "\n0x%02x:", i);
        Serial.print(pbuf);
        first = 0;
      }
      data=readEEByte(i);
      sprintf(pbuf, " %02x", data);
      Serial.print(pbuf);
    }
    Serial.println("");
}

void eewrite(void) {
  int addr;
  uint8_t data;
  addr=cliGetHex("Address? ");
  data=cliGetHex("Data? ");
  writeEEByte(addr,data,0);
  Serial.println("");
}
  
void reset30(void) {
int h,i;
h=0;
  for (i=1; i<=30; i++) {
    h=h+1;
    if((i%10) == 0) {
      h=h+6;
    }
    writeEEByte(h,0x55,1);	
  }
  writeEEByte(0x50,0x29,1);	
  writeEEByte(0x55,0x30,1);
}

void dumpAll(void) {
  unsigned char buf[16];
  char ascii[17];
  int i, j;
  ascii[16] = '\0';
  for (i=0; i<8; ++i) {
    for (j=0; j<16; ++j) {
      buf[j] = readEEByte((i*16) +j); // read eeprom
      if (buf[j] >= ' ' && buf[j] <= '~') {
	ascii[j] = buf[j];
      } else {
	ascii[j] = '.';
      }
    }
    
    sprintf(pbuf,"%02X: ", i*16);
    Serial.print(pbuf);
    for (j=0; j<16; ++j) {
      sprintf(pbuf,"%02X ",buf[j]);
      Serial.print(pbuf);
    }
    sprintf(pbuf,"%s\n",ascii);
    Serial.print(pbuf);
  }
}
