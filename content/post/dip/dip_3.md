---
title: "数字图像处理 作业3"
slug: dip_3
date: 2026-03-31
draft: false
categories: ["数字图像", "图像", "python"]
tags: ["数字图像", "图像", "python"]
params:
  hidden: true 
---


### 第三次作业
3.1 计算下图的 DFT, DCT, Hadamard 变换和Haar变换
![题图1](/data/数字图像处理/dip_hw3/hw3_t1.png)
3.2 设有一组64*64的图像,它们的协方差矩阵式单位矩阵.如果只使用一半的原始特征值计算重建图像,那么原始图像和重建图像间的均方误差是多少?

**编程作业**
3.3 编程实现lena.bmp的离散Fourier变换和离散余弦变换，并显示频谱图像。
【提交时间】：3月31日

#### 3.1
这是一个典型的图像处理或信号处理练习。  
给定的 $4 \times
4$ 矩阵 $f$ 为：  
\[ f = \left[ \begin{array}{cccc}
     0 & 1 & 1 & 0\\
     0 & 1 & 1 & 0\\
     0 & 1 & 1 & 0\\
     0 & 1 & 1 & 0
   \end{array} \right] \]  
由于该矩阵的所有行都相同，且所有列也相同（属于可分离矩阵），我们可以利用可分离变换的性质：  
$$F= TfT^T$$  
其中 $T$ 是变换矩阵。  
对于这个特定的矩阵，它可以写成外积形式：  
$$f = \textbf{1} \cdot \mathbf{r}$$  
其中 $\textbf{1} = [1, 1, 1, 1]^T$，$\mathbf{r} = [0, 1, 1, 0]$.   
因此，$F = (T \textbf{1}) (T \mathbf{r})^T$。  
在所有的标准正交变换（DFT, DCT, Hadamard, Haar）中，第一行基向量通常是常数向量（DC分量）。  
因此 $T \textbf{1} = [2, 0, 0, 0]^T$（假设使用了单位矩阵归一化系数 $1 / \sqrt{N}$）。  
这意味着结果矩阵只有第一行有非零值，该行数值等于行向量
$[0, 1, 1, 0]$ 的一维变换结果乘以 $2$。  
1. 二维离散傅里叶变换 (DFT)一维 DFT 公式（归一化）：  
$$X (k) = \frac{1}{2}  \sum_{n = 0}^3 x (n) e^{- j \frac{2 \pi}{4} kn}$$  
对于行向量 $[0, 1, 1, 0]$：  
$$
\begin{aligned}
X(0) &= \frac{1}{2}(0 + 1 + 1 + 0) = 1 \\
X(1) &= \frac{1}{2}(0 + 1 \cdot (-j) + 1 \cdot (-1) + 0) = -0.5 - 0.5j \\
X(2) &= \frac{1}{2}(0 + 1 \cdot (-1) + 1 \cdot (1) + 0) = 0 \\
X(3) &= \frac{1}{2}(0 + 1 \cdot (j) + 1 \cdot (-1) + 0) = -0.5 + 0.5j
\end{aligned}
$$
乘以系数 $2$，得到二维 DFT 结果：
\[ F_{DFT} = \left[ \begin{array}{cccc}
     2 & - 1 - j & 0 & - 1 + j\\
     0 & 0 & 0 & 0\\
     0 & 0 & 0 & 0\\
     0 & 0 & 0 & 0
   \end{array} \right] \]
2. 离散余弦变换 (DCT)使用标准 DCT-II 变换。行向量 $[0, 1, 1,
0]$ 的变换：  
$X (0) = 1 $ (DC分量)  
$X (1) = 0$ (对称性抵消)  
$X (2) = \frac{1}{2}  \sum x (n) \cos (\frac{(2 n + 1) 2 \pi}{8}) = \frac{1}{2}  (\cos (\frac{3 \pi}{4}) + \cos (\frac{5 \pi}{4})) = - \frac{\sqrt{2}}{2} \approx - 0.707$   
$X (3) = 0$  
乘以系数 $2$，得到二维 DCT 结果：
\[ F_{DCT} = \left[ \begin{array}{cccc}
     2 & 0 & - \sqrt{2} & 0\\
     0 & 0 & 0 & 0\\
     0 & 0 & 0 & 0\\
     0 & 0 & 0 & 0
   \end{array} \right] \approx \left[ \begin{array}{cccc}
     2 & 0 & - 1.414 & 0\\
     0 & 0 & 0 & 0\\
     0 & 0 & 0 & 0\\
     0 & 0 & 0 & 0
   \end{array} \right] \]
3. Hadamard 变换 (WHT)  
使用 Walsh 排序或自然排序（此处以常用的 $4 \times 4$ 哈达玛矩阵为例）：  
$$H_4 = \frac{1}{2} \left[ \begin{array}{cccc}
  1 & 1 & 1 & 1\\
  1 & - 1 & 1 & - 1\\
  1 & 1 & - 1 & - 1\\
  1 & - 1 & - 1 & 1
\end{array} \right]$$  
行向量 $[0, 1, 1, 0]$ 的变换为 $[1, 0, 0, -
1]^T$。乘以系数 $2$，得到二维 WHT 结果：
\[ F_{WHT} = \left[ \begin{array}{cccc}
     2 & 0 & 0 & - 2\\
     0 & 0 & 0 & 0\\
     0 & 0 & 0 & 0\\
     0 & 0 & 0 & 0
   \end{array} \right] \]
4. Haar 变换  
$4 \times 4$ Haar 变换矩阵：  
$T_{Haar} = \frac{1}{2} \left[
\begin{array}{cccc}
  1 & 1 & 1 & 1\\
  1 & 1 & - 1 & - 1\\
  \sqrt{2} & - \sqrt{2} & 0 & 0\\
  0 & 0 & \sqrt{2} & - \sqrt{2}
\end{array} \right]$  
行向量 $[0, 1, 1, 0]$ 的变换：  
$X (0) = 1$  
$X (1) = \frac{1}{2}  (0 + 1 - 1 - 0) = 0$  
$X (2) = \frac{1}{2}  (0 - \sqrt{2}) = - \frac{1}{\sqrt{2}}$  
$X (3) = \frac{1}{2}  (\sqrt{2} - 0) = \frac{1}{\sqrt{2}}$    
乘以系数 $2$，得到二维 Haar 变换结果：  
\[ F_{Haar} = \left[ \begin{array}{cccc}
     2 & 0 & - \sqrt{2} & \sqrt{2}\\
     0 & 0 & 0 & 0\\
     0 & 0 & 0 & 0\\
     0 & 0 & 0 & 0
   \end{array} \right] \]
**总结**：  
由于输入图像在垂直方向上是恒定的（频率为0），所有变换后的能量都集中在第一行（即 $u = 0$ 的行），体现了图像的水平结构特性。

#### 3.2
这是一个关于 KLT（K-L变换/主成分分析 PCA）重建误差的典型问题。  
在这种情况下，均方误差（MSE）可以通过协方差矩阵的特征值直接计算得出。  
**核心结论**  
在图像压缩或重建中，如果使用前 $k$ 个主成分进行重建，其最小均方误差等于所有被舍弃的特征值之和。  
**计算步骤**  
确定维度 ($N$)：  
图像大小为 $64 \times 64$，将其展开为一维向量后，总维数为：  
\[ N = 64 \times 64 = 4096 \]
分析特征值 ($\lambda$)：  
题目给出协方差矩阵是单位矩阵 ($C = I$)。  
单位矩阵的特征值全部相等，且都等于 $1$：  
\[ \lambda_1 = \lambda_2 = \ldots = \lambda_{4096} = 1 \]
计算重建误差：  
题目指出只使用''一半的特征值''进行重建，意味着我们保留了
$2048$ 个特征值，舍弃了剩下的 $M = 2048$  个特征值。  
总平方误差 (Total Squared Error) 为舍弃的特征值之和：  
\[ \sum_{i = 2049}^{4096} \lambda_i = 2048 \times 1 = 2048 \]
均方误差 (MSE) 是总误差除以总维数 $N$：  
\[ MSE = \frac{1}{N}  \sum_{i = k + 1}^N \lambda_i = \frac{2048}{4096} = 0.5 \]  
**结果**  
原始图像和重建图像间的均方误差是 $0.5$。  

**深度解析**：为什么是 0.5？  
- **物理意义**：  
当协方差矩阵为单位矩阵时，意味着图像的每一个像素点之间完全不相关，且每个像素的方差（能量）都是 $1$。  
- **压缩效率**：  
在这种情况下，没有任何一个维度比其他维度更重要（没有能量集中现象）。因此，当你丢弃一半的数据时，你恰好丢弃了一半的能量，误差也就正好是 $50\%$。  
这反映了一个事实：KLT 变换（或 PCA）在处理完全不相关的数据（如白噪声）时是没有压缩优势的。

#### 3.3  
```python
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
```
**关键点解释**：  
1. 无库保存图像：  
- 我们手动生成了一个 8 位伪彩色（Grayscale）BMP 文件。  
- 它由 54 字节头 + 1024 字节调色板（定义 0-255 级灰色）+ 像素数据组成。  
- 注意：BMP 数据在文件中是''倒着''存的（第一行数据实际上是图像的最底行），所以我们在 `save\_bmp\_8bit` 里做了 $range(height - 1, -1, -1)$ 处理。  
2. 频谱处理逻辑：  
- DFT：进行了``中心化''操作（四象限交换），这样亮亮的低频点就在正中心了。  
- DCT：不需要中心化，低频通常集中在左上角。  
- 对数压缩：如果不做 math.log，你看到的图像会是几乎全黑的，只有一个白点。  
3. 运行速度：256 阶的变换虽然是 $O (N^3)$，但在 Python
下纯循环还是有几千万次运算。运行约需 20-30 秒。如果你觉得慢，那是 Python 循环的锅，不是算法问题。 

**输出结果**：  
lena_dct.bmp  
![lena_dct.bmp](/data/数字图像处理/dip_hw3/lena_dct.bmp)  

lena_dft.bmp
![lena_dft.bmp](/data/数字图像处理/dip_hw3/lena_dft.bmp)
