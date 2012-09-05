#include "heap.h"
//�����
//i�������������ѳɶѣ�����ʹi��n�ɶ�
void adjust(T heap[],int i,int n)
{
    T tmp=heap[i];
    int j=2*i+1;
    while(j<n)
    {
        if(j+1<n&&heap[j]<heap[j+1])   //j��i���������нϴ��һ��
            ++j;
        if(heap[j]>tmp)     //���heap[j]��tmp��,��heap[j]���ϸ�
        {
            heap[i]=heap[j];
            i=j;
            j=j*2+1;
        }
        else
            break;
    }
    heap[i]=tmp;        //����tmp�Ż�
}
//����
void createheap(T heap[],int n)
{
    int i;
    for(i=(n-2)/2;i>=0;--i)
    {
        adjust(heap,i,n);
    }
}
//������
void heapsort(T heap[],int n)
{
    createheap(heap,n);
    int i;
    for(i=n-1;i>0;--i)
    {   T tmp=heap[0];      //�Ѷ��ó�ȥ
        heap[0]=heap[i];
        heap[i]=tmp;
        adjust(heap,0,i);     //����
    }
}
