# thread_file2.py
import time
import global

def thread2_function():
    for _ in range(5):
        global.shared_data += 1
        print(f"Thread 2: Shared data = {global.shared_data}")
        time.sleep(1)

