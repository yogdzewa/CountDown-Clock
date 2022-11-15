import subprocess
from time import sleep
from tkinter import *
from tkinter import messagebox
from serial import Serial
from os import _exit, system
from infi.systray import SysTrayIcon

win = Tk()
win.attributes('-topmost', True)
win.withdraw()
systray : SysTrayIcon

def showinfo(msg):
    messagebox.showinfo(title='NOTICE:',  message=msg, parent=win)
def showwarning(msg):
    messagebox.showwarning(title='WARN:', message=msg, parent=win)
def showerror(msg):
    messagebox.showerror(title='ERROR:', message=msg, parent=win)
def askquestion(ques) -> str:
    return messagebox.askquestion("QUESTTION:", ques, parent=win)

def tray_exit(sysTray, pipe:Serial):
    pipe.write(b'\xaa\x55DISC')
    _exit(1)

def shutdown_all(systray, pipe: Serial):
    pipe.write(b'\xaa\x55STOP')
    _exit(1)

def toggle_clock_switch(systray, pipe: Serial):
    pipe.write(b'\xaa\x55CONN')

def toggle_light_sensor(systray, pipe: Serial):
    pipe.write(b'\xaa\x55TLIG')
    sleep(1.2)
    pipe.write(b'\xaa\x55TLIG')

def change_mode(systray, pipe: Serial):
    pipe.write(b'\xaa\x55RRST')

def reset(systray, pipe: Serial):
    pipe.write(b'\xaa\x55RSET')

def reboot(systray, pipe: Serial):
    pipe.write(b'\xaa\x55DISC')
    pipe.close()
    subprocess.Popen("F:\\zzzzzz\\term\\user_template\\py_desktop_client\\lockscreen.pyw", shell=True)
    _exit(0)

def tray_create(serial_port: Serial):
    global systray
    menu_options = (
        ("Reboot", None, reboot),
        ("Shutdown All", None, shutdown_all),
        ("Light Sensor Switch", None, toggle_light_sensor),
        ("Reset", None, reset),
        ("Clock Switch", None, toggle_clock_switch),
        ("Mode Change", None, change_mode))
    systray = SysTrayIcon("D:\\ico\\1.ico", "Countdown Clock",
                          menu_options=menu_options, default_menu_index=6, on_quit=tray_exit,
                          tmp_arg=serial_port)
    systray.start()
