from __future__ import print_function
import ibmiotf.device
from time import sleep
import sys

import uuid

def setupBluemix():
    org = 'cl6io8'
    type = 'thermotest'
    id = 'b827ebb6050f' # str(uuid.uuid4())
    auth_method = 'token'
    auth_token = 'raspi123'
    
    print(" Organization:\t\t%s\n" % org,
          "Type:\t\t\t%s\n" % type,
          "ID:\t\t\t%s\n" % id,
          "Authentication Method:\t%s\n" % auth_method,
          "Authentication Token:\t%s\n" % auth_token)
    
    deviceOptions = {
                      'org':org,
                      'type':type,
                      'id':id,
                      'auth-method':auth_method,
                      'auth-token':auth_token
    }
    
    deviceCli = ibmiotf.device.Client(deviceOptions)
    deviceCli.connect()
    return deviceCli

def main():
    print(chr(27) + "[2J")
    HIH6120 = '/dev/HIH6120-stub'
    bluemix = setupBluemix()
    print("#\tTemperature:\tHumidity:")
    i = 0
    while True:
        i += 1
        with open(HIH6120,'w') as file:
            file.write('0')
             
        with open(HIH6120,'r') as file:
            temp = file.read()
         
        with open(HIH6120,'w') as file:
            file.write('1')
             
        with open(HIH6120,'r') as file:
            humid = file.read()
         
        HIH6120data = {"Temperature": temp,
                       "Humidity": humid }
        if bluemix.publishEvent("HIH6120", 'json', HIH6120data):
            print('\r%d\t%s\t\t%s\t' %( i, temp, humid), end="")
        else:
            print("Event Publish failed :-(")
        sys.stdout.flush()
        sleep(1)
        
    
    pass

if __name__ == '__main__':
    main()