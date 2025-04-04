/*实验3 Huffman编码和解码
实验时间
6课时。
实验目的
1. 掌握二叉树的存储结构和常用算法。
2. 熟练掌握递归程序设计方法。
问题描述
Huffman编码是二叉树的典型应用之一。给定一个文本文件stdio.h，对其进行编码和解码，计算压缩比，从而了解数据压缩的基本原理。
实验内容
1. 对文本文件统计各个字符的出现频率，构造Huffman树。
2. 以Huffman树对文本文件进行编码，统计编码后的比特数，除以8得到字节数。用原文件的大小（字节数）除以编码后的字节数，即求得压缩比。
3. 将编码后的比特流再进行解码，写入一个新的文本文件，与原文件比较，是否完全一致？比较文件可使用Windows命令行工具fc。
*/


#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;
//定义树
struct HTNode {
    char ch;
    int freq;
    HTNode *left, *right;
    HTNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};
//定义比较用的结构体
struct compare {
    bool operator()(HTNode* l, HTNode* r) {
        return l->freq > r->freq;
    }
};
//打开文件
void frqc(const char *file, unordered_map<char, int> &freq) {
    ifstream in(file, ios::in);
    if (!in.is_open()) {
        cout << "文件打开失败！" << endl;
        return;
    }

    char ch;
    while (in.get(ch)) {
        freq[ch]++;
    }
    in.close();
}
//复制粘贴来的哈夫曼树算法
HTNode* buildHuffmanTree(unordered_map<char, int> &freq) {
    priority_queue<HTNode*, vector<HTNode*>, compare> minHeap;
    for (auto pair : freq) {
        minHeap.push(new HTNode(pair.first, pair.second));
    }

    while (minHeap.size() != 1) {
        HTNode *left = minHeap.top(); minHeap.pop();
        HTNode *right = minHeap.top(); minHeap.pop();
        HTNode *top = new HTNode('\0', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }

    return minHeap.top();
}

//====================================================编码
void encode(HTNode* root, string str, unordered_map<char, string> &huffmanCode) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
        cout << "字符: " << root->ch << " 编码: " << str << endl;
    }
    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

string getEncodedString(const char *file, unordered_map<char, string> &huffmanCode) {
    ifstream in(file, ios::in);
    if (!in.is_open()) {
        cout << "文件打开失败！" << endl;
        return "";
    }

    string encodedString = "";
    char ch;
    while (in.get(ch)) {
        encodedString += huffmanCode[ch];
    }
    in.close();
    return encodedString;
}

void saveEncodedFile(const string &encodedString, const char *encodedFile) {
    ofstream out(encodedFile, ios::out | ios::binary);
    if (!out.is_open()) {
        cout << "文件保存失败！" << endl;
        return;
    }

    out << encodedString;
    out.close();
}
//===================================================计算压缩比
double calculateCompressionRatio(const char *originalFile, const char *encodedFile) {
    ifstream inOriginal(originalFile, ios::binary | ios::ate);
    ifstream inEncoded(encodedFile, ios::binary | ios::ate);
    if (!inOriginal.is_open() || !inEncoded.is_open()) {
        cout << "文件打开失败！" << endl;
        return 0.0;
    }

    double originalSize = inOriginal.tellg();
    printf("编码前的比特数：%d\n", &originalSize);
    double encodedSize = inEncoded.tellg() / 8.0; // 比特数除以8得到字节数
    inOriginal.close();
    inEncoded.close();

    return encodedSize/originalSize ;
    
}

//=================================解码
void decode(HTNode* root, const string &encodedString, const char *decodedFile) {
    ofstream out(decodedFile, ios::out);
    if (!out.is_open()) {
        cout << "文件保存失败！" << endl;
        return;
    }

    HTNode* curr = root;
    for (char bit : encodedString) {
        if (bit == '0') {
            curr = curr->left;
        } else {
            curr = curr->right;
        }

        if (!curr->left && !curr->right) {
            out.put(curr->ch);
            curr = root;
        }
    }
    out.close();
}


int main() {
    const char *inputFile = "stdio.h";//直接读取同目录下的文件
    const char *encodedFile = "encoded.bin";
    const char *decodedFile = "decoded.txt";

    unordered_map<char, int> freq;
    frqc(inputFile, freq);

    HTNode* root = buildHuffmanTree(freq);

    unordered_map<char, string> huffmanCode;
    encode(root, "", huffmanCode);

    string encodedString = getEncodedString(inputFile, huffmanCode);
    saveEncodedFile(encodedString, encodedFile);

    // 计算压缩前的比特数
    ifstream inFile(inputFile, ios::binary | ios::ate);
    streamsize inputFileSize = inFile.tellg();
    inFile.close();
    int originalBits = inputFileSize * 8;
    cout << "压缩前的比特数: " << originalBits << endl;

    // 计算压缩后的比特数
    int compressedBits = encodedString.size();
    cout << "压缩后的比特数: " << compressedBits << endl;

    double compressionRatio = calculateCompressionRatio(inputFile, encodedFile);
    cout << "压缩比: " << compressionRatio << endl;

    decode(root, encodedString, decodedFile);

    return 0;
}