#include "utility-interface.h"
#include <string.h>

void utility_test() {

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
        printf("%10x ", *p++);
    }
    printf("\n----------\n");

    //��ѹ
    char deCompress[1000] = { 0 };      //���ս�ѹ�������ݡ���ѹ����Ҫ���ڴ��С�����ǲ�֪���ġ������ȶ���Ϊ1000
    unsigned int len2 = 1000;
    err = BZ2_bzBuffToBuffDecompress(deCompress, &len2, dest, destLen, 0, 4); //������ѹ������

    //�����ѹ���
    printf("\n----------\n");
    for (int i = 0; i < len2; i++) {
        printf("%c", deCompress[i]);
    }
    printf("\n----------\n");

}
