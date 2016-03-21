import sys
import serial
import time

framebuffer = [0]*180
step = 0

def main(argv):
    global framebuffer
    if len(argv) < 1:
        print("needed serial device argument")
        sys.exit(1)

    sp = serial.Serial(argv[0], 115200)

    sp.write(b's')
    while True:
        calcfb()
        sp.write(b'f')
        sp.write(bytes(framebuffer))
        time.sleep(0.1)

def calcfb():
    global step
    global framebuffer
    if step==0:
        step = 1
        framebuffer = [0]*180
    elif step==1:
        step = 0
        framebuffer = [255]*180
#    framebuffer[step*3] = 64
#    step = step +1
    if step == 60:
        step = 0

    
if __name__ == "__main__":
    main(sys.argv[1:])
