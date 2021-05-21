import numpy as np
from PIL import Image

image = np.array(Image.open('/home/jm1417/Downloads/jpeg2000-home.jpg').convert('L'))

#image = np.array([[79, 79, 59, 82, 54, 33, 6, 92],
                  #[-17, 78, 60, -42, -80, -90, -106, 74],
                  #[-1, 74, 23, -104, -83, -59, -104, 15],
                  #[8, 79, 71, -107, -82, -47, -115, 104],
                  #[20, 83, -25, -87, -113, -78, 123, 123],
                  #[25, -61, -76, -80, -95, -57, -59, 123],
                  #[19, -69, -97, -94, -69, -75, -75, -11],
                  #[-61, -59, -90, -95, -77, -101, -88, -39]])

image = image - 128
shape = image.shape
print("Image: ", image)
# image_block_list = [[26, -5, -5, -5, -5, -5, -5, 8],
#                     [64, 52, 8, 26, 26, 26, 8, -18],
#                     [126, 70, 26, 26, 52, 26, -5, -5],
#                     [111, 52, 8, 52, 52, 38, -5, -5],
#                     [52, 26, 8, 39, 38, 21, 8, 8],
#                     [0, 8, -5, 8, 26, 52, 70, 26],
#                     [-5, -23, -18, 21, 8, 8, 52, 38],
#                     [-18, 8, -5, -5, -5, 8, 26, 8]]
# image_block_list = [[62, 5, 5, 54, 49, 48, 47, 55],
#                     [62, 57, 54, 52, 48, 47, 48, 53],
#                     [61, 60, 52, 49, 48, 47, 49, 54],
#                     [63, 61, 60, 60, 63, 65, 68, 65],
#                     [67, 67, 70, 74, 79, 85, 91, 92],
#                     [92, 95, 101, 106, 114, 115, 112, 117],
#                     [96, 111, 115, 119, 128, 128, 130, 127],
#                     [109, 121, 127, 133, 139, 141, 140, 133]]
# image = np.array(image_block_list)


T_list = [[.3536, .3536, .3536, .3536, .3536, .3536, .3536, .3536],
          [.4904, .4157, .2778, .0975, -.0975, -.2778, -.4157, -.4904],
          [.4619, .1913, -.1913, -.4619, -.4619, -.1913, .1913, .4619],
          [.4157, -.0975, -.4904, -.2778, .2778, .4904, .0975, -.4157],
          [.3536, -.3536, -.3536, .3536, .3536, -.3536, -.3536, .3536],
          [.2778, -.4904, .0975, .4157, -.4157, -.0975, .4904, -.2778],
          [.1913, -.4619, .4619, -.1913, -.1913, .4619, -.4619, .1913],
          [.0975, -.2778, .4157, -.4904, .4904, -.4157, .2778, -.0975]]
#
# T_list = [[35, 35, 35, 35, 35, 35, 35, 35],
#           [49, 42, 28, 10, -10, -28, -42, -49],
#           [46, 19, -19, -46, -46, -19, 19, 46],
#           [42, -10, -49, -28, 28, 49, 10, -42],
#           [35, -35, -35, 35, 35, -35, -35, 35],
#           [28, -49, 10, 42, -42, -10, 49, -28],
#           [19, -46, 46, -19, -19, 46, -46, 19],
#           [10, -28, 42, -49, 49, -42, 27, -10]]

sf = 32
T = np.array(T_list)
T = T * sf
T = np.rint(T).astype(int)
print(T)

T_trans = T.T
print(T_trans)

Q50_list = [[16, 11, 10, 16, 24, 40, 51, 61],
            [12, 12, 14, 19, 26, 58, 60, 55],
            [14, 13, 16, 24, 40, 57, 69, 56],
            [14, 17, 22, 29, 51, 87, 80, 62],
            [18, 22, 37, 56, 68, 109, 103, 77],
            [24, 35, 55, 64, 81, 104, 113, 92],
            [49, 64, 78, 87, 103, 121, 120, 101],
            [72, 92, 95, 98, 112, 100, 103, 99]]
Q50 = np.array(Q50_list)
Q50 = np.tile(Q50, (shape[0] // 8, shape[1] // 8))

D = np.zeros(shape)
for row in range(0, shape[0], 8):
    for col in range(0, shape[1], 8):
        D[row:row + 8, col:col + 8] = T @ image[row:row + 8, col:col + 8]

print("First mult", D)

for row in range(0, shape[0], 8):
    for col in range(0, shape[1], 8):
        D[row:row + 8, col:col + 8] = D[row:row + 8, col:col + 8] @ T_trans

print("Second mult", D)

# D = T@image@T_trans
D = D / (sf * sf)
D = np.rint(D).astype(int)

C = np.round(D / Q50)
C = C.astype(int)
print("C", C)

R = Q50 * C
print("R", R)
# decompress = np.round((T_trans @ R @ T))
decompress = np.zeros(shape)
for row in range(0, shape[0], 8):
    for col in range(0, shape[1], 8):
        decompress[row:row + 8, col:col + 8] = np.round(T_trans @ R[row:row + 8, col:col + 8] @ T)

decompress = np.rint((decompress / (sf * sf))).astype(np.int8)
decompress = (decompress + 128).astype(np.uint8)
print("decompress", decompress)


#
def visualise(original, decompressed):
    Image.fromarray(original).save("original.png")
    Image.fromarray(decompressed).save("new.png")


image = (image + 128).astype(np.uint8)
visualise(image, decompress)
