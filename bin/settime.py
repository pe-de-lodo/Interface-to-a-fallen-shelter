#!python 
import sys
import datetime
import time
import serial
port = sys.argv[-1]
if len(sys.argv)==1:
    print("settime.py {SERIAL_PORT}")
    exit()
now=datetime.datetime.now().replace(microsecond=0).isoformat()
ser = serial.Serial(port, 115200, timeout=0.5)
print(port)
print(now)
ser.write(bytes(f"\r\ndate {now}\r\n","ASCII"))
time.sleep(0.5)
response = ser.read_all()
print(response)
ser.close()