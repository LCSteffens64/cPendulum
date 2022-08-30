import serial

# use Device manager and look under ports to find the one your arduino is on.
# usually COMX where X is a digit.
ser = serial.Serial('COM7',9600)
ser.flushInput()

while True:
    ser_bytes = ser.readline()
    decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode("utf8")
    print(decoded_bytes)
    with open("sampledata.csv","a") as f:
        f.write(decoded_bytes)
        f.write("\n")


