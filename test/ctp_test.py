import os
import re
import subprocess as sp
import unittest as ut

class CtpTestCase(ut.TestCase):

    def setUp(self):
        self.orig_dir = os.getcwd()
        self.top_dir = re.search(r'^.*/ctp', self.orig_dir)
        if self.top_dir is None:
            raise OSError("Current path must contain the project's parent directory")
        else:
            self.top_dir = self.top_dir.group()
        self.bin_dir = self.top_dir + '/bin/'
        self.doc_dir = self.top_dir + '/doc/'
        self.test_dir = self.top_dir + '/test/'
        self.made_files = []

    def tearDown(self):
        for f in self.made_files:
            os.remove(f)
        os.chdir(self.orig_dir)

    def run_program(self, *args, valid_run=True):
        cmd = [self.bin_dir + 'ctp'] + [i for i in args]
        with sp.Popen(cmd, stdout=sp.PIPE, stderr=sp.PIPE) as proc:
            proc.wait()
            out = [l for l in proc.stdout] + [l for l in proc.stderr]
            if (valid_run):
                self.assertListEqual(out, [])
                return None
            else:
                return out

    def make_test_file(self, name, *lines):
        with open(name, 'w') as file:
            for l in lines:
                file.write(l + '\n')
        self.made_files.append(name)

class TestFileCreation(CtpTestCase):

    def test_std_file(self):
        self.make_test_file('test_std_file', 'delay 100')
        self.run_program('test_std_file')
        self.assertTrue(os.path.isfile(self.orig_dir + '/a.prt'))
        os.remove(self.orig_dir + '/a.prt')

    def test_given_file(self):
        self.make_test_file('test_std_file', 'delay 100')
        self.run_program('test_std_file', '-o4081')
        self.assertTrue(os.path.isfile(self.orig_dir + '/4081'))
        os.remove(self.orig_dir + '/4081')

class TestCommands(CtpTestCase):

    def test_valid_check(self):
        self.make_test_file('test_valid_check',
            'check on 1 off rest',
            'check on 1 2 off rest',
            'check off 1 2 on rest',
            'check on 16 off rest',
            'check on 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16',
            'check on 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 off rest')
        self.run_program('test_valid_check', '-w',)
        with open('a.prt', 'rb') as file:
            self.assertEqual(file.read(),
                b'PM1'
                b'\x01\x01\x00'
                b'\x01\x03\x00'
                b'\x01\xFC\xFF' # all bits except 1 and 2
                b'\x01\x00\x80'
                b'\x01\xFF\xFF'
                b'\x01\xFF\xFF'
                b'END')
        os.remove('a.prt')

    def test_error_check(self):
        self.make_test_file('test_error_check',
            'check on 112',
            'check habla',
            'check on habla',
            'check on 1',
            'check 1',
            'check on',
            'check 1 on',
            'check on 1 off 1 rest',
            'check')
        output = self.run_program('test_error_check', '-w', valid_run=False)
        self.assertListEqual(output, [
            b'test_error_check:1:check: error: argument must be either on, off, rest or a pin number (1 - 16)\n',
            b'test_error_check:1:check: error: all pins must be given a value\n',
            b'test_error_check:2:check: error: at least 2 arguments must be given\n',
            b'test_error_check:2:check: error: all pins must be given a value\n',
            b'test_error_check:3:check: error: argument must be either on, off, rest or a pin number (1 - 16)\n',
            b'test_error_check:3:check: error: all pins must be given a value\n',
            b'test_error_check:4:check: error: all pins must be given a value\n',
            b'test_error_check:5:check: error: at least 2 arguments must be given\n',
            b'test_error_check:5:check: error: all pins must be given a value\n',
            b'test_error_check:6:check: error: at least 2 arguments must be given\n',
            b'test_error_check:6:check: error: all pins must be given a value\n',
            b'test_error_check:7:check: error: a value must be given before any pin numbers\n',
            b'test_error_check:7:check: error: all pins must be given a value\n',
            b'test_error_check:8:check: error: pins may only be given a value once\n',
            b'test_error_check:9:check: error: at least 2 arguments must be given\n',
            b'test_error_check:9:check: error: all pins must be given a value\n'
        ])

    def test_valid_set(self):
        self.make_test_file('test_valid_set',
            'set off rest',
            'set on 1 2',
            'set on rest',
            'set off 1 2',
            'set off rest',
            'set on 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16',
            'set off 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 on rest')
        self.run_program('test_valid_set', '-w')
        with open('a.prt', 'rb') as file:
            self.assertEqual(file.read(),
                b'PM1'
                b'\x02\x00\x00'
                b'\x02\x03\x00'
                b'\x02\xFF\xFF'
                b'\x02\xFC\xFF' # all bits except 1 and 2
                b'\x02\x00\x00'
                b'\x02\xFF\xFF'
                b'\x02\x00\x00'
                b'END')
        os.remove('a.prt')

    def test_error_set(self):
        self.make_test_file('test_error_set',
            'set on 112',
            'set habla',
            'set on habla',
            'set on 1 off 1',
            'set on',
            'set 1',
            'set 1 on',
            'set')
        output = self.run_program('test_error_set', '-w', valid_run=False)
        self.assertListEqual(output, [
            b'test_error_set:1:set: error: argument must be either on, off, rest or a pin number (1 - 16)\n',
            b'test_error_set:2:set: error: at least 2 arguments must be given\n',
            b'test_error_set:3:set: error: argument must be either on, off, rest or a pin number (1 - 16)\n',
            b'test_error_set:4:set: error: pins may only be given a value once\n',
            b'test_error_set:5:set: error: at least 2 arguments must be given\n',
            b'test_error_set:6:set: error: at least 2 arguments must be given\n',
            b'test_error_set:7:set: error: a value must be given before any pin numbers\n',
            b'test_error_set:8:set: error: at least 2 arguments must be given\n'
        ])

    def test_valid_delay(self):
        self.make_test_file('test_valid_delay',
            'delay 1', # min deelay
            'delay 65535', # max delay - all bits on
            'delay 32768', # last bit on
            'delay 100')
        self.run_program('test_valid_delay', '-w')
        with open('a.prt', 'rb') as file:
            self.assertEqual(file.read(),
                b'PM1'
                b'\x05\x01\x00'
                b'\x05\xFF\xFF'
                b'\x05\x00\x80'
                b'\x05\x64\x00' # delay 100
                b'END')
        os.remove('a.prt')

    def test_error_delay(self):
        self.make_test_file('test_error_delay',
            'delay 0',
            'delay -10',
            'delay -1',
            'delay 65536', # max delay + 1
            'delay 100 100',
            'delay habla',
            'delay')
        output = self.run_program('test_error_delay', '-w', valid_run=False)
        self.assertListEqual(output, [
            b'test_error_delay:1:delay: error: argument must be between 1 and 65535\n',
            b'test_error_delay:2:delay: error: argument must be between 1 and 65535\n',
            b'test_error_delay:3:delay: error: argument must be between 1 and 65535\n',
            b'test_error_delay:4:delay: error: argument must be between 1 and 65535\n',
            b'test_error_delay:5:delay: error: exactly 1 argument must be given\n',
            b'test_error_delay:6:delay: error: argument must be a number\n',
            b'test_error_delay:7:delay: error: exactly 1 argument must be given\n'
        ])

    def test_valid_vin(self):
        self.make_test_file('test_valid_vin',
            'vin 16',
            'vin 15',
            'vin 14',
            'vin 5',
            'vin 16 15 14 5')
        self.run_program('test_valid_vin', '-w')
        with open('a.prt', 'rb') as file:
            self.assertEqual(file.read(),
                b'PM1'
                b'\x03\x00\x80'
                b'\x03\x00\x40'
                b'\x03\x00\x20'
                b'\x03\x10\x00'
                b'\x03\x10\xE0'
                b'END')
        os.remove('a.prt')

    def test_error_vin(self):
        self.make_test_file('test_error_vin',
            'vin 1',
            'vin 16 1',
            'vin -10',
            'vin -1',
            'vin')
        output = self.run_program('test_error_vin', '-w', valid_run=False)
        self.assertListEqual(output, [
            b"test_error_vin:1:vin: error: the number '1' is not a valid pin number\n",
            b"test_error_vin:2:vin: error: the number '1' is not a valid pin number\n",
            b"test_error_vin:3:vin: error: the number '-10' is not a valid pin number\n",
            b"test_error_vin:4:vin: error: the number '-1' is not a valid pin number\n",
            b'test_error_vin:5:vin: error: at least 1 argument must be given\n'
        ])

    def test_valid_gnd(self):
        self.make_test_file('test_valid_gnd',
            'gnd 8',
            'gnd 12',
            'gnd 8 12')
        self.run_program('test_valid_gnd', '-w')
        with open('a.prt', 'rb') as file:
            self.assertEqual(file.read(),
                b'PM1'
                b'\x04\x80\x00'
                b'\x04\x00\x08'
                b'\x04\x80\x08'
                b'END')
        os.remove('a.prt')

    def test_error_gnd(self):
        self.make_test_file('test_error_gnd',
            'gnd 1',
            'gnd 8 1',
            'gnd -10',
            'gnd -1',
            'gnd')
        output = self.run_program('test_error_gnd', '-w', valid_run=False)
        self.assertListEqual(output, [
            b"test_error_gnd:1:gnd: error: the number '1' is not a valid pin number\n",
            b"test_error_gnd:2:gnd: error: the number '1' is not a valid pin number\n",
            b"test_error_gnd:3:gnd: error: the number '-10' is not a valid pin number\n",
            b"test_error_gnd:4:gnd: error: the number '-1' is not a valid pin number\n",
            b'test_error_gnd:5:gnd: error: at least 1 argument must be given\n'
        ])

def main():
    ut.main()

if __name__ == '__main__':
    main()