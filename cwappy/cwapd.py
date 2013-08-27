#!/usr/bin/python

import datetime
import os
import serial
import threading
import time
import shutil
import subprocess
import sys

try:
    import cPickle as pickle
except ImportError:
    import pickle

import libcwap

PICKLE_PATH = os.path.join(os.path.dirname(sys.argv[0]), 'cwap.pickle')

def adjusted_time():
    return int(time.time()) - time.timezone

def str_to_ts(data_str):
    return sum(ord(byte) << ((3 - i) * 8) for i, byte in enumerate(data_str))

class NoiseMaker(threading.Thread):
    def __init__(self):
        self.should_exit = False
        super(NoiseMaker, self).__init__()
        self.daemon = True

    def run(self):
        while not self.should_exit:
            for fn in os.listdir('noise'):
                subprocess.call(('mpg123', os.path.join('noise', fn)))

    def stop(self):
        self.should_exit = True
        subprocess.call(('pkill', '-2', 'mpg123'))

class ArduinoListener(object):

    def __init__(self, serial_port=None, data_rate=9600):
        self.all_data = []
        self.should_exit = False

        self.actions = (
            self.time_request_function,
            self.speaking_clock_request_function,
            self.noise_request_function,
            self.noise_stop_request_function,
            self.alarms_request_function,

            None, #time_set_function
            self.record_data,
            self.record_data,
            self.record_data,
            self.record_data,
            self.record_data,
            self.delete_data,
        )
        libcwap.register(self.actions)

        self.arduino = serial.Serial(
                serial_port or '/dev/ttyACM0',
                data_rate or 9600,
                timeout=10)
        self.arduino.open()
        self.load_data()

    def load_data(self):
        with open(PICKLE_PATH) as fh:
            self.all_data = pickle.load(fh)

    def save_data(self):
        with open(PICKLE_PATH + '_') as fh:
            pickle.dump(self.all_data, fh)
        shutil.move(PICKLE_PATH + '_', PICKLE_PATH)

    def collect_garbage(self):
        old_size = len(self.all_data)
        for data_str in self.all_data:
            if data_str[0] == 'O':
                if str_to_ts(data_str[2:]) < adjusted_time():
                    self.debug('%d was too old, dropping' % ord(data_str[1]))
                    self.all_data.remove(data_str)
        if len(self.all_data) != old_size:
            self.info('Garbage collection removed %d old items' %
                    old_size - len(self.all_data))

    def loop(self):
        while not self.should_exit:
            try:
                self.collect_garbage()
                self.save_data()
                libcwap.action(self.arduino.read)
            except KeyboardInterrupt:
                self.should_exit = True
        self.arduino.close()

    def time_request_function(self):
        self.info('Sending timestamp over serial')
        try:
            self.arduino.write('T')
            ts = adjusted_time()
            for offset in xrange(0, 32, 8):
                self.arduino.write(ts >> offset & 0xff)
        except serial.writeTimeoutError:
            pass

    def speaking_clock_request_function(self, timestamp):
        time_data = datetime.datetime.fromtimestamp(timestamp)
        self.info('Speak time using headphone output for time %s' % time_data)
        # TODO actually speak, and make that a thread please!

    def noise_request_function(self):
        self.info('Speaking time using headphone output')
        self.noiser = NoiseMaker()
        self.noiser.start()

    def noise_stop_request_function(self):
        self.info('Stopping noise')
        if hasattr(self, 'noiser'):
            self.noiser.stop()
            self.noiser.join()
            del self.noiser

    def alarms_request_function(self):
        self.info('Sending all alarm data over serial')
        for data_str in self.all_data:
            success = False
            while not success:
                try:
                    self.arduino.write(data_str)
                except serial.writeTimeoutError:
                    pass
                else:
                    success = True

    def record_data(self, data):
        self.info('Recording incoming data "%s"' % data)
        self.all_data.append(data)

    def delete_data(self, alarmno):
        for data_str in self.all_data:
            if data_str[0] == 'O' and data_str[1] == alarmno:
                self.debug('Alarm %s deleted' % alarmno)
                self.all_data.remove(data_str)
                return
        self.warn('Was told to delete alarm %s, but not found' % alarmno)

    def info(self, string):
        print 'INFO ', string

    def debug(self, string):
        print 'DEBUG', string

    def warn(self, string):
        print 'WARN ', string

if __name__ == '__main__':
    listener = ArduinoListener()
    listener.loop()
