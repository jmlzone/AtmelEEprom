#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <wiring.h>
#include "pins_arduino.h"
#endif
#include <atmelbbio.h>

void atmelbbio_setup() {                
  // initialize the digital pin as an output.
  pinMode(CS, OUTPUT);
  pinMode(CK, OUTPUT);
  pinMode(mMOSI, OUTPUT);
  pinMode(mMISO, INPUT);
}

//Write out the N low order bits of data as MSB first
//and return the collected data.
uint32_t write_N_MSB_first(uint32_t data, int n)
{
  uint32_t ret = 0;
  
  //First shift everything to the 16th bit
  data <<= (32-n);
  digitalWrite(mMOSI, LOW); //Make the scope trace look nice
  delay(1);
  
  for(int i=0; i < n; i++)
  {
    digitalWrite(mMOSI, (bool)(data& (uint32_t(1)<<31)));
    delay(1);
    digitalWrite(CK, HIGH);
    delay(1);
    digitalWrite(CK, LOW);
    delay(1);
    int r = digitalRead(mMISO);
    ret <<=1;
    ret |= r;
    data<<=1;   
  }
  digitalWrite(mMOSI, LOW); //Make the scope trace look nice  
  return ret;
}

uint8_t readEEByte(uint8_t addr)
{
  digitalWrite(CS,1);
  write_N_MSB_first(0x300 | (addr&0x7f), 10);
  int v = write_N_MSB_first(0, 8);
  digitalWrite(CS, 0);
  delay(1);
  return v;
}

void writeEEByte(uint8_t addr, uint8_t val, bool confirm)
{
  char lbuf[24];
  digitalWrite(CS,1);
  write_N_MSB_first(0x280 | (addr&0x7f), 10);
  write_N_MSB_first(val, 8);
  digitalWrite(CS,0);
  delay(15);
  if(confirm) {
    sprintf(lbuf,"wrote [%02X] = %02X\n",addr,val);
    Serial.print(lbuf);
  }
}

void ewen()
{
  digitalWrite(CS,1);
  write_N_MSB_first(0x260, 10);
  digitalWrite(CS,0);
  delay(1);
}
 
void ewds()
{
  digitalWrite(CS,1);
  write_N_MSB_first(0x200, 10);
  digitalWrite(CS,0);
  delay(1);
}
