#!/usr/bin/env python

import math
import os
import re
import sys

def convert_file(filename):
    sys.stdout.write('Converting "%s"' % filename)
    with open(filename) as fh:
        width = int(re.search(r'(\d+)$', fh.next()).group())
        array_width = int(math.ceil(width / 8.)) * 8
        height = int(re.search(r'(\d+)$', fh.next()).group())
        name = re.search(r'(\w+)_bits', fh.next()).groups()[0]

        data = ['' for _ in xrange(height)]
        index = 0
        for line in fh:
            for hex_match in re.finditer('0x[0-9a-f]{2}', line, re.I):
                data[index / array_width] += '{:08b}'.format(eval(hex_match.group()))[::-1]
                index += 8

    sys.stdout.write(' into PCD8544 compatible data, "%s.h"...' % name)
    if os.path.exists(name + '.h') and not '-f' in sys.argv:
        print ' Error: File exists. Supply -f on the command line.'
        return

    with open(name + '.h', 'w') as out:
        out.write("uint8_t %s_%dx%d[%d][%d] = {\n" % (name, width, height, height / 8, width))
        for line in xrange(0, height, 8):
            out.write("{")
            for col in xrange(width):
                out.write('0x%02x' % int(''.join(data[line + row][col] for row in xrange(7, -1, -1)), 2))
                if col + 1 != width:
                    out.write(', ')
            if line + 8 != height:
                out.write("},\n")
            else:
                out.write("}\n")
        out.write("};\n")
    sys.stdout.write(" DONE\n")

if __name__ == '__main__':
    if sys.argv[1:]:
        for f in sys.argv[1:]:
            if not f.startswith('-'):
                convert_file(f)
