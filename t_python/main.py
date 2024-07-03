# main.py
import threading
import thread_file1
import thread_file2

# Create threads
thread1 = threading.Thread(target=thread_file1.thread1_function, name='Thread 1')
thread2 = threading.Thread(target=thread_file2.thread2_function, name='Thread 2')

# Start threads
thread1.start()
thread2.start()

# Wait for threads to finish
thread1.join()
thread2.join()

print("Both threads have finished execution.")

