#encoding=utf8
import SocketServer   
import socket
import json
from SmartDevices import SmartDevicesNode
  
HOST = '192.168.0.121'   
PORT = 8081 
ADDR = (HOST, PORT)   
def packing(data):
    return "####"+data+"****"
def unpacking(data):
    return data[4:-4]
    
class MyRequestHandler(SocketServer.BaseRequestHandler):   
    def handle(self):
        buffer_data = ""
        while True:  
            data = self.request.recv(512)
            print 'old:',data
            if(len(data)>0):
                buffer_data += data
                buffer_data = buffer_data[buffer_data.find("####"):]
                package_end = buffer_data.find("****")
                if package_end < 0:
                    continue
                package = buffer_data[:package_end+4]
                ret = self.deal_data(package)
                buffer_data = buffer_data[package_end:]
                if ret and 'bye' == ret:
                    #self.request.close()
                    break;
                #self.request.send(packing(ret))
            else:  
                break   
        self.request.close()
    def deal_data(self,data):
        data = unpacking(data)
        try:
            net_data = json.loads(data)
            print(net_data)
            if str(net_data['devices']) == 'yes':
                #may be a package from devices
                devices = SmartDevicesNode(net_data)
                devices.deal_package(self.request)
                return
            if str(net_data['action']) == 'bye':
                return "bye"
            if str(net_data['action']) == 'get':
                ret = {}
                ret["status"] = "OK"
                ret["temperature"] = 27
                return json.dumps(ret)
        except:
            self.request.send("ERROR")
            return 'bye'

class MyThreadingTCPServer(SocketServer.ThreadingTCPServer):
    def server_bind(self):
        self.socket.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
        self.socket.bind(self.server_address)

tcpServ = MyThreadingTCPServer(ADDR, MyRequestHandler) 
tcpServ.serve_forever() 
