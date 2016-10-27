/*
  Nat.h - Library for RS485 communication code Arduno Mega2560 + NodeMCU V2 + RS232shield(Zeed studio)+RS485 to RS232 converter
  Created by Mr Punjaphol k., Aug 27, 2016.
  Email:Punjaphol-K@nidec-shibaura.co.th
*/
    #include "Arduino.h"
    #include <stdlib.h>
    #include <SoftwareSerial.h>
    #include "Nat.h"


    Nat::Nat(bool WithDebug) //constructor
    {
      _printdebug = WithDebug;
    }
    
    void Nat::init(int baudrate){
      Serial.begin(baudrate);//Debugger
      Serial1.begin(baudrate); // Connect to Node MCU
      Serial3.begin(baudrate); // COnnect RS485
    }
    void Nat::sendToNodeMCU(String msg){
      Serial1.print(msg);
      Serial1.print('#');
    }
    void Nat::printdbg(String msg){
      Serial.print(msg);
    }
    void Nat::notify(){
        Serial3.flush();
        while (Serial3.available() > 0) {
          byte inByte = Serial3.read(); 
          delay(10);
        }
        Serial3.flush();
        if(_printdebug){Serial.println("METER:" + MeterType);}
    }
    String Nat::ScanMemory(byte Device485Address,byte function_code,word start_address,word end_address,byte word_length){
    String buff="";
    int counter=(int)start_address;
    int endder=(int)end_address;
    while(1){
      _cmd[0]=Device485Address;
      _cmd[1]=function_code;
      _cmd[2]=highByte(counter);
      _cmd[3]=lowByte(counter);
      _cmd[4]=0x00; //just read 1 memory address high byte
      _cmd[5]=word_length ;//1-255
      word a=this->CRC16(_cmd,6);
      int crc=(int)a;
      _cmd[6]=(byte)lowByte(crc);//Crc Low byte
      _cmd[7]=(byte)highByte(crc);//CrC High Byte
      if(_printdebug){
      Serial.print("Addrc=");
      Serial.print(_cmd[2],HEX);Serial.print(_cmd[3],HEX);
      Serial.print(";crc=");
      Serial.print(_cmd[6],HEX);
      Serial.println(_cmd[7],HEX);
      }
      buff+=( this->getValueMem(_cmd,sizeof(_cmd)));buff+="|";

      counter+=int(word_length);
      if(counter>endder){break;}
    }
    return buff;
}

String Nat::getValueMem(char *cmd,int len){
        String buffer_recv[(len*2)+5];
        String ret="";
        int i=0;
        String t="";
        
     Serial3.flush();
        Serial3.write(cmd,len);
        
      if(_printdebug){
        Serial.print("Send=");
        for(i=0;i<len;i++)
        {
        t=(String(byte(cmd[i]),HEX).length()>1)?String(byte(cmd[i]),HEX):'0' + String(byte(cmd[i]),HEX) ;
          Serial.print(t + " ");
        } 
        Serial.println();      
      }
      
       delay(100);
        i=0;
         if(_printdebug){Serial.print("Return:");}
        while (Serial3.available() > 0) {
          byte inByte = Serial3.read(); 
          buffer_recv[i]=String(inByte,HEX); //Store at buffer
          buffer_recv[i]=(buffer_recv[i].length()>1)?buffer_recv[i]:'0' + buffer_recv[i]  ;
          if( _printdebug){
            Serial.print(buffer_recv[i]);
            Serial.print(' ');
           }
          delay(1);
          i++;
        }
        if(_printdebug){Serial.println();};
        
     String buff="";
     int startpos=3;
     int endpos;
   
      if(MeterType=="TAIK")
      {
           //01 03 04 00 00 00 05 3a 30  <=case=====2 word
           //01 03 04 00 00 3a 30  <=case=====1 word
            startpos=3;
            endpos=(int(cmd[5])* 2)+ startpos;         
           for(i=startpos;i<endpos;i++){
                buff+=buffer_recv[i];
           }
      }
      if(MeterType=="JANITZA")
      {
           //  <=case=====2 word
           //01 03 00 c8 00 01 05 f4  <=case=====1 word
            startpos=3;
            endpos=(int(cmd[5])* 2)+ startpos;
           for(i=startpos;i<endpos;i++){
                buff+=buffer_recv[i];
           }
      }
      
      if(_printdebug){ Serial.println("Value(h)=" + buff);}
            //  int  r= strtol(buff.c_str(), NULL, 16);
             //  ret=(String)(r);
       const char *hex = buff.c_str(); 
       
        uint32_t val = 0;
          while (*hex) {
              // get current character then increment
              uint8_t byte = *hex++; 
              // transform hex character to the 4bit equivalent number, using the ascii table indexes
              if (byte >= '0' && byte <= '9') byte = byte - '0';
              else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
              else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;    
              // shift 4 to make space for new digit, and add the 4 bits of the new digit 
              val = (val << 4) | (byte & 0xF);
          }
         // val=val/2.5001;
        return ret=(String)val;
}



word Nat::CRC16(const byte *nData, word wLength)
{
  static const word wCRCTable[] = {
    0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
    0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
    0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
    0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
    0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
    0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
    0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
    0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
    0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
    0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
    0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
    0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
    0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
    0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
    0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
    0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
    0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
    0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
    0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
    0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
    0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
    0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
    0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
    0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
    0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
    0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
    0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
    0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
    0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
    0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
    0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
    0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };
byte nTemp;
word wCRCWord = 0xFFFF;
   while (wLength--)
   {
      nTemp = *nData++ ^ wCRCWord;
      wCRCWord >>= 8;
      wCRCWord ^= wCRCTable[nTemp];
   }
   return wCRCWord;

}





