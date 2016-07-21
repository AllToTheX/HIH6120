# import ibmiotf.application
import ibmiotf.device
from time import sleep

def setupBluemix():
    deviceOptions = {
                      'org':'cl6io8',
                      'type':'thermotest',
                      'id':'b827ebb6050f',
                      'auth-method':'token',
                      'auth-token':'raspi123'
    }
    
    deviceCli = ibmiotf.device.Client(deviceOptions)
    deviceCli.connect()
    return deviceCli

def main():
#     bluemix = setupBluemix()
    i = 0
    while True:
        i += 1
        with open('/dev/HIH6120','w') as file:
            file.write('0')
             
        with open('/dev/HIH6120','r') as file:
            temp = file.read()
         
        with open('/dev/HIH6120','w') as file:
            file.write('1')
             
        with open('/dev/HIH6120','r') as file:
            humid = file.read()
         
        HIH6120data = {"Temperature": temp,
                       "Humidity": humid }
    #         bluemix.publishEvent("HIH6120", 'json', HIH6120data)
        print i, temp, humid
        sleep(0.5)
        
    
    pass

if __name__ == '__main__':
    main()