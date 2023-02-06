# NOTICE: the tray icon and reboot path is using absolute address 
# in help_fucn.py

from os import _exit
import ctypes
from serial import SerialException, Serial
from serial.tools import list_ports
from time import sleep, time
from help_func import *
pipe : Serial

def main():
    global pipe
    ports_list = [x for x in list_ports.comports()
                 if 'USB-SERIAL CH340' in x.description]
    len_ports = len(ports_list)
    if len_ports != 1:
        if len_ports == 0:
            showerror('Not Found Existing CH340 COM!')
        elif len_ports > 1:
            showerror('Only Support One Connection!')
        _exit(1)
    try:
        pipe = Serial(ports_list[0].name, 28800)
    except BaseException: # exit
        showerror(
            f'{ports_list[0].name} permission denied!\nMaybe other process\
 is running.\nNow exiting...')
        _exit(1)  # prevent previous line not working

    header = b'\xaa\x55'
    pipe.write(header+b'CONN')
    tray_create(serial_port=pipe)
    showinfo('Now the timer is RUNNING.')

    while(True):
        pipe.timeout = None
        try:
            recvinfo = pipe.read_until(b':')
            #if(recvinfo == None):
                #continue
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
                    pipe.timeout = tmp if tmp > 0 else 0
                    recvinfo = pipe.read_until(b':')
                    if recvinfo != b'':
                        continue
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
                    if recvinfo != b'':
                        break
                    pipe.write(header+b'BEEP')
            elif(recvinfo == b'STOP:'):
                # showinfo('QUIT CURRENT PROCESS!')
                # pipe.write(header+b'DISC')
                # _exit(1)
                continue
        except SerialException as err:
            if 'Permission' in err.args[0]:
                showwarning(
                    f'{pipe.name} permission denied!\nmaybe disconnected from PC now.')
            else:
                showerror(err)
            _exit(1)
        except KeyboardInterrupt:
            showwarning('keyboardint in main()')
            systray.shutdown()
            pipe.write(header+b'DISC')
            _exit(1)
    
#end of @main


if __name__ == '__main__':
    main()