#encoding=utf8
import SocketServer   
import socket
  
HOST = '192.168.0.121'   
PORT = 8088   
ADDR = (HOST, PORT)   
class MyRequestHandler(SocketServer.BaseRequestHandler):   
    def handle(self):
        while True:  
            data = self.request.recv(512)  
            if(len(data)>0):  
                ret = self.deal_data(data)
                if ret == 1:
                    #self.request.close()
                    break;
                self.request.send("OK") 
            else:  
                break   
        self.request.close()
    def deal_data(self,data):
        if str(data) == 'bye':
            return 1
        print(data)
        return 0

class MyThreadingTCPServer(SocketServer.ThreadingTCPServer):
    def server_bind(self):
        self.socket.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
        self.socket.bind(self.server_address)

tcpServ = MyThreadingTCPServer(ADDR, MyRequestHandler) 
tcpServ.serve_forever() 
