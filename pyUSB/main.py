import serial
import os
import time
import math

def AD7799_ConvTo_mV(Data, Vref, Gain, N, Pol):
    if Pol:
        result = (Data / (2 ** (N - 1)) - 1) * Vref / Gain
    else:
        result = (Data * Vref) / ((2 ** N) * Gain)
    return result * 1000.0

try:
    ser = serial.Serial('COM5', 115200, timeout=0)
    k = 0
    win = 5
    buf = []
    #filt = True
    filt = False
    while True:
        data = ser.read(3)
        if len(data) == 3:
            int_le = int.from_bytes(data, 'little')
            mv = AD7799_ConvTo_mV(int_le,2.5,2.0,24,0)
            Temperature = -8.451576E-06 * mv**2 - 1.769281E-01 * mv + 2.043937E+02;
            if filt:
                buf.append(round(Temperature, 4))
                k += 1
                if k>=win:
                    os.system('cls')
                    result = sorted(buf)[math.ceil(win/2)]
                    buf = []
                    k = 0
                    print(f"Temperature (wind): {result}\n")
            else:
                os.system('cls')
                print(f"Temperature (no wind): {Temperature:.4f}\n")
            #print(f"ADC: {data}, ADC int: {int_le},ADC mv: {mv}, Temperature: {result}")
except KeyboardInterrupt:
    pass
finally:
    ser.close()
