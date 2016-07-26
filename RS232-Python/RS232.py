# -*- coding: utf-8 -*-
import serial
import os
import threading
import numpy as np
from time import sleep
from PyQt5 import uic
Form = uic.loadUiType('gui-matplotlib.ui')  # Load ui
from PyQt5 import QtCore
from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout
import sys

############################## Main Window ###################################
class MyWindow(QMainWindow, Form):
    def __init__(self):
        Form.__init__(self)
        QMainWindow.__init__(self)
        self.setupUi(self)

        self.open_pushButton.clicked.connect(self.open)
        self.close_pushButton.clicked.connect(self.close)

        self.string = ''
        self.txt = []
        self.python_list = []
        self.c_list = []

        self.ser_read = serial.Serial(port='COM2', baudrate = 1200)  
        self.ser_read.close()
    
    ##### Open Port Push Button 
    def open(self):
        self.lcdNumber.setSegmentStyle(2)
        if self.ser_read.isOpen() == False:     
            self.ser_read.open()
            self.port_label.setText("Port COM2 opened!")
            self.ByteR_lineEdit.setText('')
        
            self.t = RS232Thread(self.ser_read)
            self.t.update_trigger.connect(self.update_txtbox)  
            self.t.start()
        else:
            self.port_label.setText("Port is already open!")

    ##### Close Port Push Button
    def close(self):   
        if self.ser_read.isOpen() == True:
            self.ser_read.close()
            self.t.flag = False
            self.port_label.setText("Port COM2 closed!")
        else:
            self.port_label.setText("Port is already closed!")

    ##### Update Received Data
    def update_txtbox(self, string, txt_list, python_list, c_list):
        self.string = string
        self.txt = txt_list
        self.ByteR_lineEdit.setText(self.string)
        self.lcdNumber.display(self.string)
        self.python_list = python_list
        self.c_list = c_list
        self.python_lineEdit.setText(str(self.python_list))
        self.c_lineEdit.setText(str(self.c_list))
        if (python_list == c_list):
            self.output_label.setText("Sent and Received Data are Equal!")
        else:
            self.output_label.setText("Sent and Received Data are Different!")

##############################################################################
##### RS232 Thread 

class RS232Thread(QtCore.QThread):
    update_trigger = QtCore.pyqtSignal(str, list, list, list)
    def __init__(self, serial):
        QtCore.QThread.__init__(self)
        self.ser = serial
        self.flag = True
        self.txt = []
        self.string = ''
        self.python_path = os.path.join(os.getcwd(), "pythonOutput.txt")
        self.c_path = os.path.join(os.path.dirname(os.path.dirname(os.getcwd()))
                                   , "RS232-C++", "Debug", "c++Output.txt")

    def run(self):
        while self.flag:
            str_out = self.ser.read() 
            self.string = self.string + str_out.decode()
            self.txt.append(str(str_out))

            with open("pythonOutput.txt", 'w') as f:
                self.txt = [s.strip('b') for s in self.txt]
                self.txt = [s.strip("'") for s in self.txt]
                self.txt = ' '.join(self.txt).replace('\\x00','').split()
                self.txt = ' '.join(self.txt).replace('\\xcc','').split()
                f.writelines(self.txt)

            python_file = open(self.python_path, "r")
            c_file = open(self.c_path, "r")
            self.python_list = python_file.readlines()
            self.c_list = c_file.readlines() 

            self.update_trigger.emit(self.string, self.txt, self.python_list, self.c_list)

###############################################################################
##### Main Window 
app = QApplication(sys.argv)
window = MyWindow()
window.show()
sys.exit(app.exec_())