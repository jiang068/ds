# read_image 函数可以复用
from hw2_1 import read_image

# 统计图像基本信息函数
def analyze_image(pixel_data, width, height):
    # 1. 计算最大值、最小值、均值
    min_val = 255
    max_val = 0
    total = 0
    count = 0

    for i in range(len(pixel_data)):
        pixel_value = pixel_data[i]
        if pixel_value < min_val:
            min_val = pixel_value
        if pixel_value > max_val:
            max_val = pixel_value
        total += pixel_value
        count += 1

    mean = total // count

    # 2. 计算方差
    squared_diffs = sum((pixel_data[i] - mean) ** 2 for i in range(len(pixel_data)))
    variance = squared_diffs // count

    return min_val, max_val, mean, variance

if __name__ == '__main__':
    try:
        width, height, pixel_data = read_image('lena.bmp')
        # 打印一下信息确认读取正常
        print(f"图像大小: {width}x{height}, 数据长度: {len(pixel_data)} bytes")

        min_val, max_val, mean, variance = analyze_image(pixel_data, width, height)
        print(f"最小值: {min_val}, 最大值: {max_val}, 均值: {mean}, 方差: {variance}")

    except FileNotFoundError:
        print("错误：找不到 lena.bmp 文件，请确保它在当前目录下。")