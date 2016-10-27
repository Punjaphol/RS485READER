
/*RS485Modbus RTU(Powermeter) online to NETPIE.
 * 
 * This supported for Arduno Mega2560 + NodeMCU V2 + RS232shield(Zeed studio)+RS485 to RS232 converter+9 volt adapter power supply
 * When connected to 
 * Wiring How to?
 * 
 * Wiring between Mega and NodeMCU V2
 * Mega           NodeMcu
 * Tx1------------ Rx0
 * Rx1------------Tx0
 * 
 * Wiring between Mega and RS232shield 
 * Remove all jumper from RS232shield ,Don't use digital pin selector.
 * Mega           RS232shield
 * Tx3------------Rx
 * Rx3------------Tx
 * 
 *  * Wiring between RS485converter and RS232shield 
 * RS485to232converter(DB9)      RS232shield(DB9)
 * pin5--------------------------pin5
 * pin3--------------------------pin2
 * pin2--------------------------pin3
 * 
 * 
 *  *  * Wiring between RS485converter and PowerMeter eg.Taik-T250
 * RS485to232converter(conn)      PowerMeter eg.Taik-T250(conn)
 * A------------------------------A
 * B------------------------------B
 * G-------- Ground of Powersupply                  
 * POWER---- +9 Volt of Powersupply
 */
#include "Nat.h"
/*
 * Setting 485 address and range of memory here!
 */
#define METER485_ADDRESS 0x01 // limit 30 device (1E)
#define HOLDING_REGISTER 0x03 // function code
#define MEMORY_START 0x00C8//0x00C8 //200
#define MEMORY_END 0x00F8 //0x00F8//248
#define BAUDRATE 9600 //Baud rate
#define METER_TYPE "JANITZA" //TAIK
int reset_counter=0;
String buff="";
/*
 * 
 */
Nat ExtremeMegaBoard(0);// build the Nat object
void setup()
{
      ExtremeMegaBoard.MeterType=METER_TYPE;
      ExtremeMegaBoard.init(BAUDRATE);
      ExtremeMegaBoard.printdbg("Ready\r\n");
      ExtremeMegaBoard.MeterType=METER_TYPE;
}
void loop() 
{ 
       String buff="";   
      if( METER_TYPE == "TAIK" ){
        buff=READ_TAIK(); 
      }
      if( METER_TYPE == "JANITZA" ){
        buff=READ_JANITZA(); 
      }
       ExtremeMegaBoard.sendToNodeMCU(buff);
       ExtremeMegaBoard.printdbg(buff + "\r\n");
 }


String READ_TAIK(){
           String buff="";
           ExtremeMegaBoard.notify();// notify hardware before read
           buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x01F8,0x01F8,0x01);
           buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x0200,0x0202,0x01);
           buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x0207,0x0209,0x01);
           buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x020E,0x0210,0x01);
           buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x0215,0x0217,0x01);
           buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x0219,0x021C,0x01);
           buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x0108,0x0109,0x02); // Zixma WH
           buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x0100,0x0101,0x02); // Hour scale
           buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x000D,0x000D,0x01); // 
           return buff;
}

String READ_JANITZA(){
          String buff="";
          ExtremeMegaBoard.notify();// notify hardware before read
          buff="2.5001|" ;//unit--0
          buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x00CE,0x00CE,0x01);//I-R--1
          buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x00C8,0x00C8,0x01);//V-RN--2
          buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x00CB,0x00CB,0x01);//V-RS--3
          buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x00CF,0x00CF,0x01);//I-S--4
          buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x00C9,0x00C9,0x01);//V-SN--5
          buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x00CC,0x00CC,0x01);//V-ST--6
          buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x00D0,0x00D0,0x01);//I-T--7
          buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x00CA,0x00CA,0x01);//V-TN--8
          buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x00CD,0x00CD,0x01);//V-TR--9
          buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x0119,0x0119,0x01);//ΣA--10
          buff+="0|" ;//ΣV--11
          buff+="0|" ;//ΣVL--12
          buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x0117,0x0117,0x01);//ΣW--ok--13
          buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x01AE,0x01AF,0x02);//ΣVAR--ok--14
          buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x0114,0x0114,0x01);//ΣPF--ok--15
          buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x0113,0x0113,0x01);//F--ok--16
          buff+=ExtremeMegaBoard.ScanMemory(METER485_ADDRESS,HOLDING_REGISTER,0x01A0,0x01A1,0x02);//ΣWH--ok--17
          buff+="0.4000001|" ;//Hour Scale--18
          buff+="120|" ;//CT-Ratio--19
          return buff;
}


 
