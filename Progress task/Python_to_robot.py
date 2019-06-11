import serial
import sys
import datetime

  

file1=open(sys.argv[1],"r")#takes the file name as argument via commandline 
text=file1.read()#reads the file content
text=text+' #' #appends ' #' to the file content
print(text)
ser=serial.Serial()
ser.port=sys.argv[2]
ser.baudrate=9600
print(ser.portstr)

print(datetime.datetime.now().time().__format__('%H:%M:%S'))#time before serial data transfer
ser.open()
ser.write(text.encode())
ser.close()
print(datetime.datetime.now().time().__format__('%H:%M:%S'))# time after serial data transfer
