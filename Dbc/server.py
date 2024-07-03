import socket
import json
from datetime import datetime

HOST = '127.0.0.1'
PORT = 12345

def handle_client_connection(client_socket):
    while True:
        data = client_socket.recv(1024)
        if not data:
            break
        message = json.loads(data.decode())
        print("Received message:", message)
        current_time = datetime.now()
        current_time_str = current_time.strftime("%H:%M:%S.%f:%f")
        response = {"message": message, "time": current_time_str}
        client_socket.send(json.dumps(response).encode())

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((HOST, PORT))
    server_socket.listen(5)
    print("Server listening on {}:{}".format(HOST, PORT))
    
    try:
        while True:
            client_socket, _ = server_socket.accept()
            print("Connected to client:", client_socket.getpeername())
            handle_client_connection(client_socket)
    except KeyboardInterrupt:
        print("\nServer shutting down...")
    finally:
        server_socket.close()

if __name__ == "__main__":
    main()

