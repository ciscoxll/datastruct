#include <cstdlib>
#include <iostream>
#include <cmath>
#include <queue>
using namespace std;

int main(int argc, char *argv[])
{
    void shellsort(int *a,int n,int d);       //������a������������,����,��������n����,ÿ��
                                              //������dλ�� 
    int a[9]={91,6,85,15,92,35,30,22,39};
    cout<<"����ǰ..."<<endl;
    for(int i=0;i<9;i++)
        cout<<a[i]<<' '; 
    shellsort(a,9,2);
    cout<<'\n'<<"�����"<<endl;
    for(int i=0;i<9;i++)
        cout<<a[i]<<' ';                                         
    system("PAUSE");
    return EXIT_SUCCESS;
}
void shellsort(int* a,int n,int d)        
{
    queue<int> l[10];    //10����������װ0��9
    int i,j,k;
    for(i=0;i<d;i++)        //��ÿһλ
    {
        for(j=0;j<n;j++)        //��ÿһ���� 
        {
            l[a[j]/int(pow(10.0,i))%10].push(a[j]);       //����λ��ֵ�����ŵ���Ӧ�Ķ��� 
        }
        int m=0;
        for(k=0;k<10;k++)       //������,��ֵ�Żص�����a 
        {
            while(!l[k].empty())
            {
                a[m]=l[k].front();
                l[k].pop();
                m++;
            }
        }
    }
}
