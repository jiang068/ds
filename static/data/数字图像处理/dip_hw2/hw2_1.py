# 读取图像函数
def read_image(filename):
    with open(filename, 'rb') as f:
        header = f.read(54)
        # 1. 自动获取像素数据在文件中的起始位置 (bfOffBits)
        # 对于8位灰度图，这通常是 54 (header) + 1024 (palette) = 1078
        data_offset = int.from_bytes(header[10:14], byteorder='little')
        
        width = int.from_bytes(header[18:22], byteorder='little')
        height = int.from_bytes(header[22:26], byteorder='little')
        
        # 2. 移动指针到数据区并读取
        f.seek(data_offset)
        pixel_data = f.read()
        
    return width, height, pixel_data

# 读取指定区域像素值函数
def read_region(pixel_data, width, height, x, y, region_size):
    region = []
    # BMP 每行字节数必须是 4 的倍数 (Padding)
    # 对于 256 宽度的图，刚好是 4 的倍数，不需要额外填充，但写上更严谨
    row_size = (width + 3) & ~3 
    
    for i in range(region_size):
        for j in range(region_size):
            # 3. 修正坐标：BMP 是从最后一行向上存的
            # 这里的 y 是左上角坐标，对应图像内存中靠近底部的行
            actual_y = (height - 1) - (y + i)
            
            # 8位图索引：行偏移 + 列偏移 (没有 * 3)
            pixel_index = actual_y * row_size + (x + j)
            
            if pixel_index < len(pixel_data):
                # 直接获取单个数值 (0-255)
                pixel_value = pixel_data[pixel_index]
                region.append(pixel_value)
    return region

if __name__ == '__main__':
    try:
        width, height, pixel_data = read_image('lena.bmp')
        # 打印一下信息确认读取正常
        print(f"图像大小: {width}x{height}, 数据长度: {len(pixel_data)} bytes")
        
        region = read_region(pixel_data, width, height, 200, 200, 10)
        
        # 打印 10x10 矩阵
        for i in range(10):
            # 每行打印 10 个像素值
            row_values = region[i*10 : (i+1)*10]
            print(row_values)
            
    except FileNotFoundError:
        print("错误：找不到 lena.bmp 文件，请确保它在当前目录下。")