import serial
import os
import csv
import time

def AD7799_ConvTo_mV(Data, Vref, Gain, N, Pol):
    if Pol:
        result = (Data / (2 ** (N - 1)) - 1) * Vref / Gain
    else:
        result = (Data * Vref) / ((2 ** N) * Gain)
    return result * 1000.0

try:
    ser = serial.Serial('COM5', 115200, timeout=0)
    results = []
    start_time = time.time()
    os.makedirs("pyUSB", exist_ok=True)
    measurement_number = 1
    while True:
        data = ser.read(3)
        if len(data) == 3:
            int_le = int.from_bytes(data, 'little')
            mv = AD7799_ConvTo_mV(int_le, 2.5, 2.0, 24, 0)
            Temperature = -8.451576E-06 * mv**2 - 1.769281E-01 * mv + 2.043937E+02
            current_time = time.time() - start_time
            os.system('cls')
            print(f"Измерение #{measurement_number}")
            print(f"Время: {current_time:.3f} c")
            print(f"Z: {int_le}")
            print(f"Temperature: {Temperature:.4f} °C\n")
            results.append([measurement_number, f"{current_time:.3f}", int_le, f"{Temperature:.4f}"])
            measurement_number += 1
except KeyboardInterrupt:
    csv_path = "pyUSB/data.csv"
    with open(csv_path, "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["№ измерения", "Время (с)", "Z", "Temperature (°C)"])
        writer.writerows(results)
    print(f"\nДанные сохранены в {csv_path}")

finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()