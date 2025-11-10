import serial
import os
import csv
import time
import matplotlib.pyplot as plt
from collections import deque

from fontTools.merge.util import current_time


def AD7799_ConvTo_mV(Data, Vref, Gain, N, Pol):
    if Pol:
        result = (Data / (2 ** (N - 1)) - 1) * Vref / Gain
    else:
        result = (Data * Vref) / ((2 ** N) * Gain)
    return result * 1000.0

try:
    ser = serial.Serial('COM5', 115200, timeout=0.01)
    results = []
    #start_time = time.time()
    measurement_number = 1

    '''# Для графика
    window_size = 1000  # количество последних точек, отображаемых на графике
    times = deque(maxlen=window_size)
    temps = deque(maxlen=window_size)
    plt.ion()
    fig, ax = plt.subplots()
    line, = ax.plot([], [], 'r-', label='Температура')
    ax.set_xlabel("Время, с")
    ax.set_ylabel("Температура, °C")
    ax.set_title("Измерение температуры в реальном времени")
    ax.legend()
    ax.grid(True)
    ax.set_ylim(24, 45)'''
    while True:
        data = ser.read(3)
        '''print(f"{data} : {data.hex()}")
        if data:
            break
        else:
            os.system('cls')'''
        if len(data) == 3:
            #int_le = int.from_bytes(data[:3], 'little')
            int_le = int.from_bytes(data, 'little')
            mv = AD7799_ConvTo_mV(int_le, 2.5, 2.0, 24, 0)
            Temperature = -8.451576E-06 * mv**2 - 1.769281E-01 * mv + 2.043937E+02
            #current_time = time.time() - start_time
            #current_time = int.from_bytes(data[4:], 'little')
            os.system('cls')
            print(f"Измерение #{measurement_number}")
            #print(f"Время: {current_time:.3f} c")
            print(f"Z: {int_le}")
            print(f"V: {mv} mV")
            print(f"Temperature: {Temperature:.4f} °C\n")
            #results.append([measurement_number, f"{current_time:.3f}", int_le, f"{Temperature:.4f}"])
            measurement_number += 1

        '''# обновление данных графика
        times.append(current_time)
        temps.append(Temperature)
        line.set_data(times, temps)
        ax.relim()
        ax.autoscale_view()
        plt.pause(0.01)'''
except KeyboardInterrupt:
    '''csv_path = "data.csv"
    with open(csv_path, "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["№ измерения", "Время (с)", "Z", "Temperature (°C)"])
        writer.writerows(results)
    print(f"\nДанные сохранены в {csv_path}")'''
    print(f"\nok")

finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
    '''plt.ioff()
    plt.show()'''