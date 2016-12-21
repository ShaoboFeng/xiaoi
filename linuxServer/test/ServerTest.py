import socket 
import time 

address = ('192.168.0.121', 8088)  
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
while True:
        try:  
                s.connect(address)
                break
        except Exception, e:
                time.sleep(0.1)
                continue
count = 0
while True:
    count += 1
    if count > 5:
        s.send('bye')
        break
    s.send('hello')
    data = s.recv(512)
    if len(data)>0:  
        if(data != 'start'):
            continue;
        print 'the data received is',data   
        s.send('hihi')
        #continue
    else:
        break 

s.close()
