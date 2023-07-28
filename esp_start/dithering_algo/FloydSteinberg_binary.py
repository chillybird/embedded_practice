import cv2
import numpy as np
from utils import resize_image, modify_filename


def dithering(image, coefficients):
    height, width = image.shape[:2]
    output_image = np.zeros((height, width), dtype=np.uint8)

    # 从coefficients参数中获取系数值
    c1, c2, c3, c4 = coefficients

    for y in range(height):
        for x in range(width):
            old_pixel = image[y, x]
            new_pixel = 255 if old_pixel >= 128 else 0
            output_image[y, x] = new_pixel
            error = old_pixel - new_pixel
            
            if x < width - 1 and y < height - 1:
                image[y, x + 1] += int(error * c1) # right
                if x - 1 >= 0:
                    image[y + 1, x - 1] += int(error * c2) # bottom left
                image[y + 1, x] += int(error * c3)  # bottom
                image[y + 1, x + 1] += int(error * c4) # bottom right
            
            elif x == width - 1 and y < height - 1:
                image[y + 1, x] += int(error * c3)  # bottom
            elif x < width - 1 and y == height - 1:
                image[y, x + 1] += int(error * c1) # right

    return output_image

# 定义系数值
"""
0  0    0    0
0  0    *   7/16
0 3/16 5/16 1/16
"""
coefficients = (7/16, 3/16, 5/16, 1/16)
# coefficients = (3/16, 0, 3/16, 1/4)
# coefficients = (3/8, 0, 3/8, 1/4)
# coefficients = (3/8, 0, 3/8, 1/4)

# 读取图像文件
img_path = "imgs/hei6.png"
image = cv2.imread(img_path)
image = resize_image(image, 200, 200)

image = 0.2989 * image[:, :, 2] + 0.5870 * image[:, :, 1] + 0.1140 * image[:, :, 0]

# 调用抖动算法函数，并传递系数值
dithered_image = dithering(image, coefficients)
cv2.imwrite(modify_filename(img_path, "fs_binary", True), dithered_image)

