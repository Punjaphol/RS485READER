/*
  Nat.h - Library for RS485 communication code.
  For Arduno Mega2560 + NodeMCU V2 + RS232shield(Zeed studio)+RS485 to RS232 converter
  Created by Mr Punjaphol k., Aug 27, 2016.
  Email:Punjaphol-K@nidec-shibaura.co.th
*/
#include "Arduino.h"
#include <SoftwareSerial.h>
#ifndef Nat_h
#define Nat_h


class Nat
{
  public:
    bool _printdebug;
    String MeterType;
    Nat(bool WithDebug);
    String ScanMemory(byte Device485Address,byte function_code,word start_address,word end_address,byte word_length);
    void init(int baudrate);
    void printdbg(String msg);
    void sendToNodeMCU(String msg);
    void notify();
    bool SendtoPC(char*  HOST,String sts);
    
  private:
    byte _cmd[8] = {0x01,0x03,0x00,0x0F,0x00,0x01};
    word CRC16 (const byte *nData, word wLength);
    String getValueMem(char *cmd,int len);
    String getStringPartByNr(String data, char separator, int index);
};

#endif

