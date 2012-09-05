#ifndef ALLOCATOR_H
#define ALLOCATOR_H

//����ļ���stl�ж�Ӧ��SGI��allocator.SGI�е�allocator�̳���___glibcxx_base_allocator<_Tp>
//___glibcxx_base_allocator<_Tp>ʵ�������ļ�new_allocator.h

#include <new>      //Ϊ�˵���placement new
#include <cstddef>  //����size_t,ptrdiff_t����

namespace N6B
{
    template <class T>
    inline T* _allocate(ptrdiff_t size,T*)
    {
        T* tmp=static_cast<T*>(::operator new(static_cast<size_t>(size*sizeof(T))));    //����rawmemory
        return tmp;
    }
    template <class T>
    inline void _construct(T* p,const T& value)     //��ʼ���ڴ�
    {
        new(p) T(value);      //placement new
    }
    template <class T>
    inline void _deallocate(T* buff)       //�����ڴ�ռ�
    {
        ::operator delete(buff);
    }
    template <class T>
    inline void _destroy(T* p)              //����
    {
        p->~T();
    }
    template <class T>
    class allocator
    {
    public:
        //������������
        typedef T                   value_type;
        typedef T*                  pointer;
        typedef const T*            const_pointer;
        typedef T&                  reference;
        typedef const T&            const_reference;
        typedef size_t              size_type;
        typedef ptrdiff_t           difference_type;

        //������岻֪���Ǹ�ʲô�õ�
        template<typename _Tp1>
        struct rebind
        { typedef allocator<_Tp1> other; };

        pointer allocate(size_t n,const void* = 0)  //�ڶ���������ʵ��������.�����붨��,Ϊ��ƥ�����
        {
            return _allocate(difference_type(n),pointer(0));
        }
        void deallocate(pointer p,size_t n)
        {
            _deallocate(p);
        }
        void construct(pointer p,const T& value)
        {
            _construct(p,value);
        }
        void destroy(pointer p)
        {
            _destroy(p);
        }
        pointer address(reference x)
        {
            return static_cast<pointer>(&x);
        }
        const_pointer address(const_reference x)
        {
            return static_cast<const_pointer>(&x);
        }


    };
}
#endif // ALLOCATOR_H
