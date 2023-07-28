import cv2
import os
import numpy as np

def resize_image(image, height, width):
    return cv2.resize(image, (width, height), interpolation=cv2.INTER_NEAREST)

def binary_image(image, threshold=127):
    alpha_channel = None
    _, _, channel = image.shape
    if (channel == 4): # png
        _, alpha_channel = cv2.threshold(image[:, :, 3], threshold, 255, cv2.THRESH_BINARY)
        alpha_channel = cv2.bitwise_not(alpha_channel)
    
    gray_image = 0.2989 * image[:, :, 2] + 0.5870 * image[:, :, 1] + 0.1140 * image[:, :, 0]
    _, binarized_image = cv2.threshold(gray_image, threshold, 255, cv2.THRESH_BINARY)
    binarized_image = binarized_image.astype(np.uint8)

    if alpha_channel is not None:
        bor = cv2.bitwise_or(binarized_image, alpha_channel)
        return bor
    else:
        return binarized_image

# 修改文件名
def modify_filename(file_path:str, file_str:str, append=False):
    # 获取文件目录
    directory = os.path.dirname(file_path)

    # 获取文件名和扩展名
    filename, file_extension = os.path.splitext(os.path.basename(file_path))
    
    # 新的文件名
    new_filename = ""
    if append:
        new_filename = filename + "_" + file_str + file_extension
    else:
        new_filename = file_str + file_extension

    # 构建新的文件路径
    new_file_path = os.path.join(directory, new_filename)

    return new_file_path

# 显示图像
def img_show(image, title="image"):
    cv2.imshow(title, image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

# 遍历文件夹
def traverse_folder(folder_path):
    file_paths = []
    for root, directories, files in os.walk(folder_path):
        for file_name in files:
            file_path = os.path.join(root, file_name)
            file_paths.append(file_path)
    
    return file_paths