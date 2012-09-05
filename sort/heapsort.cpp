#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    void adjustHeap(int* array,int s,int m); //s�ǶѶ�λ��...s���������Ƕ���.����ʹ����array�ɶ� 
    void heapsort(int* array,int n);            //�������㷨 
    int a[8]={49,38,65,97,76,13,27,49};
    heapsort(a,8);
    cout<<"�����"<<endl;
    for(int i=0;i<8;i++)
        cout<<a[i]<<' ';
    system("PAUSE");
    return EXIT_SUCCESS;
}
void adjustHeap(int* array,int s,int m)     //���� 
{
    int temp=array[s];
    int pos=2*s+1;
    while(pos<=m)
    {
        if(pos+1<=m&&array[pos]<array[pos+1])
            pos=pos+1;
        if(temp<array[pos])
        {
            array[s]=array[pos];
            s=pos;
            pos=2*pos+1;
        }
        else 
            break;
    }
    array[s]=temp;
}
void heapsort(int* array,int n)     
{
    int i;
    for(i=n/2;i>=0;i--)      //���ѹ��� 
    {
        adjustHeap(array,i,n);
    }
    for(i=n-1;i>0;i--)
    {
        int temp=array[0];      //�Ѷ������һ��Ԫ�ؽ��� 
        array[0]=array[i];
        array[i]=temp;
        adjustHeap(array,0,i-1);                        //���µ����� 
    }
}
