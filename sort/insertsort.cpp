#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    void insertsort(int* array,int n);
    int a[10]={49,38,65,97,76,13,27,49,55,4};
    insertsort(a,10);
    for(int i=0;i<10;i++)
        cout<<a[i]<<' ';
    cout<<endl;
    system("PAUSE");
    return EXIT_SUCCESS;
}
void insertsort(int* array,int n)
{
    for(int i=1;i<n;i++)        //�ӵ�2����ʼ...ÿ�β�һ�� 
    {
        //��0��i-1��a[i]�Ĳ���λ��,�ö��ֲ��ҷ�ʽʵ�� 
        int low=0,high=i;       //ע������high���Ͻ�...��ô������Ŀ�ĵ� 
        while(low<high)
        {
            if(array[i]<array[(low+high)/2])
            {
                high=(low+high)/2-1;
            }
            else //if(array[i]<array[(low+high)/2])
            {
                low=(low+high)/2+1;
            }           
        } //�������֮��low>=high��,������ 
        cout<<"����λ��"<<low<<endl;
        cout<<high<<endl;
        int temp=array[i];
        for(int j=i;j>low;j--)
        {
            array[j]=array[j-1];
        }
        array[low]=temp;
    }
}
