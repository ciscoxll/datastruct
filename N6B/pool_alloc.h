#ifndef POOL_ALLOC_H
#define POOL_ALLOC_H
//����һ���Զ����allocatorʵ��.
//��ͬ��Ĭ�ϵ�allocatorʹ�õͼ�����,ÿ��һ�����������
//ʹ�����ڴ�ط�ʽʵ��
//���ļ��޷����ڴ���8K�Ķ����allocate

template <class T>
class pool_alloc
{
    struct Link{Link* next;};       //�����ڴ水Link����.Link��ʵ��һ��ָ��,�൱��void*,����ָ��Ŀռ�Ĵ�С��sizeof(T)
    struct Chunk                    //�����ڴ水Chunk����,��С��8K....
    {
        enum{size=8*1024-sizeof(char)-sizeof(Chunk*)};       //һ�η���8k���ڴ�
        char mem[size];
        Chunk* next;
    };
    Chunk* chunks;
    Link* head;                             //head��ʼ��ָ����һ�����õ�λ��
    pool_alloc(pool_alloc&);          //����ϵͳ���ṩ�Ŀ������캯��
    void operator=(pool_alloc&);      //����ϵͳ���ṩ�ĸ�ֵ����

    void grow();                    //˽�г�Ա����ʵ�������
public:
    typedef T                value_type;
    typedef size_t           size_type;
    typedef ptrdiff_t        difference_tyre;
    typedef T*               pointer;
    typedef const T*         const_pointer;
    typedef T&               reference;
    typedef const T&         const_reference;

    pool_alloc():head(0),chunks(0){}
    pointer allocate(size_type n,const void* hint=0)
    {
        if(head==0) grow();
        Link* p=head;
        for(int i=0;i<n;i++)
            head=head->next;
        return reinterpret_cast<pointer>(p);
    }
    void deallocate(pointer p,size_type n)      //�д��޸�
    {
        if(n==1)
        {
            Link* b=reinterpret_cast<Link*>(p);
            b->next=head;
            head=b;
        }
    }
    ~pool_alloc()
    {
        Chunk* temp=chunks;
        while(chunks)
        {
            temp=chunks->next;
            delete chunks;
            chunks=temp;
        }
    }
    void construct(pointer p,const T& val){new (p) T(val);}
    void destroy(pointer p){p->~T();}
};
template <class T>
void pool_alloc<T>::grow()                 //����һ��Chunk.��chunksָ���µ�Chunk,����headָ���µĿ��ÿռ�
{
        Chunk* p=new Chunk;
        p->next=chunks;
        chunks=p;

        int temp=sizeof(T);
        int esize=sizeof(Chunk)/temp;
        char* start=chunks->mem;
        char* end=start+esize-1;

        for(char* p=start;p<end;p+=temp)       //��Link*����һ����
            reinterpret_cast<Link*>(p)->next=reinterpret_cast<Link*>(p+temp);
        reinterpret_cast<Link*>(end)->next=0;
        head=reinterpret_cast<Link*>(start);
}
#endif // POOL_ALLOC_H
