import threading
import time

def calculate_square(numbers):
    for n in numbers:
        print(f"[Квадрат] {n}^2 = {n*n}")
        time.sleep(0.5)

def calculate_cube(numbers):
    for n in numbers:
        print(f"[Куб] {n}^3 = {n*n*n}")
        time.sleep(0.5)

numbers = [2, 3, 4, 5]
thread1 = threading.Thread(target=calculate_square, args=(numbers,))
thread2 = threading.Thread(target=calculate_cube, args=(numbers,))
thread1.start()
thread2.start()
thread1.join()
thread2.join()
print("Обчислення завершені!")
