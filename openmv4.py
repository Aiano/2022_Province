import sensor, image, time, gc
from pyb import UART

# 调整参数 #
PixelsThreshold=300 #色块像素阈值
Margin=1   #合并
Line_threshold=(0,40)   #黑线灰度阈值
A_port_width=100 #A点色块宽度阈值
Wait_port_width=200  #等停指示色块宽度阈值

UpROI=(0,60,320,20)  #向上识别ROI
DefaultROI=(0,120,320,20)  #默认识别ROI

# 变量  #
line_blobs_num=0 #色块数量
up_line_blobs_num=0 #上方色块数量
run_status=0    #路径状态

# 初始化 #
sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QVGA)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
sensor.set_vflip(True)
sensor.set_hmirror(True)
sensor.skip_frames(20)

uart = UART(3, 115200)
uart.init(115200, bits=8, parity=None, stop=1)

clock = time.clock()
while(True):
    clock.tick()
    img = sensor.snapshot()#.invert()

    #巡线
    LineBlobs = img.find_blobs([Line_threshold],roi=DefaultROI,pixels_threshold=PixelsThreshold,merge= True,margin=Margin)
    if LineBlobs:
        line_blobs_num=0
        for lb in LineBlobs:
            line_blobs_num+=1
            img.draw_rectangle(lb[0:4])
            img.draw_cross(lb[5], lb[6])

        if line_blobs_num==1:    ##非岔路口
            if lb.w()>Wait_port_width: ###等停点
                print("Waiting Port")
            elif lb.w()>A_port_width:   ###A点或岔路过渡
                UpBlobs = img.find_blobs([Line_threshold],roi=UpROI,pixels_threshold=PixelsThreshold,merge= True,margin=Margin)
                if UpBlobs:
                        up_line_blobs_num=0
                        for ub in UpBlobs:
                            up_line_blobs_num+=1
                            img.draw_rectangle(ub[0:4])
                            img.draw_cross(ub[5], ub[6])
                        if up_line_blobs_num==1:    ####A点
                            print("A Port")
                        elif up_line_blobs_num==2:    ####岔路过渡
                            print("Y Cross Transition")
                else:
                    print("Normal" + 'L'+ str(lb.cx()))
                    uart.write('L'+ str(lb.cx()) + '\n')


            else:   ###非特殊点
                print("Normal" + 'L'+ str(lb.cx()))
                uart.write('L'+ str(lb.cx()) + '\n')

        elif line_blobs_num==2:  ##岔路口
            # uart.write('X'+'\n')
            print("Y Cross")


    else:
        line_blobs_num=0

    #print(str(clock.fps())+','+str(line_blobs_num)+','+str(weizhi))



    gc.collect()
