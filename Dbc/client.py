import socket
import json

HOST = '127.0.0.1'
PORT = 12345

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((HOST, PORT))
    
    try:
        while True:
            message = input("Enter message: ")
            if message.lower() == "quit":
                break
            client_socket.send(json.dumps({"message": message}).encode())
            response = client_socket.recv(1024)
            response_data = json.loads(response.decode())
            print("Server response:", response_data)
    finally:
        client_socket.close()

if __name__ == "__main__":
    main()

