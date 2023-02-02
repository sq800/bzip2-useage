#include "high_interface.h"


void high_test() {
    FILE* f;        //�������մ��ļ��ľ��
    BZFILE* b;      //�������տ⺯����BZ2_bzReadOpen�򿪵��ļ�
    char buf[1024]; //�������տ⺯����BZ2_bzRead��ȡ�����ֽ�
    int bzerror;    //�������տ��׳��ĳɹ�/�������
    int nOpen;      //��������BZ2_bzRead��ȡ�����ֽڵĳ���
    char fileName[] = "bz2/gbk.txt.bz2";
    int stop = 1;   //ֹͣ��ȡ�ļ����ı�־

    f = fopen(fileName, "rb"); // ����2����ֻ��
    if (f == NULL) {
        printf("�ļ�������");
        return;
    }
    b = BZ2_bzReadOpen(
        &bzerror, // ������
        f,        // �ļ�ָ��
        0,        // ��Χ0-4������Խ��monitoring/debugging���Խ�߳�
        1,        // ����Ϊ��Ϊ0��������ʹ����һ��ѹ���㷨�������㷨��һ����ڴ����ģ��������ٶȱ���
        NULL,     // ��ʹ��
        0         // ��ʹ��
    );
    while (stop) {
        if (bzerror == BZ_OK) // �ɹ���
        {
            int num = sizeof(buf);
            nOpen = BZ2_bzRead(&bzerror, b, buf, num);
            if (bzerror == BZ_OK) { //��ȡ�ɹ�
                for (int i = 0; i < sizeof(buf); i++) {     //���ַ���ʽѭ����ӡ���������ֽ�
                    printf("%c", buf[i]);
                }
            }
            else if (bzerror == BZ_STREAM_END) {    //�������ļ��������ֲ��г�Ϊ�߼�������ĩβ
                for (int i = 0; i < nOpen; i++) {   //���ַ���ʽѭ����ӡ���������ֽ�
                    printf("%c", buf[i]);
                }
                stop = 0;   //�˳�ѭ����ȡ
            }
            else if (bzerror == BZ_MEM_ERROR)
            {
                printf("û���㹻���ڴ����");
            }
            else {  //�������
                printf("��ȡ��������,�����룺%d", bzerror);
            }
        }
        else // ��ʧ�������ر��ļ�
        {
            BZ2_bzReadClose(&bzerror, b);
        }
    }
    //�ر��ļ�
    BZ2_bzReadClose(&bzerror, b);
    fclose(f);
}