import cv2
from utils import resize_image, modify_filename, binary_image


# 二值化
def binary_nums(num_size=30, colon_width=15):
    for i in range(10):
        img_path = f"nums/num{i}.png"
        print(img_path)
        image = cv2.imread(img_path, cv2.IMREAD_UNCHANGED)
        image = resize_image(image, num_size, num_size)
        binarized_image = binary_image(image)
        cv2.imwrite(modify_filename(img_path, "bin", True), binarized_image)

    img_path = f"nums/colon.png"
    image = cv2.imread(img_path, cv2.IMREAD_UNCHANGED)
    image = resize_image(image, num_size, colon_width)
    binarized_image = binary_image(image)
    cv2.imwrite(modify_filename(img_path, "bin", True), binarized_image)

def crop_image(image, x, y, width, height):
    # 裁剪图片的坐标原点为左上角

    # 获取图像的高度和宽度
    image_height, image_width = image.shape[:2]
    
    # 确保裁剪区域在图像范围内
    if x < 0 or y < 0 or x + width > image_width or y + height > image_height:
        print("裁剪区域超出图像范围")
        return None
    
    # 裁剪图像
    cropped_image = image[y:y+height, x:x+width]
    
    return cropped_image


def draw_num_bg(num_idx=0, pos=(0, 0, 32, 32), dir_name="clock"):
    save_path = f"{dir_name}/bg_{num_idx}.png"
    img_28 = cv2.imread(f'nums_28/num{num_idx}_28.png', 0)
    img_32 = cv2.imread(f'nums_32/num{num_idx}_32.png', 0)
    img_bg = cv2.imread('hei6b.jpg', 0)

    x, y, width, height = pos

    # 调用裁剪函数进行裁剪
    cropped_bg = crop_image(img_bg, x, y, width, height)
    # 生成阴影
    padded_img_28 = cv2.copyMakeBorder(img_28, 2, 2, 2, 2, cv2.BORDER_CONSTANT, value=255)

    num_shade = cv2.bitwise_xor(padded_img_28, img_32)
    shaed_bg = cv2.bitwise_or(cropped_bg, num_shade)
    # 叠加数字
    bg_img = cv2.bitwise_and(padded_img_28, shaed_bg)

    # img_show(bg_img, save_path)

    cv2.imwrite(save_path, bg_img)

# 图像取模
def convert_to_image_array(image, array_name="gImage_1"):

    image = binary_image(image)

    height, width = image.shape[:2]
    # array_string = "unsigned char {}[{}] = {{\n    ".format(array_name, (width * height) // 8)
    array_string = "{{\n    ".format(array_name, (width * height) // 8)
    # print("extern unsigned char {}[{}];".format(array_name, (width * height) // 8))

    binary_string = ''
    pix_count = 0
    for i in range(height):
        for j in range(width):
            pixel = 1 if image[i][j] > 0 else 0  # 将像素值转换为0或1
            binary_string += str(pixel)

            if len(binary_string) == 8:  # 每8个像素处理一次
                hex_value = hex(int(binary_string, 2))[2:].zfill(2)
                array_string += "0x{}, ".format(hex_value)
                binary_string = ''

                pix_count = pix_count + 1
                if pix_count % 16 == 0:  # 每16个一行
                    array_string += "\n    "

    if binary_string:  # 处理剩余的像素
        hex_value = hex(int(binary_string, 2))[2:].zfill(2)
        array_string += "    0x{}".format(hex_value)

    array_string = array_string[:-4]
    # array_string += "};"
    array_string += "},"
    return array_string