#include "utility-interface.h"
#include <string.h>

void utility_test() {

    char buffer[] = "hello world!1234567890hello world!1234567890678ty7t32148776416754327854763214354343231";   //原始数据
    int len = strlen(buffer);       //原始数据长度

    int destLen = len * 1.1 + 600;  //压缩后的数据长度，原始数组长度*1.01+600，可以多给一些，保持内存充足。
    char* dest = malloc(destLen);   //接收压缩过的数据
    memset(dest, 0, destLen);
    int err;        //错误码
    //压缩
    err = BZ2_bzBuffToBuffCompress(
        dest,       //目标buffer（压缩后的数据）
        &destLen,   //目标buffer长度。函数内部压缩完之后，会设置为实际的长度。
        buffer,     //原始数据
        len,        //原始数据长度
        1,          //blockSize100k 字典大小，范围1-9，实际尺寸是100k*N
        4,          //verbosity 控制台输出压缩信息，范围0-4，数字越小输出越少
        30          //默认值30，库内部排序算法相关
    );
    //输出压缩结果
    char* p = dest;
    printf("\n----------\n");
    for (int i = 0; i < destLen; i++) {
        printf("%10x ", *p++);
    }
    printf("\n----------\n");

    //解压
    char deCompress[1000] = { 0 };      //接收解压缩的数据。解压缩需要的内存大小事先是不知道的。这里先定义为1000
    unsigned int len2 = 1000;
    err = BZ2_bzBuffToBuffDecompress(deCompress, &len2, dest, destLen, 0, 4); //参数与压缩类似

    //输出解压结果
    printf("\n----------\n");
    for (int i = 0; i < len2; i++) {
        printf("%c", deCompress[i]);
    }
    printf("\n----------\n");

}
