// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// HDC1000
// This code is designed to work with the HDC1000_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Temperature?sku=HDC1000_I2CS#tabs-0-product_tabset-2

#include<Wire.h>

// HDC1000 I2C address is 0x40(64)
#define Addr 0x40

void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);

  // Starts I2C communication
  Wire.beginTransmission(Addr);
  // Select configuration register
  Wire.write(0x02);
  // Temperature, humidity enabled, resolultion = 14-bits, heater on
  Wire.write(0x30);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
}

void loop()
{
  unsigned int data[2];

  // Starts I2C communication
  Wire.beginTransmission(Addr);
  // Send temp measurement command
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(500);

  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);

  // Read 2 bytes of data
  // temp msb, temp lsb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  // Convert the data
  int temp = (data[0] * 256) + data[1];
  float cTemp = (temp / 65536.0) * 165.0 - 40;
  float fTemp = cTemp * 1.8 + 32;

  // Starts I2C communication
  Wire.beginTransmission(Addr);
  // Send humidity measurement command
  Wire.write(0x01);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(500);

  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);

  // Read 2 bytes of data
  // humidity msb, humidity lsb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data
  float humidity = (data[0] * 256) + data[1];
  humidity = (humidity / 65536.0) * 100.0;

  // Output data to serial monitor
  Serial.print("Relative Humidity :");
  Serial.print(humidity);
  Serial.println(" %RH");
  Serial.print("Temperature in Celsius :");
  Serial.print(cTemp);
  Serial.println(" C");
  Serial.print("Temperature in Fahrenheit :");
  Serial.print(fTemp);
  Serial.println(" F");
  delay(500);
}
