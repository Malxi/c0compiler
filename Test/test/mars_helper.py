#-*- coding: utf-8 -*-
#!/usr/bin/python
import os

mars = "Mars4_5.jar"
filename = "mips.asm"

for len in range(10000000, 20000000, 1000000):
    out = os.popen("java -jar " + mars + " " + str(len) + " " + filename).read()
    if "terminated" in out:
    	print(u"步数大于 " + str(len))
    else:
    	print(u"步数小于 " + str(len))
    	break
    
