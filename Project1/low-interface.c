#include "low-interface.h"
#include <string.h>



void compress() {
    const int block_length = 1024;//ÿ��ѹ���Ŀ��С��һ��ѹ��1k�ֽ�

    char buffer[] = "hello world!1234567890hello world!1234567890678ty7t32148776416754327854763214354343231";   //ԭʼ����

    char dest[1024] = { 0 };

    //ѹ����Ϣ�ṹ��
    bz_stream* bzs = (bz_stream*)malloc(sizeof(bz_stream));
    bzs->opaque = NULL;
    bzs->bzalloc = NULL;
    bzs->bzfree = NULL;

    int bzerror;
    UINT64 nextIn = 0, nextOut = 0;

    //ѹ����ʼ��
    int err = BZ2_bzCompressInit(bzs,
        1,//blockSize100k ��Χ1-9���ֵ�Ĵ�С��100k*N�ֽ�
        2,  //verbosity  ��Χ0-4������Խ��monitoring/debugging���Խ�߳�
        30  //workFactor Ĭ��ֵ30����Χ0-250������
    );
    if (err == BZ_PARAM_ERROR) {
        printf("��������"); return NULL;
    }
    if (err == BZ_MEM_ERROR) {
        printf("û���㹻���ڴ����"); return NULL;
    }

    //ѹ��
    char* p = dest;
    int len = strlen(buffer);
    if (err == BZ_OK) {
        bzs->next_in = buffer;             //next_inָ��Ҫѹ��������
        bzs->avail_in = 30;               //һ�ζ�����
        bzs->next_out = p; //next_outָ����ѹ��������ݵĵ�ַ
        bzs->avail_out = 1024;              //һ�ηŶ���

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
    char buffer[] = "hello world!1234567890hello world!1234567890678ty7t32148776416754327854763214354343231";   //ԭʼ����
    int len = strlen(buffer);       //ԭʼ���ݳ���

    int destLen = len * 1.1 + 600;  //ѹ��������ݳ��ȣ�ԭʼ���鳤��*1.01+600�����Զ��һЩ�������ڴ���㡣
    char* dest = malloc(destLen);   //����ѹ����������
    memset(dest, 0, destLen);
    int err;        //������
    //ѹ��
    err = BZ2_bzBuffToBuffCompress(
        dest,       //Ŀ��buffer��ѹ��������ݣ�
        &destLen,   //Ŀ��buffer���ȡ������ڲ�ѹ����֮�󣬻�����Ϊʵ�ʵĳ��ȡ�
        buffer,     //ԭʼ����
        len,        //ԭʼ���ݳ���
        1,          //blockSize100k �ֵ��С����Χ1-9��ʵ�ʳߴ���100k*N
        4,          //verbosity ����̨���ѹ����Ϣ����Χ0-4������ԽС���Խ��
        30          //Ĭ��ֵ30�����ڲ������㷨���
    );
    //���ѹ�����
    char* p = dest;
    printf("\n----------\n");
    for (int i = 0; i < destLen; i++) {
        printf("%c", *p++);
    }
    printf("\n----------\n");

    //�ײ�ӿڽ�ѹ

    bz_stream strm;
    int ret;
    int verbosity = 0, small = 1;
    strm.bzalloc = NULL;
    strm.bzfree = NULL;
    strm.opaque = NULL;
    ret = BZ2_bzDecompressInit(&strm, verbosity, small);
    if (ret != BZ_OK) return ret;

    char decompress[1024] = { 0 };//�ṩһ��1k�Ļ�����
    int decomLen = 10;          //�������10�ֽڵĽ�ѹ����
    strm.next_in = dest;        //ѹ������
    strm.next_out = decompress; //��ѹ��ŵĵ�ַ
    strm.avail_in = destLen;    //ѹ�����ݵĳ���
    strm.avail_out = decomLen;  //��ѹ����

    err = BZ2_bzDecompress(&strm);
    BZ2_bzDecompressEnd(&strm);
    printf("\n----------\n");
    for (int i = 0; i < decomLen; i++)
    {
        printf("%c", decompress[i]);
    }
    printf("\n----------\n");


}