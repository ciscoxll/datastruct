#ifndef HASH_H
#define HASH_H

#define KeyType int
#define ValueType int
static const float hash_full_ratio=0.5;    //��ϣ����
static const int hash_resize_factor=2;     //�����ٶ�,ÿ����������������ԭ����hash_resize_factor��
static const float hash_refresh_ratio=0.25;     //��DELETEռ�ı������ڴ����ǵ���refresh��DELETEԪ������ɾ��
typedef enum {EMPTY,OCCUPY,DELETE}status;
typedef struct cell     //key-value�ṹ
{
    KeyType key;
    ValueType value;
    status stat;
}cell;
typedef unsigned long (*hashfun_t) (KeyType);  //��ϣ����ָ��
typedef struct
{
    cell* addr;         //�׵�ַ
    int currentsize;        //��ǰhash���ڵ�Ԫ�ظ���
    int tombsize;        //���ΪDELETE�ĸ���
    int size;      //hash���С
    hashfun_t hashfunction;     //hash����ָ��
    int prime_offset;      //���ڲ��õ����ʵ�����������hash��Ĵ�С
}hash;
int createhash(hash* h,int defaultsize,hashfun_t hf);
void destroyhash(hash* h);
cell* find(hash* h,KeyType key);
int push(hash* h,KeyType key,ValueType value);
int pop(hash* h,KeyType key);
#endif // HASH_H
