---
title: "数字图像处理 作业2"
slug: dip_2
date: 2026-03-14
draft: false
categories: ["数字图像", "图像", "python"]
tags: ["数字图像", "图像", "python"]
params:
  hidden: true
---

### 第二次作业：
2.1 马赫带和同时对比度反映了什么共同问题？  
2.2 列举几个视觉错觉的例子。  
2.3 计算5x5邻域各像素到中心象素的的欧式距离，街区距离和棋盘距离。  

**编程作业，请打印**
2.4 编写一个程序，打开灰度图像lena.bmp，读出以（200,200）为左上角的10*10区域的像素值。  
2.5 编写一个程序，打开灰度图像lena.bmp，统计它的最大值、最小值、均值和方差。  
2.6 任意修改灰度的lena.bmp的彩色映像表，写出你的修改方法，给出修改后图像显示（彩色打印）。  
提交时间：3月17日  

#### 2.1 
马赫带与同时对比度的共同点这两个现象共同反映了人类视觉系统的侧抑制（Lateral Inhibition）机制。  
**本质**： 视觉细胞在受光刺激时，不仅会产生兴奋，还会对相邻的细胞产生抑制作用。  
**共同问题**： 它们都说明了人眼感知的亮度并不直接等于物理光强。视觉系统倾向于通过加强边界处的对比度来“锐化”物体轮廓，使我们在物理亮度平滑过渡的地方看到亮条/暗条（马赫带），或在不同背景下对同一灰度块产生不同的亮度感知（同时对比度）。  

#### 2.2 
视觉错觉的例子视觉错觉（Optical Illusion）  
是指生理或心理因素导致的图形与客观事实不符的现象：  
**米勒-莱尔错觉**（Müller-Lyer Illusion）： 两条等长的线段，两端箭头朝内或朝外，会导致线段看起来长短不一。  
**赫林错觉**（Hering Illusion）： 在放射状背景线上的两条平行线，看起来像是向外弯曲。  
**艾宾浩斯错觉**（Ebbinghaus Illusion）： 被大圆包围的圆圆心看起来比被小圆包围的同等大小圆心要小。  
彭罗斯阶梯（Penrose Stairs）： 二维平面上展示的永远向上或向下走的“不可能图形”。  

#### 2.3 
5x5 邻域距离计算设中心像素坐标为 $(0,0)$，邻域内任一像素坐标为 $(x, y)$，其中 $x, y \in \{-2, -1, 0, 1, 2\}$。  
1. **欧式距离** (Euclidean Distance)公式：  
$D_e = \sqrt{x^2 + y^2}$  
距离矩阵如下（仅展示第一象限，其余象限均和第一象限中心对称）：  

| | x=0 | x=1 | x=2 |
| :--- | :--- | :--- | :--- |
| y=0 | 0 | 1 | 2 |
| y=1 | 1 | 1.41 | 2.24 |
| y=2 | 2 | 2.24 | 2.83 |

2. **街区距离** (City Block / $D_4$ Distance)公式：  
$D_4 = |x| + |y|$  
类似棋盘格的十字移动，距离值全为整数。  

|  | x=0 | x=1 | x=2 |
| :--- | :--- | :--- | :--- |
| y=0 | 0 | 1 | 2 |
| y=1 | 1 | 2 | 3 |
| y=2 | 2 | 3 | 4 |

3. **棋盘距离** (Chessboard / $D_8$ Distance)公式：  
$D_8 = \max(|x|, |y|)$  
允许对角线移动，每层“外圈”的距离值相等。  

|  | x=0 | x=1 | x=2 |
| :--- | :--- | :--- | :--- |
| y=0 | 0 | 1 | 2 |
| y=1 | 1 | 1 | 2 |
| y=2 | 2 | 2 | 2 |

### 编程作业
#### 2.4
不允许用库。所以自己写一个读 `bmp` 文件的函数 `read_image`。  

先看文件属性：  
```txt
分辨率  256x256
宽度  256 像素
高度  256 像素
位深度  8 位
名称  lena.bmp
项目类型  BMP 文件
```

`hw2_1.py`
```python
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
```
输出结果：  
```
图像大小: 256x256, 数据长度: 65536 bytes
[132, 130, 127, 124, 122, 115, 111, 110, 106, 102]
[135, 135, 133, 133, 127, 124, 122, 118, 114, 111]
[137, 137, 137, 136, 133, 129, 126, 123, 122, 118]
[140, 140, 138, 136, 136, 133, 131, 128, 127, 122]
[142, 143, 138, 137, 135, 134, 134, 130, 128, 125]
[144, 142, 140, 140, 137, 136, 139, 136, 130, 129]
[142, 142, 141, 140, 140, 137, 137, 136, 134, 130]
[142, 143, 141, 141, 139, 139, 138, 134, 134, 134]
[141, 143, 141, 143, 140, 140, 139, 136, 134, 134]
[142, 143, 143, 143, 141, 139, 139, 138, 135, 138]
```

#### 2.5
`hw2_2.py`
```python
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
```
输出结果：  
```
图像大小: 256x256, 数据长度: 65536 bytes
最小值: 23, 最大值: 240, 均值: 124, 方差: 2250
```

### 2.6
`hw2_3.py`
```python
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
```
输出的新的 `lena_red.bmp` 应该是红色的，如图：
![lena_red.bmp](/data/数字图像处理/dip_hw2/lena_red.bmp)