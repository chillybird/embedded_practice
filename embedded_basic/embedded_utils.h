#ifndef __EMBEDDED_UTILS_H__
#define __EMBEDDED_UTILS_H__

// 判断处理器是大端还是小端
int isBigEndian() {
    /**
     * 大端模式:低位字节存在高地址上,高位字节存在低地址上。
     * 小端模式:高位字节存在高地址上,低位字节存在低地址上。
    */
    short num = 0x0102; // 一个16位的示例数据 0x0102
    char *bytes = (char *)&num; // 将 short 类型的指针转换为 char* 类型的指针
    return bytes[0] == 0x01; // 判断字节序列的第一个字节是否为 0x01
}



#endif // EMBEDDED_UTILS_H__