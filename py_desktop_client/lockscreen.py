import os
import subprocess
from tkinter import *
from tkinter import messagebox
import ctypes
import serial
from serial.tools import list_ports
from time import sleep, time
win = Tk()
win.attributes('-topmost', True)
win.withdraw()

def showinfo(msg):
    messagebox.showinfo(title='NOTICE:',  message=msg, parent=win)
def showwarning(msg):
    messagebox.showwarning(title='WARN:', message=msg, parent=win)
def showerror(msg):
    messagebox.showerror(title='ERROR:', message=msg, parent=win)
def askquestion(ques) -> str:
    return messagebox.askquestion("QUESTTION:", ques, parent=win)

def main():
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
        subprocess.run('taskkill /f /fi "IMAGENAME eq lockscreen.exe"', shell=True)
        os._exit(1) #prevent previous line not working
    showinfo('Now the timer is RUNNING.')
    header = b'\xaa\x55'
    while(True):
        pipe.timeout = None
        try:
            recvinfo = pipe.read_until(b':')
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
                os._exit(1)
        except serial.SerialException as err:
            if 'Permission' in err.args[0]:
                showwarning(
                    f'{pipe.name} permission denied!\nmaybe disconnected from PC now.')
            else:
                showerror(err)
            os._exit(1)
    
#end of @main


if __name__ == '__main__':
    main()