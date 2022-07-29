from maix import camera, display, image
import serial

# 调整参数 #
PixelsThreshold=100 #色块像素阈值
Line_threshold=(80, 100, -128, 127, -128, 127)   #黑线阈值
A_port_width=60 #A点色块宽度阈值
Wait_port_width=160  #等停指示色块宽度阈值

UpROI=(0,90,240,20)  #向上识别ROI
DefaultROI=(0,120,240,20)  #默认识别ROI

# 变量  #
line_blobs_num=0 #色块数量
up_line_blobs_num=0 #上方色块数量
run_status=0    #路径状态

# 初始化 #
ser = serial.Serial("/dev/ttyS1",115200)    # 连接串口
camera.camera.config(size=(240, 240))



while(True):
    img = camera.capture()

    #巡线
    LineBlobs = img.find_blobs([Line_threshold],roi=DefaultROI,x_stride = 20, pixels_threshold=PixelsThreshold,merge= True)
    if LineBlobs:
        line_blobs_num=0
        for lb in LineBlobs:
            line_blobs_num+=1           
            img.draw_rectangle(lb["x"],lb["y"],lb["x"]+lb["w"],lb["y"]+lb["h"],color=(0, 0, 255), thickness=1)
 

        if line_blobs_num==1:    ##非岔路口
            if lb["w"]>Wait_port_width: ###等停点
                print("Waiting Port")
                img.draw_string(10, 10,"Waiting Port", scale = 1.0, color = (255, 255, 255),thickness = 1)
            elif lb["w"]>A_port_width:   ###A点或岔路过渡
                UpBlobs = img.find_blobs([Line_threshold],roi=UpROI,x_stride = 20,pixels_threshold=PixelsThreshold,merge= True)
                if UpBlobs:
                        up_line_blobs_num=0
                        for ub in UpBlobs:
                            up_line_blobs_num+=1
                            img.draw_rectangle(ub["x"],ub["y"],ub["x"]+ub["w"],ub["y"]+ub["h"],color=(0, 0, 255), thickness=1)
                        if up_line_blobs_num==1:    ####A点
                            print("A Port")
                            img.draw_string(10, 10,"A Port", scale = 1.0, color = (255, 255, 255),thickness = 1)
                        elif up_line_blobs_num==2:    ####岔路过渡
                            img.draw_string(10, 10,"Y Cross Transition", scale = 1.0, color = (255, 255, 255),thickness = 1)
                            print("Y Cross Transition")
                else:
                    print("Normal2")
                    img.draw_string(10, 10,"Normal2", scale = 1.0, color = (255, 255, 255),thickness = 1)
                    data = 'L' + str(int(lb["x"]+0.5*lb["w"])) + '\n'
                    ser.write(data.encode())
                    img.draw_string(10, 100,str(int(lb["x"]+0.5*lb["w"])), scale = 1.0, color = (255, 255, 255),thickness = 1)


            else:   ###非特殊点
                print("Normal")
                img.draw_string(10, 10,"Normal", scale = 1.0, color = (255, 255, 255),thickness = 1)
                data = 'L' + str(int(lb["x"]+0.5*lb["w"])) + '\n'
                ser.write(data.encode())
                img.draw_string(10, 100,str(int(lb["x"]+0.5*lb["w"])), scale = 1.0, color = (255, 255, 255),thickness = 1)

        elif line_blobs_num==2:  ##岔路口
            img.draw_string(10, 10,"Y Cross", scale = 1.0, color = (255, 255, 255),thickness = 1)
            print("Y Cross")


    else:
        line_blobs_num=0
        
    display.show(img)
