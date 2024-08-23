import subprocess
import socket
import threading
import os

begin = b'\x23\x76\xBF\x00\xFF\x00'
port = 49001

def killxrt():
    try:
        subprocess.run(['pkill', 'xrt']).check_returncode()
    except subprocess.CalledProcessError as e:
        print(f'REAPER: pkill xrt failed with exit code {e.returncode}')

def procdata(buf, it, addr):
    for b in buf:
        if it < len(begin):
            if b != begin[it]:
                print(f'REAPER: Client {addr} failed to provide correct begin: kicking')
            else:
                it += 1

        else:
            if b == 0:
                print(f'REAPER: Killing xrt on request of {addr}')
                killxrt()

    return it

def procsock(conn, addr):
    with conn:
        print(f"REAPER: Connected by {addr}")
        it = 0
        while True:
            buf = conn.recv(4096)
            if not buf:
                break
            it = procdata(buf, it, addr)
            if it < 0:
                break
        print(f"REAPER: Disconnected by {addr}")

if __name__ == '__main__':

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.bind(('0.0.0.0', port))
        sock.listen()

        print (f'REAPER: Waiting for connections on port {port}')

        while True:
            conn, addr = sock.accept()
            threading.Thread(target=procsock, args=(conn, addr)).start()
