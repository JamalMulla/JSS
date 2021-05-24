import numpy as np
from PIL import Image

def save(image, name):
    image = (image + 128).astype(np.uint8)
    Image.fromarray(image).save(name + ".png")


def proper(image):
    image = image - 128
    shape = image.shape
    print(shape)
    
    T_list = [[.3536, .3536, .3536, .3536, .3536, .3536, .3536, .3536],
          [.4904, .4157, .2778, .0975, -.0975, -.2778, -.4157, -.4904],
          [.4619, .1913, -.1913, -.4619, -.4619, -.1913, .1913, .4619],
          [.4157, -.0975, -.4904, -.2778, .2778, .4904, .0975, -.4157],
          [.3536, -.3536, -.3536, .3536, .3536, -.3536, -.3536, .3536],
          [.2778, -.4904, .0975, .4157, -.4157, -.0975, .4904, -.2778],
          [.1913, -.4619, .4619, -.1913, -.1913, .4619, -.4619, .1913],
          [.0975, -.2778, .4157, -.4904, .4904, -.4157, .2778, -.0975]]
    
    
    T = np.array(T_list)
    T_trans = T.T
    
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


    for row in range(0, shape[0], 8):
        for col in range(0, shape[1], 8):
            D[row:row + 8, col:col + 8] = D[row:row + 8, col:col + 8] @ T_trans
            
    C = np.round(D / Q50)
    C = C.astype(int)
            
            
    count = 0
    for row in range(0, shape[0], 1):
        for col in range(0, shape[1], 1):
            if C[row][col] != 0:
                count+=1
    print(count/(shape[0]*shape[1]) * 100, "% coefficents retained with proper")
    return C
    
def approx(image):
    image = image - 128
    shape = image.shape
    
    T_list = [[.3536, .3536, .3536, .3536, .3536, .3536, .3536, .3536],
          [.4904, .4157, .2778, .0975, -.0975, -.2778, -.4157, -.4904],
          [.4619, .1913, -.1913, -.4619, -.4619, -.1913, .1913, .4619],
          [.4157, -.0975, -.4904, -.2778, .2778, .4904, .0975, -.4157],
          [.3536, -.3536, -.3536, .3536, .3536, -.3536, -.3536, .3536],
          [.2778, -.4904, .0975, .4157, -.4157, -.0975, .4904, -.2778],
          [.1913, -.4619, .4619, -.1913, -.1913, .4619, -.4619, .1913],
          [.0975, -.2778, .4157, -.4904, .4904, -.4157, .2778, -.0975]]
    
    sf = 100
    T = np.array(T_list)
    T = T * sf
    T = np.rint(T).astype(int)
    
    T_trans = T.T
    
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

    for row in range(0, shape[0], 8):
        for col in range(0, shape[1], 8):
            D[row:row + 8, col:col + 8] = D[row:row + 8, col:col + 8] @ T_trans
            
    D = D / (sf * sf)
    D = np.rint(D).astype(int)

    C = np.round(D / Q50)
    C = C.astype(int)
    
    count = 0
    for row in range(0, shape[0], 1):
        for col in range(0, shape[1], 1):
            if C[row][col] != 0:
                count+=1
    print(count/(shape[0]*shape[1]) * 100, "% coefficents retained with approx")
    return C
    
image = np.array(Image.open('/home/jm1417/Downloads/icecream.jpg').convert('L'))
proper(image)
approx(image)





#
# T_list = [[35, 35, 35, 35, 35, 35, 35, 35],
#           [49, 42, 28, 10, -10, -28, -42, -49],
#           [46, 19, -19, -46, -46, -19, 19, 46],
#           [42, -10, -49, -28, 28, 49, 10, -42],
#           [35, -35, -35, 35, 35, -35, -35, 35],
#           [28, -49, 10, 42, -42, -10, 49, -28],
#           [19, -46, 46, -19, -19, 46, -46, 19],
#           [10, -28, 42, -49, 49, -42, 27, -10]]


#print(T)

#T_trans = T.T
#print(T_trans)

#Q50_list = [[16, 11, 10, 16, 24, 40, 51, 61],
            #[12, 12, 14, 19, 26, 58, 60, 55],
            #[14, 13, 16, 24, 40, 57, 69, 56],
            #[14, 17, 22, 29, 51, 87, 80, 62],
            #[18, 22, 37, 56, 68, 109, 103, 77],
            #[24, 35, 55, 64, 81, 104, 113, 92],
            #[49, 64, 78, 87, 103, 121, 120, 101],
            #[72, 92, 95, 98, 112, 100, 103, 99]]
#Q50 = np.array(Q50_list)
#Q50 = np.tile(Q50, (shape[0] // 8, shape[1] // 8))

#D = np.zeros(shape)
#for row in range(0, shape[0], 8):
    #for col in range(0, shape[1], 8):
        #D[row:row + 8, col:col + 8] = T @ image[row:row + 8, col:col + 8]

#print("First mult", D)

#for row in range(0, shape[0], 8):
    #for col in range(0, shape[1], 8):
        #D[row:row + 8, col:col + 8] = D[row:row + 8, col:col + 8] @ T_trans

#print("Second mult", D)




# D = T@image@T_trans
#D = D / (sf * sf)
#D = np.rint(D).astype(int)

#C = np.round(D / Q50)
#C = C.astype(int)
#print("C", C)

#R = Q50 * C
#print("R", R)
# decompress = np.round((T_trans @ R @ T))
#decompress = np.zeros(shape)
#for row in range(0, shape[0], 8):
    #for col in range(0, shape[1], 8):
        #decompress[row:row + 8, col:col + 8] = np.round(T_trans @ R[row:row + 8, col:col + 8] @ T)

#decompress = np.rint((decompress / (sf * sf))).astype(np.int8)
#decompress = (decompress + 128).astype(np.uint8)
#print("decompress", decompress)



