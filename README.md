# CameraMove_detect
Based on the phase metric, including cropping, splicing, and calculating the phase shift scale for the four corners of the image, and calculating the moving direction angle (基于相位度量 ，包括图像四角裁剪、拼接、计算相位移尺度及方向角)

该算法主要包括以下几个步骤：
1、将当前视频的每一帧提取出人员运动相对较少的四个区域后进行重新拼接
2、对拼接后生成的新图像计算当前相位相关值以及运动的角度
3、通过图像处理的方法将运动角度提取出来
4、设定相机移动的判定阈值进行判断。

如果觉得对您有点帮助，希望您给我一颗星，谢谢

The algorithm mainly includes the following steps:
1. Re-splicing each frame of the current video out of four areas with relatively few people's motions
2. Calculate the current phase correlation value and the angle of motion for the new image generated after splicing
3. Extract the motion angle by image processing
4. Determine the threshold for camera movement to determine.

If you feel helpful to you, help me give me a star, thank you
