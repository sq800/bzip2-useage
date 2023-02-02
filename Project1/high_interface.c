#include "high_interface.h"


void high_test() {
    FILE* f;        //用来接收打开文件的句柄
    BZFILE* b;      //用来接收库函数：BZ2_bzReadOpen打开的文件
    char buf[1024]; //用来接收库函数：BZ2_bzRead读取出的字节
    int bzerror;    //用来接收库抛出的成功/错误代码
    int nOpen;      //用来接收BZ2_bzRead读取出的字节的长度
    char fileName[] = "bz2/gbk.txt.bz2";
    int stop = 1;   //停止读取文件流的标志

    f = fopen(fileName, "rb"); // 设置2进制只读
    if (f == NULL) {
        printf("文件不存在");
        return;
    }
    b = BZ2_bzReadOpen(
        &bzerror, // 错误码
        f,        // 文件指针
        0,        // 范围0-4，数字越大，monitoring/debugging输出越冗长
        1,        // 设置为不为0的数，则使用另一种压缩算法，这种算法少一半的内存消耗，代价是速度变慢
        NULL,     // 不使用
        0         // 不使用
    );
    while (stop) {
        if (bzerror == BZ_OK) // 成功打开
        {
            int num = sizeof(buf);
            nOpen = BZ2_bzRead(&bzerror, b, buf, num);
            if (bzerror == BZ_OK) { //读取成功
                for (int i = 0; i < sizeof(buf); i++) {     //以字符形式循环打印出读到的字节
                    printf("%c", buf[i]);
                }
            }
            else if (bzerror == BZ_STREAM_END) {    //读到了文件流（库手册中称为逻辑流）的末尾
                for (int i = 0; i < nOpen; i++) {   //以字符形式循环打印出读到的字节
                    printf("%c", buf[i]);
                }
                stop = 0;   //退出循环读取
            }
            else if (bzerror == BZ_MEM_ERROR)
            {
                printf("没有足够的内存可用");
            }
            else {  //其他情况
                printf("读取遇到错误,错误码：%d", bzerror);
            }
        }
        else // 打开失败立即关闭文件
        {
            BZ2_bzReadClose(&bzerror, b);
        }
    }
    //关闭文件
    BZ2_bzReadClose(&bzerror, b);
    fclose(f);
}