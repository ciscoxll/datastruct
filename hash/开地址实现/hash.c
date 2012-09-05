#include "hash.h"
#include <stdlib.h>
static unsigned long defaulthash(KeyType key)   //�ļ�������static
{
    unsigned long ret=0;
    int len=sizeof(key);
    const char* p=(const char*)&key;
    while(--len) ret = (ret << 5) - ret + *p++;
    return ret;
}
/*
 *prime_size���ص���һ������size����С����6i+1,6i-1...ͨ�����õ�
 *�������ֵ��������hash��Ĵ�С
 *ע�⣬hash�����Զ������ġ���ʼ����Ĵ�С����ֻ��һ���ο���С������һ������hash->size
 */
static int prime_size (int size, int *prime_offset)
{
  static const int primes[] = {
    13, 19, 29, 41, 59, 79, 107, 149, 197, 263, 347, 457, 599, 787, 1031,
    1361, 1777, 2333, 3037, 3967, 5167, 6719, 8737, 11369, 14783,
    19219, 24989, 32491, 42257, 54941, 71429, 92861, 120721, 156941,
    204047, 265271, 344857, 448321, 582821, 757693, 985003, 1280519,
    1664681, 2164111, 2813353, 3657361, 4754591, 6180989, 8035301,
    10445899, 13579681, 17653589, 22949669, 29834603, 38784989,
    50420551, 65546729, 85210757, 110774011, 144006217, 187208107,
    243370577, 316381771, 411296309, 534685237, 695090819, 903618083,
    1174703521, 1527114613, 1837299131, 2147483647
  };
  int tablesize=sizeof(primes)/sizeof(int);
  while((*prime_offset)<tablesize && primes[*prime_offset]<size)
      ++(*prime_offset);
  if(*prime_offset==tablesize)      //���hash��̫���޷������ˣ��򵥵ĵ���abort
      abort();
  return primes[*prime_offset];
}
int createhash(hash* h,int defaultsize,hashfun_t hf)
{
    h->prime_offset=0;
    h->size=prime_size(defaultsize,&h->prime_offset);
    if((h->addr=(cell*)malloc(sizeof(cell)*h->size))==0)
        return 1;
    h->hashfunction=hf?hf:defaulthash;
    h->currentsize=0;
    h->tombsize=0;
    int i;
    for(i=0;i<h->size;++i)          //��ʼ��Ϊ��
        (h->addr+i)->stat=EMPTY;
    return 0;
}
void destroyhash(hash* h)
{
    free(h->addr);
}
/*
 *�������̣���ԭ�����size��prime_size�䶯�ˡ��������˴洢�ռ䡣currentsize����,tombsize����
 */
static int grow(hash* h)
{
    int oldsize=h->size;
    h->size=prime_size(oldsize*hash_resize_factor,&h->prime_offset);    //����size...��������prime_size�����Զ�����prime_offset
    cell* oldaddr=h->addr;
    if((h->addr=(cell*)malloc(h->size*sizeof(cell)))==0)     //���´洢�ռ�
        return 1;
    int i;
    for(i=0;i<h->size;++i)
        (h->addr+i)->stat=EMPTY;    //��ʼ���±�
    h->currentsize=0;               //currentsize��������
    for(i=0;i<oldsize;++i)          //��ԭ���������ת���´洢�ռ�
    {
        if((oldaddr+i)->stat==OCCUPY)
           push(h,(oldaddr+i)->key,(oldaddr+i)->value);     //��������currentsize����
    }
    free(oldaddr);      //����ԭ���Ĵ洢�ռ�
    h->tombsize=0;         //tomb������0
    return 0;
}
static void refresh(hash* h)    //ˢ��hash���ѱ��ΪDELETE��Ԫ������ɾ��(Ū��EMPTY)
{
    int i;
    for(i=0;i<h->size;++i)
    {
        if((h->addr+i)->stat==DELETE)
           (h->addr+i)->stat=EMPTY;
        if((h->addr+i)->stat==OCCUPY && h->hashfunction((h->addr+i)->key)!=i)
        {
            (h->addr+i)->stat=EMPTY;
            --h->currentsize;           //����Ҫע���һ����push�����currentsize����
            push(h,(h->addr+i)->key,(h->addr+i)->value);
        }
    }
    h->tombsize=0;
}
/*
 *���Һ���,�ɹ����ؽ��ָ�룬ʧ�ܷ��ز���λ��ָ��
 *��������Ǻ��溯���Ļ��������룬ɾ�������ȵ�����������ҵ�λ��
 *     EMPTY ����cell*  û�ҵ������ز���λ��
 *     OCCUPY  key��ȡ��ҵ������ؽ��λ��
 *             key���ȡ���������λ��
 *     DELETE  ������������������λ��
 */
cell* find(hash* h,KeyType key)
{
    unsigned long locate=h->hashfunction(key)%h->size;
    cell* ret=h->addr+locate;
    while(ret->stat==DELETE || (ret->stat==OCCUPY&&ret->key!=key))
        ret=h->addr+(++locate)%h->size;
    return ret;     //����Ҫô��һ����λ�ã�Ҫô�ҵ���
}
int push(hash* h,KeyType key,ValueType value)
{
    cell* p=find(h,key);    //�ҵ�����λ��
    if(p->stat==EMPTY)      //�����µ�key-value��
    {
        p->key=key;
        p->value=value;
        p->stat=OCCUPY;
        ++h->currentsize;
    }
    else    //����ԭ��key��Ӧ��value
    {
        p->value=value;
    }
    /*
     *���Ԫ�ع��࣬hash�����ܻή�͡�ͨ�����·������ռ䣬ʹ��ϣ������һ����Χ��
     *�����Ϳ��Ա�֤������.�����������hash��
     */
    if(h->currentsize>h->size*hash_full_ratio)
        return grow(h);
    return 0;
}
/*
 *ɾ�������Ǳ�Ƿ�����Ϊ����ַ����ϣ������ɾ����������⡣�ж��ַ��������һ������������ɾ��������ÿ��ɾ������Ҫˢ��
 *��һ���Ǳ�Ƿ��������Ҳ��������⣬��DELETE���λ�õĿռ����޷��õġ�
 *�������ɾ��֮��ͻ��кܶ�"���ռ�"����Ҫ����ĳ�ַ���ÿ��һ��ʱ��������ɾ��.refresh��������������µ�
 *��һ�ַ�����������ɾ��ʱ����ʱ�䳤���ڶ��ַ���ɾ���ܿ졣
 *�����õڶ��ַ���
 */
int pop(hash* h,KeyType key)
{
    cell* p=find(h,key);
    if(p->stat==EMPTY)      //key�����ڣ��޷�pop
        return -1;
    p->stat=DELETE;
    --h->currentsize;
    ++h->tombsize;
    if(h->tombsize>hash_refresh_ratio*h->size)    //������ռ���࣬��ˢ�½�������ɾ��
        refresh(h);
    return 0;
}


