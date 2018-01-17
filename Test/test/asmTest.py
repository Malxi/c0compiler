#-*- coding:utf-8 -*-

import subprocess
import re
import os
from TestCase import *
from threading import Timer

'''
py2.7 运行asmTest.py
'''

version = '0/'              #因为语法不同，所以就将\0\作为存放while和if-else类的语法测试吧
#compiler = 'D:/cprojects/c0compiler/build.exe'      #自己的编译器路径
#compiler = 'build.exe'
compiler = 'c0compiler.exe'
asm = 'out.asm'             #编译生成的asm文件
timeout_sec = 2000          #等待mars运行时长

def compile(cp, src):
    #proc = subprocess.Popen('cmd.exe', shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    #控制台输入文件名
    proc = subprocess.Popen('{} {}\n'.format(cp, src).encode(), shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    #参数输入文件名
    out, err = proc.communicate('{}\n'.format(src).encode())
    proc.wait()
    return str(out)

def runAsm(src, test_in):
    proc = subprocess.Popen('java -jar Mars4_5.jar {}'.format(src), shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    kill_proc = lambda p: p.kill()
    timer = Timer(timeout_sec, kill_proc, [proc])
    try:
        timer.start()
        out, err = proc.communicate(test_in)
    finally:
        timer.cancel()
    #proc.wait()
    return str(out[67:].replace('\r', ''))

def runTest(test):
    r = compile(compiler, version + test['src'])
    if not os.path.exists(asm):
        print(r)
    for i in range(len(test['case'])):
        r = runAsm(asm, test['case'][i])
        '''
        print len(r)
        print len(test['ans'][i])
        for j in range(len(r)):
            print ord(r[j])
        '''
        if r == test['ans'][i]:
            print('{} test case {} passed!'.format(test['src'], i))
        else:
            print('{} test case {} faild!'.format(test['src'], i))
            print('res : ' + r)
            print('except : ' + test['ans'][i])

if __name__ == '__main__':
    testList = []
    if version == '0/':
        testList = testList_0
    for item in testList:
        runTest(item)