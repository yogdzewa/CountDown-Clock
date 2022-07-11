import os
import psutil
import signal
import pystray
from concurrent.futures import ProcessPoolExecutor, ThreadPoolExecutor
from tkinter import *
from tkinter import messagebox
import ctypes
import serial
from serial.tools import list_ports
from time import sleep, time
win = Tk()
win.attributes('-topmost', True)
win.withdraw()
pipe = None

def showinfo(msg):
    messagebox.showinfo(title='NOTICE:',  message=msg, parent=win)
def showwarning(msg):
    messagebox.showwarning(title='WARN:', message=msg, parent=win)
def showerror(msg):
    messagebox.showerror(title='ERROR:', message=msg, parent=win)
def askquestion(ques) -> str:
    return messagebox.askquestion("QUESTTION:", ques, parent=win)
def handler(signum, frame):
    if signum == signal.SIGINT:
        print('Signal INT received, now exit')
        pipe.write(b'\xaa\x55DISC')
        os._exit(1)
def get_pid_by_name(process_name: str) -> list:
    processPidList = []
    for proc in psutil.process_iter():
        if process_name in proc.name():
            processPidList.append(proc.pid)
    return processPidList
def wait_for_signal(parent_pid):
    child_pid = os.getpid()
    return


def main():
    global pipe
    # install signal handler
    # signal.signal(signal.SIGINT, handler)
    ports_list = [x for x in list_ports.comports()
                 if 'USB-SERIAL CH340' in x.description]
    len_ports = len(ports_list)
    if len_ports != 1:
        if len_ports == 0:
            showerror('Not Found Existing CH340 COM!')
        elif len_ports > 1:
            showerror('Only Support One Connection!')
        os._exit(1)
    try:
        pipe = serial.Serial(ports_list[0].name, 28800)
    except BaseException:
        showerror(
            f'{ports_list[0].name} permission denied!\nMaybe former process \
is running.\nNow trying to kill it(include this one)')
        pid_list = get_pid_by_name('pythonw.exe')
        for pid in pid_list:
            os.kill(pid, signal.SIGINT)
        os._exit(1)  # prevent previous line not working
    header = b'\xaa\x55'
    pipe.write(header+b'CONN')
    showinfo('Now the timer is RUNNING.')
    pool = ProcessPoolExecutor(1)
    pool.submit(wait_for_signal, os.getpid())

    while(True):
        pipe.timeout = 2
        try:
            recvinfo = pipe.read_until(b':')
            if(recvinfo == None):
                continue
            # print('recvinfo: ', recvinfo)
            if(recvinfo == b'NTOU:'):
                pipe.timeout = 60
                recvinfo = pipe.read_until(b':')
                # print('NTOU: ', recvinfo)
                if recvinfo == b'REST:':
                    continue
                time_tmp = time()
                choice = askquestion(
                    ques='4 minutes left.\nContinue counting down?')
                # print('choice: ', choice)
                if(choice == 'yes'):
                    tmp = 240 - (time() - time_tmp)
                    time_tmp = tmp if tmp > 0 else 0
                    sleep(time_tmp)
                    ctypes.windll.user32.LockWorkStation()
                    sleep(6)
                    pipe.write(header+b'RRST')
                else:  # rest right now
                    pipe.write(header+b'RRST')
            elif(recvinfo == b'RTOU:'):
                pipe.timeout = 180
                while True:
                    recvinfo = pipe.read_until(b':')
                    # print('RTOU:', recvinfo)
                    if recvinfo == b'NOML:':
                        break
                    pipe.write(header+b'BEEP')
            elif(recvinfo == b'STOP:'):
                showinfo('QUIT CURRENT PROCESS!')
                pipe.write(header+b'DISC')
                os._exit(1)
        except serial.SerialException as err:
            if 'Permission' in err.args[0]:
                showwarning(
                    f'{pipe.name} permission denied!\nmaybe disconnected from PC now.')
            else:
                showerror(err)
            os._exit(1)
        except KeyboardInterrupt:
            showwarning('keyboardint in main()')
            pipe.write(header+b'DISC')
            os._exit(1)
    
#end of @main


if __name__ == '__main__':
    main()