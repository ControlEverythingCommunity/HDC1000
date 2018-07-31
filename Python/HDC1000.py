# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# HDC1000
# This code is designed to work with the HDC1000_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/content/Temperature?sku=HDC1000_I2CS#tabs-0-product_tabset-2

import smbus
import time

# Get I2C bus
bus = smbus.SMBus(1)

# HDC1000 address, 0x40(64)
# Select configuration register, 0x02(02)
#		0x30(48)	Temperature, Humidity enabled, Resolultion = 14-bits, Heater on
bus.write_byte_data(0x40, 0x02, 0x30)

# HDC1000 address, 0x40(64)
# Send temp measurement command, 0x00(00)
bus.write_byte(0x40, 0x00)

time.sleep(0.5)

# HDC1000 address, 0x40(64)
# Read data back, 2 bytes
# temp MSB, temp LSB
data0 = bus.read_byte(0x40)
data1 = bus.read_byte(0x41)

# Convert the data
temp = (data0 * 256) + data1
cTemp = (temp / 65536.0) * 165.0 - 40
fTemp = cTemp * 1.8 + 32

# HDC1000 address, 0x40(64)
# Send humidity measurement command, 0x01(01)
bus.write_byte(0x40, 0x01)

time.sleep(0.5)

# HDC1000 address, 0x40(64)
# Read data back, 2 bytes
# humidity MSB, humidity LSB
data0 = bus.read_byte(0x40)
data1 = bus.read_byte(0x41)

# Convert the data
humidity = (data0 * 256) + data1
humidity = (humidity / 65536.0) * 100.0

# Output data to screen
print "Relative Humidity : %.2f %%" %humidity
print "Temperature in Celsius : %.2f C" %cTemp
print "Temperature in Fahrenheit : %.2f F" %fTemp
