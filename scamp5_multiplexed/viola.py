import numpy as np
from PIL import Image

def scale(img):
    height,width,channels = img.shape
    s_width = 128
    s_height = 128
    emptyImage = np.zeros((s_width, s_height, channels),np.uint8)
    sh = s_height / height
    sw = s_width / width
    for i in range(s_height):
        for j in range(s_width):
            x=int(i/sh)
            y=int(j/sw)
            emptyImage[i,j]=img[x,y]
    return emptyImage


def scale_test():
    img = np.array(Image.open('/home/jm1417/Downloads/jpeg2000-home.jpg'))
    scaled = scale(img)

    Image.fromarray(img).save("original_viola.png")
    Image.fromarray(scaled).save("downscaled.png")

