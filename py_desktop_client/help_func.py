from tkinter import *
from tkinter import messagebox

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
# def get_pid_by_name(process_name: str) -> list:
#     processPidList = []
#     for proc in psutil.process_iter():
#         if process_name in proc.name():
#             processPidList.append(proc.pid)
#     return processPidList