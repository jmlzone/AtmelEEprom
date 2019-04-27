#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#define CLIBUFLEN 40
//#define cliGetC Serial.read(); // getc is blocking, shoud be made blocking
#define cliPutc(c) Serial.print(c);
#define cliPrompt "My Cli> "
#define cliEscaped "$Deleted\n"
#define cliRubout "\010 \010"
#define cliCRLF "\n"
#define cliNOTFOUND  -1
#define cliNULLCMD  -2
extern char cliBuf[];
typedef struct cmd_s { /* keword / command values */
  char *cmdName;
  int cmd;
} cmd_t;

void cliGetLine(char *prompt);
void cliPrint(char *msg);
int cliGetCmd(char *buf, cmd_t *table);
int cliMatch(char *cmd, char *buf);
char* cliGetOption(char *prompt);
int cliGetNum(char *prompt);
int cliGetHex(char *prompt);
void cliUpdatePtr(char *ptr);
char cliGetC(void);


