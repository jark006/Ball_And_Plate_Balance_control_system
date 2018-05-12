import sensor, image, time,math,pyb,json
from pyb import Servo
from pyb import UART

#获取黑球坐标，输出串口(baudRate:115200)
#黑球色域
Block_threshold   = (5,45,-20,35,-20,20)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(10) 
sensor.set_auto_whitebal(False)
uart = UART(3, 115200)
uart.init(115200,bits=8,parity=None,stop=1)

while(True):
    img = sensor.snapshot().lens_corr(1.0 ,1.0)
    blobs = img.find_blobs([Block_threshold])
    if blobs:
        for b in blobs:
            if (b[5]>5) and (b[5]<125) and (b[6]>3) and (b[6]<115):
                if (b[2]<15) and (b[3]<15):
                    img.draw_rectangle(b[0:4])
                    uart_buf = bytearray([0xef,0x0d,int(b[5])>>8,int(b[5]),int(b[6])>>8,int(b[6]),0xfe])
                    uart.write(uart_buf)