from telnetlib import Telnet
import sys

if (len(sys.argv) != 3):
    print("error: No port secified. Try \"python3 main.py <PORT> <File REQUEST>\".")
    exit (1)

ip="localhost"
t = Telnet()
t.open(ip, int(sys.argv[1]))

with open(sys.argv[2], 'r') as f:
    request = f.read().replace('\n', '\r\n')

t.write(bytes(request, 'ascii'))
response = t.read_until(b'</html>\r\n', 10) # need to replace this 
print(response.decode())

t.close()
