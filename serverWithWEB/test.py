import time, threading

# 新线程执行的代码:
def loop(a):
    print(a)



print('thread %s is running...' % threading.current_thread().name)
a=2
t = threading.Thread(target=loop, args=(a,))
t.start()
print('thread %s ended.' % threading.current_thread().name)