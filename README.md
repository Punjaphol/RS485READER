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
