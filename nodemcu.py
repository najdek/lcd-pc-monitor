# Importing Libraries
import serial
import time
import string, random
import crc8
import requests

arduino = serial.Serial(port='COM9', baudrate=115200, timeout=.1)



Loop = True

counter = 0
while (Loop):
    counter += 1
    if (counter > 12):
        counter = 1

    if (counter % 2 == 0):
        cpu_fan_icon = 0
        gpu_fan_icon = 0
    else:
        cpu_fan_icon = 1
        gpu_fan_icon = 1

    if (counter == 1):
        try:
            mininginfo = requests.get(url='http://127.0.0.1:4067/summary').json()
            hashrate = round(float(mininginfo['hashrate_minute']/100000))
        except:
            hashrate = 0
    try:
        hwinfo = requests.get(url='http://127.0.0.1:55555/').json()
        for sensor in hwinfo:
            sensorClass = sensor['SensorClass']
            sensorName = sensor['SensorName']
            if sensorName == "Total CPU Usage":
                cpu_usage = round(float(sensor['SensorValue'].replace(',','.')))
            if sensorClass == "GIGABYTE X570 AORUS PRO (ITE IT8688E)" and sensorName == "CPU":
                cpu_fan = int(sensor['SensorValue'])
                if (cpu_fan > 1450):
                    cpu_fan = 1450
                cpu_fan = round(cpu_fan/1450 * 100)
                if cpu_fan == 0:
                    cpu_fan_icon = 0
            if sensorName == "CPU Die (average)":
                cpu_tmp = round(float(sensor['SensorValue'].replace(',','.')))
            if sensorName == "GPU Core Load":
                gpu_usage = int(sensor['SensorValue'])
            if sensorName == "GPU Fan1" and sensor['SensorUnit'] == "%":
                gpu_fan = int(sensor['SensorValue'])
                if gpu_fan == 0:
                    gpu_fan_icon = 0
            if (counter <= 6):
                if sensorName == "GPU Temperature":
                    gpu_tmp = int(sensor['SensorValue'])
                    gpu_tmp_type = 0
            else:
                if sensorName =="GPU Memory Junction Temperature":
                    gpu_tmp = int(sensor['SensorValue'])
                    gpu_tmp_type = 1
            if sensorName == "Physical Memory Load":
                ram_usage = round(float(sensor['SensorValue'].replace(',','.')))
            if sensorName == "Current UP rate":
                net_up = round(float(sensor['SensorValue'].replace(',','.')))
                if net_up > 999999:
                    net_up = 999999
            if sensorName == "Current DL rate":
                net_dn = round(float(sensor['SensorValue'].replace(',','.')))
                if net_dn > 999999:
                    net_dn = 999999







    #    cpu_usage = random.randint(0, 100)
    #    cpu_fan_icon = random.randint(0, 1)
    #    cpu_fan = random.randint(0, 100)
    #    cpu_tmp = random.randint(0, 100)
    #    gpu_usage = random.randint(0, 100)
    #    gpu_fan_icon = random.randint(0,1)
    #    gpu_fan = random.randint(0,100)
    #    gpu_tmp_type = random.randint(0,1)
    #    gpu_tmp = random.randint(0,110)
    #    ram_usage = random.randint(0,100)
    #    net_up = random.randint(0,999999)
    #    net_dn = random.randint(0,999999)

        data = 'w'
        data += str(cpu_usage).zfill(3)
        data += str(cpu_fan_icon)
        data += str(cpu_fan).zfill(3)
        data += str(cpu_tmp).zfill(3)
        data += str(gpu_usage).zfill(3)
        data += str(gpu_fan_icon)
        data += str(gpu_fan).zfill(3)
        data += str(gpu_tmp_type)
        data += str(gpu_tmp).zfill(3)
        data += str(ram_usage).zfill(3)
        data += str(net_up).zfill(6)
        data += str(net_dn).zfill(6)
        data += str(hashrate).zfill(3)
        data += ';'

        print(data)

        arduino.write(bytes(data, 'utf-8'))
    except:
        print("error")

    #response = arduino.readline()
    #print(response)
    time.sleep(1)
