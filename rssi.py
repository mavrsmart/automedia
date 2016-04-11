#!/usr/bin/python
import binascii
import sys
f = open("/srv/modem/tty3g1", "r+")
data=''
error="Usage: python ussd.py action (code)\r\nActions: balans, popolnit, data-status, 3g-data-status, signal"
if len(sys.argv) < 2:
    print error
    sys.exit()
 
if sys.argv[1] == 'balans':
    print>>f, "AT+CUSD=1,*111#,15\r\n"
    while data[:5]!="+CUSD":
        data=f.readline()
    data = data[10:-6]
    print binascii.unhexlify(data)
elif sys.argv[1] == 'signal':
    print>>f, "AT+CSQ\r\n"
    i=0
    while data[:5]!="+CSQ:":
        i=i+1
        if(i>4):sys.exit()
#        print i
        data=f.readline()
    data = data[6:-5]
    sig_str = -113+int(data)*2
    sig_per = int(data)*100 / 31
#print unicode(sig_str)+"dBm / "+unicode(sig_per)+"%"
    print unicode(sig_per)+"%"
elif sys.argv[1] == 'popolnit':
    print>>f, "AT+CUSD=1,*123*"+sys.argv[2]+"#,15\r\n"
    while data[:5]!="+CUSD":
        data=f.readline()
    data = data[10:-6]
    print binascii.unhexlify(data)
elif sys.argv[1] == 'data-status':
    print>>f, "AT+CUSD=1,*121#,15\r\n"
    while data[:5]!="+CUSD":
        data=f.readline()
    data = data[10:-6]
    print binascii.unhexlify(data)
elif sys.argv[1] == '3g-data-status':
    print>>f, "AT+CUSD=1,*122#,15\r\n"
    while data[:5]!="+CUSD":
        data=f.readline()
    data = data[10:-6]
    print binascii.unhexlify(data)
else:
    print error
f.close
