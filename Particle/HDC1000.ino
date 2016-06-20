// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// HDC1000
// This code is designed to work with the HDC1000_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Temperature?sku=HDC1000_I2CS#tabs-0-product_tabset-2

#include <application.h>
#include <spark_wiring_i2c.h>

// HDC1000 I2C address is 0x40(64)
#define Addr 0x40

double cTemp = 0.0, fTemp = 0.0, humidity = 0.0;
int temp = 0, hum = 0;

void setup()
{
  // Set variable
  Particle.variable("i2cdevice", "HDC1000");
  Particle.variable("humidity", humidity);
  Particle.variable("cTemp", cTemp);

  // Initialise I2C communication
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(9600);

  // Start I2C Transmission
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

  // Start I2C Transmission
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
  temp = ((data[0] * 256) + data[1]);
  cTemp = (temp / 65536.0) * 165.0 - 40;
  fTemp = cTemp * 1.8 + 32;

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Send humidity measurement command
  Wire.write(0x01);
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
  hum = ((data[0] * 256) + data[1]);
  humidity = (hum / 65536.0) * 100.0;

  // Output data to dashboard
  Particle.publish("Relative Humidity : ", String(humidity));
  delay(1000);
  Particle.publish("Temperature in Celsius : ", String(cTemp));
  delay(1000);
  Particle.publish("Temperature in Fahrenheit : ", String(fTemp));
  delay(1000);
}

