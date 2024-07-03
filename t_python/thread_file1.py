# thread_file1.py
import time
import global

def thread1_function():
    for _ in range(5):
        global.shared_data += 1
        print(f"Thread 1: Shared data = {global.shared_data}")
        time.sleep(1)

