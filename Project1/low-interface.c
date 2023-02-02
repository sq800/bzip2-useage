#include "low-interface.h"
#include <string.h>



void compress() {
    const int block_length = 1024;//每次压缩的块大小，一次压缩1k字节

    char buffer[] = "hello world!1234567890hello world!1234567890678ty7t32148776416754327854763214354343231";   //原始数据

    char dest[1024] = { 0 };

    //压缩信息结构体
    bz_stream* bzs = (bz_stream*)malloc(sizeof(bz_stream));
    bzs->opaque = NULL;
    bzs->bzalloc = NULL;
    bzs->bzfree = NULL;

    int bzerror;
    UINT64 nextIn = 0, nextOut = 0;

    //压缩初始化
    int err = BZ2_bzCompressInit(bzs,
        1,//blockSize100k 范围1-9，字典的大小，100k*N字节
        2,  //verbosity  范围0-4，数字越大，monitoring/debugging输出越冗长
        30  //workFactor 默认值30，范围0-250（含）
    );
    if (err == BZ_PARAM_ERROR) {
        printf("参数错误"); return NULL;
    }
    if (err == BZ_MEM_ERROR) {
        printf("没有足够的内存可用"); return NULL;
    }

    //压缩
    char* p = dest;
    int len = strlen(buffer);
    if (err == BZ_OK) {
        bzs->next_in = buffer;             //next_in指向要压缩的数据
        bzs->avail_in = 30;               //一次读多少
        bzs->next_out = p; //next_out指向存放压缩后的数据的地址
        bzs->avail_out = 1024;              //一次放多少

        bzerror = BZ2_bzCompress(bzs, BZ_RUN);
    }
    BZ2_bzCompressEnd(bzs);
    printf("\n----------\n");
    for (int i = 0; i < bzs->total_out_lo32; i++) {
        +
            printf("%10x ", *p++);
    }
    printf("\n----------\n");

}
void test() {
    //compress();
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
        printf("%c", *p++);
    }
    printf("\n----------\n");

    //底层接口解压

    bz_stream strm;
    int ret;
    int verbosity = 0, small = 1;
    strm.bzalloc = NULL;
    strm.bzfree = NULL;
    strm.opaque = NULL;
    ret = BZ2_bzDecompressInit(&strm, verbosity, small);
    if (ret != BZ_OK) return ret;

    char decompress[1024] = { 0 };//提供一个1k的缓存区
    int decomLen = 10;          //本次输出10字节的解压数据
    strm.next_in = dest;        //压缩数据
    strm.next_out = decompress; //解压存放的地址
    strm.avail_in = destLen;    //压缩数据的长度
    strm.avail_out = decomLen;  //解压多少

    err = BZ2_bzDecompress(&strm);
    BZ2_bzDecompressEnd(&strm);
    printf("\n----------\n");
    for (int i = 0; i < decomLen; i++)
    {
        printf("%c", decompress[i]);
    }
    printf("\n----------\n");


}