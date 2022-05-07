import serial
from inputimeout import inputimeout, TimeoutOccurred

ser = serial.Serial('/dev/serial0', 115200, timeout=1)
if ser.isOpen():
	ser.close()
ser.open()

ser.write(b"Teensy Connected to Pi!\r\n")

try:
	while 1:
		response = ser.readline().decode("utf-8")
		if len(response) > 0:
			print(response)
			ser.write(b"Pi received msg!\r\n")
except KeyboardInterrupt:
	ser.close()
