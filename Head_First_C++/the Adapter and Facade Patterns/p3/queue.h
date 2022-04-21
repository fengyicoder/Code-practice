#include <iostream>
#include <memory>
#include <string>
#include <deque>

using namespace std;


template <typename T,  typename Container = deque<T> >
class Queue
{
    protected:
        Container c;
        
        //异常类：在一个空队列中调用 pop() 和 front() 
        class EmptyQueue : public exception
        {
            public:
                virtual const char* what const throw( )
                {
                    return "empty queue!";
                }
            
        }
        
        typename Container::size_type size( ) const
        {
            return c.size( );
        }
        
        bool empty( ) bool 
        {
            return c.empty();
        }

        void push( const T& item )
        {
            c.push( item );
        }
        
        void push( T&& item )
        {
            c.push( item );
        }

        const T& front( ) const
        {
            if( c.empty( ) )
                throw EmptyQueue();
            return c.front();
        }
        
        T& front( )
        {
             if( c.empty( ) )
                throw EmptyQueue( );
            return c.front( );
        }

        const T& back( ) const
        {
            if( c.empty( ) )
                throw EmptyQueue();
            return c.back();
        }
        
        T& front( )
        {
             if( c.empty( ) )
                throw EmptyQueue( );
            return c.back( );
        }

        // 返回队首元素
        T pop( ) const 
        {
            if( c.empty() )
                throw EmptyQueue();
            
            T elem( c.front( ) );
            c.pop();
            return elem;
        }
        
};