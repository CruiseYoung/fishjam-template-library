#ifndef FTL_ALGORITHM_H
#define FTL_ALGORITHM_H

/*******************************************************************************************************
* 各种算法
*
*******************************************************************************************************/

/*******************************************************************************************************
* 压缩、解压缩 算法
*   LZW(Lempel Ziv Compression)算法 -- 
*     原理：提取原始图像数据中的不同图案，基于这些图案创建一个编译表，然后用编译表中的图案索引来
*            替代原始光栅数据中的相应图案，减少原始数据大小。类似调色板图像的实现原理。
*     编译表是根据原始图像数据动态创建的，解码时还要从已编码的数据中还原出原来的编译表。
*     数据流(CharStream,图像的光栅数据序列) 通过 编译表(String Table)计算，输出 编码流(CodeStream, 经过压缩运算的编码数据)
*******************************************************************************************************/



#endif //FTL_ALGORITHM_H