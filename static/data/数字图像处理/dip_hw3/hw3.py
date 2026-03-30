import math
import cmath
import sys
import os

# 导入 hw2 的读取函数
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from hw2.hw2_1 import read_image

# --- 1. 简单的变换函数 ---

def dft_1d(data):
    N = len(data)
    return [sum(data[n] * cmath.exp(-2j * math.pi * k * n / N) for n in range(N)) for k in range(N)]

def dct_1d(data):
    N = len(data)
    output = []
    for k in range(N):
        alpha = math.sqrt(1/N) if k == 0 else math.sqrt(2/N)
        s = sum(data[n] * math.cos(math.pi * k * (2*n + 1) / (2*N)) for n in range(N))
        output.append(s * alpha)
    return output

def transform_2d(matrix, func_1d):
    # 行变换
    rows_done = [func_1d(row) for row in matrix]
    # 列变换 (转置 -> 变换 -> 转置)
    cols = [[rows_done[r][c] for r in range(256)] for c in range(256)]
    cols_done = [func_1d(col) for col in cols]
    return [[cols_done[c][r] for c in range(256)] for r in range(256)]

# --- 2. 频谱处理 (取模、对数压缩) ---

def process_spectrum(matrix, is_dft=False):
    # 1. 取模
    mag = [[abs(matrix[y][x]) for x in range(256)] for y in range(256)]
    # 2. DFT 中心化
    if is_dft:
        tmp = [[0]*256 for _ in range(256)]
        for y in range(256):
            for x in range(256):
                tmp[(y+128)%256][(x+128)%256] = mag[y][x]
        mag = tmp
    # 3. 对数映射与归一化
    log_mag = [[math.log(1 + mag[y][x]) for x in range(256)] for y in range(256)]
    flat = [v for row in log_mag for v in row]
    mi, ma = min(flat), max(flat)
    return [int(255 * (v - mi) / (ma - mi)) for row in log_mag for v in row]

# --- 3. 手写 BMP 输出函数 ---

def save_bmp_8bit(data, width, height, filename):
    # BMP 文件头 (14 bytes) + DIB 头 (40 bytes) + 调色板 (1024 bytes)
    offset = 54 + 1024
    file_size = offset + len(data)
    
    # 构建 Header
    header = bytearray([0]*offset)
    header[0:2] = b'BM'
    header[2:6] = file_size.to_bytes(4, 'little')
    header[10:14] = offset.to_bytes(4, 'little')
    header[14:18] = (40).to_bytes(4, 'little')
    header[18:22] = width.to_bytes(4, 'little')
    header[22:26] = height.to_bytes(4, 'little')
    header[26:28] = (1).to_bytes(2, 'little')
    header[28:30] = (8).to_bytes(2, 'little')
    
    # 写入灰度调色板 (B, G, R, Reserved)
    for i in range(256):
        start = 54 + i * 4
        header[start:start+4] = bytes([i, i, i, 0])
    
    #  BMP 像素是从底向上存的，需要翻转数据
    pixel_bytes = bytearray()
    for y in range(height - 1, -1, -1):
        pixel_bytes.extend(data[y*width : (y+1)*width])
        
    with open(filename, 'wb') as f:
        f.write(header)
        f.write(pixel_bytes)

# --- 主程序 ---

if __name__ == '__main__':
    # 读取 lena.bmp 
    w, h, raw = read_image('lena.bmp')
    
    # 转为二维矩阵
    img = []
    for y in range(256):
        # 兼容 BMP 倒序存储
        row = list(raw[(255-y)*256 : (255-y)*256 + 256])
        img.append(row)

    print("正在计算 DFT...")
    dft_res = transform_2d(img, dft_1d)
    dft_data = process_spectrum(dft_res, is_dft=True)
    save_bmp_8bit(dft_data, 256, 256, 'lena_dft.bmp')

    print("正在计算 DCT...")
    dct_res = transform_2d(img, dct_1d)
    dct_data = process_spectrum(dct_res, is_dft=False)
    save_bmp_8bit(dct_data, 256, 256, 'lena_dct.bmp')

    print("完成！请查看 lena_dft.bmp 和 lena_dct.bmp")