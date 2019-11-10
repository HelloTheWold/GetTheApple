import serial
import time
import cv2
import numpy as np
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BOARD)
GPIO.setup(12,GPIO.IN)
GPIO.setup(12,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)

ser = serial.Serial('/dev/ttyAMA0',9600,timeout=1)
ser2 = serial.Serial('/dev/ttyAMA0',115200,timeout=1)

def send2serial(str):
    ser.write(str.encode())#move forward 100mm
    # 获得接收缓冲区字符
    response = ser.read(4)
    while response != "OVER".encode():
        response = ser.read(4)
        #pass
    print("response:"+response)
    

def get_gy25_yaw():
    ser2.write(b"\xA5")
    ser2.write(b"\x51")
    response = ser2.read(8)
    yaw = 0
    
    if response[0:1] == b"\xaa" and response[7:8] == b"\x55":
        high_yaw = response[1:2]
        low_yaw = response[2:3]  
        
        high_yaw = int.from_bytes(high_yaw,byteorder="big",signed=True)#ord(high_yaw)
        low_yaw = int.from_bytes(low_yaw,byteorder="big",signed=False)
        
        if(high_yaw>0):
            yaw = high_yaw*256 + low_yaw
        else:
            yaw = high_yaw*256 - low_yaw
        yaw = yaw/100 + 180
    #print (yaw)
    return yaw

def get_gy25_yaw_stable():
    yaw1 = get_gy25_yaw()
    time.sleep(0.01)
    yaw2 = get_gy25_yaw()
    time.sleep(0.01)
    yaw3 = get_gy25_yaw()
    yaw = (yaw1+yaw2+yaw3)/3
    #print (yaw)
    return yaw


def test_yawdiff_rotate(rotate_value):#positive value : clockwise
    ser2.write(b"\xA5")
    ser2.write(b"\x55")
    
    if rotate_value >= 100:
        send_str = "rc0"+str(rotate_value)+"Z"
    elif rotate_value <= -100:
        send_str = "ra0"+str(rotate_value)+"Z"
    elif rotate_value >= 10:
        send_str = "rc00"+str(rotate_value)+"Z"
    elif rotate_value <= -10:
        send_str = "ra00"+str(rotate_value)+"Z"
    elif rotate_value >= 0:
        send_str = "rc000"+str(rotate_value)+"Z"
    elif rotate_value < 0:
        send_str = "ra000"+str(rotate_value)+"Z"
    
    print(send_str)
    send2serial(send_str)
    
    yaw_current = get_gy25_yaw_stable()
    yaw_diff = yaw_current - 180
    
    return yaw_diff






camera=cv2.VideoCapture(0)
def cam_detection_lines_hsv(color):
    for i in range(10):
        ret,frame=camera.read()
    while not ret: #记得要对视频是否处理完成做判断
        ret,frame=camera.read()
        
    hsv_img = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    #set H S V threshold
    if color == "yellow":
        #yellow parameter  
        iLowH = 10
        iHighH = 55
        iLowS = 40
        iHighS = 255
        iLowV = 20
        iHighV = 255
    elif color == "red":
        #red parameter
        iLowH = 160
        iHighH = 179
        iLowS = 90
        iHighS = 255
        iLowV = 90
        iHighV = 255
    elif color == "black":
        #black parameter
        iLowH = 0
        iHighH = 180
        iLowS = 0
        iHighS = 255
        iLowV = 0
        iHighV = 38
    elif color == "truck":
        #truck parameter
        iLowH = 2
        iHighH = 23
        iLowS = 70
        iHighS = 165
        iLowV = 90
        iHighV = 255
    
    
    
    list_line1 = []
    list_line2 = []
    #对彩色图像做直方图均衡化
    #split hsv_img into 3 channels
    hsvSplit = cv2.split(hsv_img)
    #Hist equalization
    cv2.equalizeHist(hsvSplit[2],hsvSplit[2])
    #merge hsv_img using the result of hist equalization
    cv2.merge(hsvSplit,hsv_img)
    
    #using OpenCV API to detect special color we interest
    #detect each pixel in hsv_img
    #if current pixel HSV value is between the theshold we set before,then set this pixel 255 in the output image
    #else,set this pixel 0 in the output image
    imgThresholded = cv2.inRange(hsv_img, (iLowH, iLowS, iLowV), (iHighH, iHighS, iHighV))
    
    #perform open opration for imgThresholded, in order to drop some noisy pixels 
    element = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))
    imgThresholded = cv2.morphologyEx(imgThresholded, cv2.MORPH_OPEN, element)
    #then perform close opration to connect some separated white small piece
    imgThresholded = cv2.morphologyEx(imgThresholded, cv2.MORPH_CLOSE, element)
                                      
    #get a line along row direction
    #line2 = imgThresholded[240,:]
    #print(line2.shape[0])#640
    line1 = imgThresholded[:,220]
    line2 = imgThresholded[:,420]
    
    
    for index in range(line1.shape[0]):
        if line1[index]==255:
            list_line1.append(index)
    for index in range(line2.shape[0]):
        if line2[index]==255:
            list_line2.append(index)
    
    if len(list_line1)==0 or len(list_line2)==0:
        #print("no pixel in two lines")
        return imgThresholded,-100,-100
    
    #计算树干列表的均值
    sum_index_line1=0
    sum_index_line2=0
    
    for index in list_line1:
        sum_index_line1 = sum_index_line1+index
    for index in list_line2:
        sum_index_line2 = sum_index_line2+index
    
    line1_mean=sum_index_line1/len(list_line1)
    line2_mean=sum_index_line2/len(list_line2)
    
    return imgThresholded,round(line1_mean),round(line2_mean)



def adjust(color):
    imgThresholded,pixel1,pixel2 = cam_detection_lines_hsv(color)
    diff = pixel2 - pixel1
    print (pixel1,pixel2,diff)
    while (pixel1==-100 or pixel2==-100):
        imgThresholded,pixel1,pixel2 = cam_detection_lines_hsv(color)
        diff = pixel2 - pixel1
        print (pixel1,pixel2,diff)

    while not (diff>-2 and diff < 2):

        if diff >=100 or diff < -100:
            break
        if diff > 0 :
            value = round(diff*0.15)
            if value==0:
                value = 1
                
            if value<10:
                str_correction = "rc000"+str(value)+"Z"
            else:
                str_correction = "rc00"+str(value)+"Z"
            print(str_correction)
            send2serial(str_correction)
            
        else:
            value = round(-diff*0.15);
            if value==0:
                value = 1
                
            if value<10:
                str_correction = "ra000"+str(value)+"Z"
            else:
                str_correction = "ra00"+str(value)+"Z"
            print(str_correction)
            send2serial(str_correction)
        
        time.sleep(0.8)
        imgThresholded,pixel1,pixel2 = cam_detection_lines_hsv(color)
        diff = pixel2 - pixel1
        print (pixel1,pixel2,diff)
        while (pixel1==-100 or pixel2==-100):
            imgThresholded,pixel1,pixel2 = cam_detection_lines_hsv(color)
            diff = pixel2 - pixel1
            print (pixel1,pixel2,diff)
            time.sleep(0.5)


        if pixel1!=-100:
            print("pixel1:"+str(pixel1)+" pixel2:"+str(pixel2)+" diff_pixel:"+str(pixel2-pixel1))
        #cv2.circle(imgThresholded,(220,pixel1),10,(255,255,255))
        #cv2.circle(imgThresholded,(420,pixel2),10,(255,255,255))
        #cv2.imshow("threshold_img",imgThresholded)
        #cv2.waitKey(10)

def test_pixeldiff_yawdiff():
    ser2.write(b"\xA5")
    ser2.write(b"\x55")
    
    for i in range(15):
        imgThresholded,pixel1,pixel2 = cam_detection_lines_hsv("yellow")
    pixel_diff = pixel2-pixel1
    
    print("ok")
    while(GPIO.input(12)==GPIO.LOW):
        time.sleep(0.2)
    
    yaw_current = get_gy25_yaw_stable()
    yaw_diff = yaw_current - 180.0
    return pixel_diff,yaw_diff



    
   
   
#test1
#diff_yaw = test_yawdiff_rotate(30)#positive value : clockwise
#print (diff_yaw)

#test2
pixel_diff,yaw_diff = test_pixeldiff_yawdiff()
print ("pixel_diff:"+str(pixel_diff)+",yaw_diff:"+str(yaw_diff))

