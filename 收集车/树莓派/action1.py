#import serial moudle
import serial
import time
import cv2
import numpy as np
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BOARD)
GPIO.setup(12,GPIO.IN)
GPIO.setup(12,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)
#open named port at 9600,1s timeout  ttyAMA0
ser = serial.Serial('/dev/ttyUSB0',9600,timeout=1)
ser2 = serial.Serial('/dev/ttyAMA0',115200,timeout=1)


def send2serial(str):
    ser.write(str.encode())#move forward 100mm
    # 获得接收缓冲区字符
    '''
    while ser.inWaiting()==0:
        pass
        #print("sd")
        #time.sleep(0.001)#0.05s=50ms
    '''
    response = ser.read(4)
    while response != "OVER".encode():
        response = ser.read(4)
        #pass
    
    print(response)
    
    #print(str.encode())

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
    yaw = round((yaw1+yaw2+yaw3)/3)
    #print (yaw)
    return yaw
#action_squence = ["ms0000"]    
#action_squence = ["ms0000"]





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
    #print(line1.shape[0])
    
    #
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
            value = round(diff*0.379+0.3416)
            if value==0:
                value = 1
                
            if value<10:
                str_correction = "rc000"+str(value)+"Z"
            else:
                str_correction = "rc00"+str(value)+"Z"
            print(str_correction)
            send2serial(str_correction)
            
        else:
            value = round(-diff*0.379+0.3416)
            if value==0:
                value = 1
                
            if value<10:
                str_correction = "ra000"+str(value)+"Z"
            else:
                str_correction = "ra00"+str(value)+"Z"
            print(str_correction)
            send2serial(str_correction)
        
        #time.sleep(0.8)
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




def action_entrance():
    
    action_squence = ["ml0020Z","mf0120Z","release_box","ml0480Z","mf0240Z","rt0000Z","adjust_yellow",\
                      "mb0300Z","mr0550Z","mf0320Z","rt0000Z",\
                      "mb0300Z","mr0550Z","mf0320Z","rt0000Z","adjust_yellow",\
                      "mb0260Z","mr0550Z","mf0280Z","rt0000Z",\
                      "mb0270Z","ml0550Z","mf0290Z","rt0000Z","adjust_yellow",\
                      "mb0260Z","mr0550Z","mf0280Z","rt0000Z",\
                      "mb0300Z","mr0510Z","ml0020Z","mf1300Z"
                     ]
    
    #action_squence = ["ml0500Z"]#,"mf1400Z","mb0800jZ"]#"rt0000Z"]
    for action in action_squence:
        time.sleep(0.2)
        if GPIO.input(12)==GPIO.LOW:
            break
        '''
        if action == "rt0000Z":
            ser2.write(b"\xA5")
            ser2.write(b"\x55")
            yaw_start = get_gy25_yaw_stable()
            print ("yaw_start:")
            print (yaw_start)
            send2serial(action)
            yaw_end = get_gy25_yaw_stable()
            print ("yaw_end:")
            print(yaw_end)
            #while(yaw_end<240 or yaw_end>300):
            #    yaw_end = get_gy25_yaw()
    
            while(not(yaw_end>268 and yaw_end<272)):
                if(yaw_end<270):
                    str_correction = "ra00"+str(270-round(yaw_end))+"Z"
                    send2serial(str_correction)
                else:
                    str_correction = "rc00"+str(round(yaw_end)-270)+"Z"
                    send2serial(str_correction)
                yaw_end = get_gy25_yaw_stable()
                        
                #while(yaw_end<240 or yaw_end>300):
                #    yaw_end = get_gy25_yaw()
        '''
        if(action=="adjust_yellow"):
            adjust("yellow")
        elif(action=="release_box"):
            send2serial("ru0000Z")
        elif(action=="adjust_red"):
            adjust("red");
        elif(action=="adjust_black"):
            adjust("black")
        
        else:    
            send2serial(action)
        
        #send2serial(action)
 

action_done_flag = False
while(True):
    if(GPIO.input(12)==GPIO.LOW):
        if action_done_flag == True:
            send2serial("rd0000Z")
            action_done_flag = False
        time.sleep(0.2)
    else:
        #adjust("yellow")
        action_entrance()
        action_done_flag = True
        while(GPIO.input(12)== GPIO.HIGH):
            time.sleep(0.2)
            print ("I am sleeping...")
     



'''
ser2.write(b"\xA5")
ser2.write(b"\x55")
while True:
    yaw = get_gy25_yaw_stable()
    print (yaw)
'''