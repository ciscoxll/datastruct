#include "hash.h"
#include <stdlib.h>
/*
 *����һ����ʽʵ�ֵĹ�ϣ�������ж�����table�����ڴ��ָ��buff����ָ��
 *buff�������������ݣ��Ծ�̬����ʽ��֯��ָ�����int��.
 *����һ��hash���ӣ���table��̫��ʱ�����·���table����Ҳ��ʹbuff���ط���
 *ʹ�þ�̬����ԭ����:
 *��̬�����ط���ռ�ʱָ���ϵ����ʧЧ�����Է���ص���realloc����,������buff���Ĳ����ͼ�һ��
 */
//buff��Ԫ�صı�ǣ�EMPTY��ʾ�գ����Է������ݣ�REAR��ʾ��λ������β
#define EMPTY -1
#define REAR -2
//�ṩһ��Ĭ�ϵĹ�ϣ����
static unsigned long defaulthash(Keytype key)
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
int createhash(hash* h,int size,hash_t hf)
{
    //table���ֵĳ�ʼ��
    h->prime_offset=0;
    h->totalsize=prime_size(size,&h->prime_offset);
    h->table=(int*)malloc(h->totalsize*sizeof(int));    //�����ռ�
    if(h->table==NULL)
        return -1;
    int i;
    for(i=0;i<h->totalsize;++i)
        h->table[i]=EMPTY;
    h->currentsize=0;
    h->hf=hf?hf:defaulthash;
    //buff�ĳ�ʼ��
    h->buffsize=h->totalsize;
    h->buff=(cell*)malloc(h->buffsize*sizeof(cell));
    if(h->buff==NULL)
        return -1;
    h->buffcurrent=0;
    return 0;
}
void destroyhash(hash* h)
{
    free(h->table);
    free(h->buff);
}
//����ϣ��̫��ʱ���ô˺���
static int rehash(hash* h)
{
    //���·���table��
    h->totalsize=prime_size(h->totalsize*hash_increase_factor,&h->prime_offset);
    free(h->table);
    h->table=(int*)malloc(h->totalsize*sizeof(int));
    if(h->table==NULL)
        return -1;
    int i;
    for(i=0;i<h->totalsize;++i)
        h->table[i]=EMPTY;
    h->currentsize=0;
    //���·���buff��
    int oldsize=h->buffcurrent;
    cell* oldbuff=h->buff;
    h->buff=(cell*)malloc(h->buffsize*sizeof(cell));
    h->buffcurrent=0;
    for(i=0;i<oldsize;++i)
    {
        if(oldbuff[i].next!=EMPTY)
        {
            insert(h,oldbuff[i].key,oldbuff[i].data);
        }
    }
    free(oldbuff);
    return 0;
}
int insert(hash* h,Keytype key,Elemtype value)
{
    int pos=(h->hf(key))%h->totalsize;
    if(h->table[pos]==EMPTY)    //hash�ɹ�
    {
        h->table[pos]=h->buffcurrent;
        h->buff[h->buffcurrent].data=value;     //����
        h->buff[h->buffcurrent].key=key;
        h->buff[h->buffcurrent].next=REAR;
        ++h->buffcurrent;
        ++h->currentsize;
    }
    else    //��ͻ��
    {
        int p=h->table[pos];
        while(h->buff[p].key!=key && h->buff[p].next!=REAR)
        {             
            p=h->buff[p].next;
        }
        if(h->buff[p].key==key)      //���key�ڱ����Ѿ�������
        {
            h->buff[p].data=value;
            return -1;
        }
        h->buff[p].next=h->buffcurrent;
        p=h->buff[p].next;
        h->buff[p].next=REAR;
        h->buff[p].key=key;
        h->buff[p].data=value;
        ++h->buffcurrent;
        ++h->currentsize;
    }
    //���ܷ������治�����·���
    if(h->buffcurrent==h->buffsize)
    {
        h->buffsize*=2;
        h->buff=realloc(h->buff,h->buffsize*sizeof(cell));
        if(h->buff==NULL)
            abort();
    }
    //�������ڹ�ϣ��̫����rehash
    if(h->totalsize*hash_factor < h->currentsize)
    {
        rehash(h);
    }
    return 0;
}
//�ɹ�����0,��value����ֵ��ʧ�ܷ���-1
int search(hash* h,Keytype key,Elemtype* value)
{
    int pos=(h->hf(key))%h->totalsize;
    int p=h->table[pos];
    if(p==EMPTY)    //key��table���в�����
        return -1;
    while(p!=REAR)
    {
        if(h->buff[p].key==key)
        {
            *value=h->buff[p].data;  //��
            return 0;
        }
        p=h->buff[p].next;
    }
    return -1;
}
/*
 *����ģ�C���Ծ�Ȼ��֧�ֺ��������أ�remove��������stdio�����һ��remove��ͻ�ˣ���ʱ����remove1
 *���������������ɾ��key���ڵ�����key�����ڣ�����-1�����
 *ɾ�������������ɾ����ֻ�ǰ�ָ��������һ���������ͻ��в����޷�ʹ�õĿռ䣮
 *�������õ��ģ�rehash�������ؽ�buff��,��ʱ�ͻ�ѿռ������
 */
int remove1(hash* h,Keytype key)
{
    int pos=(h->hf(key))%h->totalsize;
    if(h->table[pos]==EMPTY)
        return -1;
    int p=h->table[pos];
    if(h->buff[p].key==key)         //��Ϊ�ǲ�����ͷ���������ɾ�����Ǳ�ͷ���͵����⴦��
    {
        h->table[pos]=h->buff[p].next;
        h->buff[p].next=EMPTY;
        --h->currentsize;
        return 0;
    }
    int q=h->buff[p].next;        //pָ����Ǵ�ɾ���ǰһ��� qָ���ɾ���
    while( q!=REAR && h->buff[q].key!=key)       //ɾ����ͷ���
    {
        p=q;
        q=h->buff[q].next;
    }
    if(q==REAR)     //û�ҵ�
        return -1;
    h->buff[p].next=h->buff[q].next;
    h->buff[q].next=EMPTY;
    --h->currentsize;
    return 0;
}











