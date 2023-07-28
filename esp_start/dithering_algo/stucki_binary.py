import cv2
import numpy as np
from utils import resize_image, modify_filename

def stucki_thresholding(image):
    # 将图像转换为灰度图像
    # gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    gray = 0.2989 * image[:, :, 2] + 0.5870 * image[:, :, 1] + 0.1140 * image[:, :, 0]

    height, width = gray.shape

    # 创建一个与原始图像大小相同的空白图像用于存储二值化结果
    thresholded = np.zeros_like(gray, dtype=np.uint8)

    for y in range(height):
        for x in range(width):
            old_pixel = gray[y, x]
            new_pixel = 255 if old_pixel >= 128 else 0
            thresholded[y, x] = new_pixel

            error = old_pixel - new_pixel

            if x < width - 1:
                # Error diffusion to right neighbor
                gray[y, x + 1] += error * 8 / 42
            if x < width - 2:
                # Error diffusion to right-next neighbor
                gray[y, x + 2] += error * 4 / 42
            if y < height - 1:
                if x > 0:
                    # Error diffusion to bottom-left neighbor
                    gray[y + 1, x - 1] += error * 2 / 42
                # Error diffusion to bottom neighbor
                gray[y + 1, x] += error * 8 / 42
                if x < width - 1:
                    # Error diffusion to bottom-right neighbor
                    gray[y + 1, x + 1] += error * 4 / 42
                if x < width - 2:
                    # Error diffusion to bottom-next neighbor
                    gray[y + 1, x + 2] += error * 2 / 42

    return thresholded
   

# 读取图像文件
img_path = "hei6.png"
image = cv2.imread(img_path)

image = resize_image(image, 200)

# 运行 Stucki 抖动算法进行图像二值化
thresholded_image = stucki_thresholding(image)

cv2.imwrite(modify_filename(img_path, "stucki_binary", True), thresholded_image)

#显示原始图像和二值化图像
# cv2.imshow("Original Image", image)
# cv2.imshow("Thresholded Image", thresholded_image)
# cv2.waitKey(0)
# cv2.destroyAllWindows()
