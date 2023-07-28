import cv2
import numpy as np
from utils import resize_image, modify_filename

m1 = np.array([[0, 2], [3, 1]])
u1 = np.ones((2, 2))
m2 = np.concatenate((np.concatenate((4 * m1, 4 * m1 + 2 * u1), axis=1),
                     np.concatenate((4 * m1 + 3 * u1, 4 * m1 + u1), axis=1)), axis=0)
u2 = np.ones((4, 4))
m3 = np.concatenate((np.concatenate((4 * m2, 4 * m2 + 2 * u2), axis=1),
                     np.concatenate((4 * m2 + 3 * u2, 4 * m2 + u2), axis=1)), axis=0)

image_path = "hei0.jpg"
I = cv2.imread(image_path)

I = resize_image(I, 200)

gI = 0.2989 * I[:, :, 2] + 0.5870 * I[:, :, 1] + 0.1140 * I[:, :, 0]

h, w = gI.shape

bw = np.zeros((h, w), dtype=np.uint8)
for i in range(h):
    for j in range(w):
        if gI[i, j] / 4 > m3[i % 8, j % 8 ]:
            bw[i, j] = 255
        else:
            bw[i, j] = 0

cv2.imwrite(modify_filename(image_path, "bayer_binary", True), bw)
cv2.imshow("Bayer Dithering", bw)
cv2.waitKey(0)
cv2.destroyAllWindows()
