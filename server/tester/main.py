from telnetlib import Telnet
import sys

error = False
if (len(sys.argv) != 3 and len(sys.argv) != 5):
    error = True
elif (len(sys.argv) == 5 and sys.argv[3] != "-nginx"):
    error = True
if (error):
    print("error: Port Not secified.")
    print("try \" python3 main.py <PORT> <File REQUEST> [OPT] -nginx <PORT>\".")
    exit (1)

def test_webserv(port, request):
    ip="localhost"
    t = Telnet()
    t.open(ip, int(port))

    with open(sys.argv[2], 'r') as f:
        request = f.read().replace('\n', '\r\n')

    t.write(bytes(request, 'ascii'))
    response = t.read_until(b'</html>\r\n', 10) # need to replace this 
    return response.decode()

    t.close()

def test_nginx(port):
    ip="localhost"
    t = Telnet()
    t.open(ip, int(port))

    with open(sys.argv[2], 'r') as f:
        request = f.read().replace('\n', '\r\n')

    t.write(bytes(request, 'ascii'))
    response = t.read_until(b'</html>\r\n', 10) # need to replace this 
    return response.decode()

    t.close()

print("./webserv ------------------------------------------------------------")
r_webserv = test_webserv(sys.argv[1], sys.argv[2])
print(r_webserv)
print("./nginx ------------------------------------------------------------")
r_nginx = test_nginx(sys.argv[4])
print(r_nginx)
