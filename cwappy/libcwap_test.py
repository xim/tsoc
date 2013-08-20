#!/usr/bin/env python

import libcwap

def reader(size):
    print "Got read"
    return 'T' * size

def callback(name):
    def actual_callback(*args, **kwargs):
        print name, 'got args', args, 'and kwargs', kwargs
    return actual_callback

actions = (
    callback("time_request_function"),
    callback("speaking_clock_request_function"),
    callback("noise_request_function"),
    callback("alarms_request_function"),

    callback("time_set_function"),
    callback("record_data"),
    callback("record_data"),
    callback("record_data"),
    callback("record_data"),
    callback("record_data"),
    callback("delete_data"),
)

if __name__ == '__main__':
    libcwap.register(actions)
    libcwap.action(reader)
