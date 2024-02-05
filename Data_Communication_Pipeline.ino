#include <PZEM004Tv30.h>

PZEM004Tv30 pzem(12, 13); // Software Serial pins 12 (RX) & 13 (TX)

#include <ModbusIP.h>
#include <Modbus.h>
#include <Ethernet.h>
#include <SPI.h>
int setPoint;
const int setPoint_IR = 100;
ModbusIP mb;

void setup() {
  Serial.begin(9600);
  Serial2.begin(115200,SERIAL_8N1);

    // put your setup code here, to run once:
  Serial.begin(9600);
  byte mac [] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  byte ip[] = {172,16,41,240};
  mb.config (mac, ip);
  
  mb.addIreg(setPoint_IR);
}

void loop() {
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();

  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println("V");

  Serial.print("Current: ");
  Serial.print(current);
  Serial.println("A");

  Serial.print("Power: ");
  Serial.print(power);
  Serial.println("W");

  Serial.print("Energy: ");
  Serial.print(energy, 3);
  Serial.println("kWh");

  Serial.print("Frequency: ");
  Serial.print(frequency, 1);
  Serial.println("Hz");

  Serial.print("PF: ");
  Serial.println(pf);
  Serial.println();

///SolarVoltageSection
    // read the input on analog pin 0:

  int sensorValue = analogRead(A9);

  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 50V):

  int solarVoltage = sensorValue * (5.0 / 1024.0)*3.4; //voltage = value * (5.0/1023) * ((R1 + R2)/R2);
    // print out the value you read:
  //Serial.println(sensorValue);
  Serial.print("Voltage: ");
  Serial.print(solarVoltage);
  Serial.println(" V");

    // Sending Frequency to PLC 
  mb.task ();
  setPoint = frequency;
  mb.Ireg(setPoint_IR, setPoint);
  Serial.print("sensorValue = ");
  Serial.println(setPoint);

  // Concatenate the values into a string and send to ESP32
  //String dataString = "V:" + String(voltage) + "|C:" + String(current) + "|P:" + String(power) +
                    //  "|E:" + String(energy) + "|F:" + String(frequency) + "|PF:" + String(pf);
                      
  String dataString = String(voltage) + "$" + String(current) + "$" + String(power) +
                      "$" + String(energy) + "$" + String(frequency) + "$" + String(pf)+ "&" + String(solarVoltage);

  // Send the concatenated string to Serial2
  Serial2.println(dataString);
  
  delay(2000);
}
