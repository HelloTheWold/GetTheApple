# 色块监测 例子
#
# 这个例子展示了如何通过find_blobs()函数来查找图像中的色块
# 这个例子查找的颜色是深绿色

import sensor, image, time, pyb
from pyb import Pin
from pyb import UART
from array import array

count = array('i',[0,0,0,0,0])

uart = UART(3, 9600)
uart.init(9600, bits=8, parity=None, stop=1)

out0 = Pin('P7', Pin.OUT_PP)#设置p_out为输出引脚 green
out1 = Pin('P8', Pin.OUT_PP)#设置p_out为输出引脚 red
# 颜色追踪的例子，一定要控制环境的光，保持光线是稳定的。
red_threshold = (60, 75, 42, 58, -7, 61)
green_threshold = (53, 100, -39, -26, -23, 6)
#设置绿色的阈值，括号里面的数值分别是L A B 的最大值和最小值（minL, maxL, minA,
# maxA, minB, maxB），LAB的值在图像左侧三个坐标图中选取。如果是灰度图，则只需
#设置（min, max）两个数字即可。
led_r=pyb.LED(1)
led_l=pyb.LED(2)
led_b=pyb.LED(3)
sensor.reset() # 初始化摄像头
sensor.set_pixformat(sensor.RGB565) # 格式为 RGB565.
sensor.set_framesize(sensor.QQVGA) # 使用 QQVGA 速度快一些
sensor.skip_frames(time = 2000) # 跳过2000s，使新设置生效,并自动调节白平衡
sensor.set_auto_gain(False) # 关闭自动自动增益。默认开启的，在颜色识别中，一定要关闭白平衡。
sensor.set_auto_whitebal(False)
#关闭白平衡。白平衡是默认开启的，在颜色识别中，一定要关闭白平衡。
clock = time.clock() # 追踪帧率
while(True):
    count_red=0
    count_green=0
    clock.tick() # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot() # 从感光芯片获得一张图像
    area=(30,30,100,55)
    img.draw_rectangle(area, color = (255, 255, 255))
    blobs1 = img.find_blobs([red_threshold],area_threshold=1500,roi=area)
    blobs2 = img.find_blobs([green_threshold],area_threshold=1500,roi=area)
    #blobs1 = img.find_blobs([red_threshold],area_threshold=200)
    #blobs2 = img.find_blobs([green_threshold],area_threshold=200)
    #find_blobs(thresholds, invert=False, roi=Auto),thresholds为颜色阈值，
    #是一个元组，需要用括号［ ］括起来。invert=1,反转颜色阈值，invert=False默认
    #不反转。roi设置颜色识别的视野区域，roi是一个元组， roi = (x, y, w, h)，代表
    #从左上顶点(x,y)开始的宽为w高为h的矩形区域，roi不设置的话默认为整个图像视野。
    #这个函数返回一个列表，[0]代表识别到的目标颜色区域左上顶点的x坐标，［1］代表
    #左上顶点y坐标，［2］代表目标区域的宽，［3］代表目标区域的高，［4］代表目标
    #区域像素点的个数，［5］代表目标区域的中心点x坐标，［6］代表目标区域中心点y坐标，
    #［7］代表目标颜色区域的旋转角度（是弧度值，浮点型，列表其他元素是整型），
    #［8］代表与此目标区域交叉的目标个数，［9］代表颜色的编号（它可以用来分辨这个
    #区域是用哪个颜色阈值threshold识别出来的）。
    if blobs1:
    #如果找到了目标颜色
        for br in blobs1:
            if br[8]:
                count_red=br[8]
                led_r.on()
                time.sleep(1)
                led_r.off()
                #迭代找到的目标颜色区域红色
                # Draw a rect around the blob.
                img.draw_rectangle(br[0:4]) # rect
                #用矩形标记出目标颜色区域
                #print(b[0:4])
                img.draw_cross(br[5], br[6]) # cx, cy
                #在目标颜色区域的中心画十字形标记
                #print(b[5], b[6])
    if blobs2:
        for bg in blobs2:
            if bg[8]:
                count_green=bg[8]
                led_l.on()
                time.sleep(1)
                led_l.off()
                #迭代找到的目标颜色区域绿色
                # Draw a rect around the blob.
                img.draw_circle(bg[5], bg[6],int(bg[3]/2)) # rect
                #用矩形标记出目标颜色区域
                img.draw_cross(bg[5], bg[6]) # cx, cy
                #在目标颜色区域的中心画十字形标记
    #print(clock.fps()) # 注意: 你的OpenMV连到电脑后帧率大概为原来的一半
    #如果断开电脑，帧率会增加
    if count_green==0 and count_red==0:
        #out0.low()
        #out1.low()
        #print('没红没绿')
        res=0
    elif count_green>0 and count_red>0:
        #out0.high()
        #out1.high()
        #print('both')
        #uart.write("both" +'\n')
        res=3
    elif count_green==0 and count_red>0:
        #out0.low()
        #out1.high()
        #print('红')
        #uart.write("red" +'\n')
        res=1
    elif count_green>0 and count_red==0:
        #out0.high()
        #out1.low()
        #print('绿')
        #uart.write("green" +'\n')
        res=2
    count[2]=count[3]
    count[1]=count[2]
    count[0]=count[1]
    count[3]=res

    #print(count[0],count[1],count[2],count[3])
    if count[1]==res and count[2]==res and count[0]==res:
        if res==1:
            uart.write("red")
            print('red')
        elif res==2:
            uart.write("green")
            print('green')
        elif res==3:
            uart.write("both")
            print('both')
        else:
            print('00')
