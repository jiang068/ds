from hw2_1 import read_image

# 修改调色板函数
def modify_palette_to_new_file(input_file, output_file, new_palette):
    with open(input_file, 'rb') as f:
        full_data = bytearray(f.read())  # 读取全部数据并转为可修改的 bytearray

    # 1. 获取调色板的起始位置 (通常是 54)
    # 也可以从 header[10:14] 获取像素开始位置，调色板就在 54 到那个位置之间
    palette_start = 54 
    
    # 2. 修改调色板 (8位 BMP 的调色板每项占 4 字节：B, G, R, Reserved)
    for i in range(256):
        r, g, b = new_palette[i]
        # 计算当前颜色项在 bytearray 中的索引
        idx = palette_start + (i * 4)
        
        # 写入 B, G, R 和 保留位 (0)
        full_data[idx] = b
        full_data[idx + 1] = g
        full_data[idx + 2] = r
        full_data[idx + 3] = 0

    # 3. 将修改后的完整数据写入新文件
    with open(output_file, 'wb') as f:
        f.write(full_data)

if __name__ == '__main__':
    # 直接把灰度图的 i 映射到红色通道就算调色了。
    red_palette = [(i, 0, 0) for i in range(256)] 

    try:
        modify_palette_to_new_file('lena.bmp', 'lena_red.bmp', red_palette)
        print("成功！已生成新文件：lena_red.bmp")
    except FileNotFoundError:
        print("未找到原图，请检查路径。")