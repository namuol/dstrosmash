#ifndef _VECTOR_H
#define _VECTOR_H

#include <cstdlib>

/*
 * Something like std::vector since the std:: collections seem to 
 *  misbehave when it comes to memory allocation.
 */

template<class T>
class item {
public:
    bool active;
    T data;
};

template<class T, unsigned int N>
class vector {
    unsigned int _size;
    unsigned int first_avail;
    item<T> items[N];

public:
    // Returns the current number of active elements in the vector.
    inline unsigned int size() { return _size; }
    // Returns the maximum number of elements in the vector.
    inline unsigned int capacity() { return N; }

    vector()
    : _size(0), first_avail(0)
    {
        for(unsigned int i=0; i<N; ++i)
            items[i].active=false;
    }

    // Note: non-active items will _not_ be copied.
    vector(const vector<T,N>& other)
    {
        _size = 0;
        for(unsigned int i=0;i<N;++i)
        {
            items[i].active = other.items[i].active;
            if(items[i].active)
            {
                items[i].data = other.items[i].data;
                ++_size;
            }
        }
        first_avail = other.first_avail;
    }


    // Returns -1 if the item couldn't be added due to limited space.
    int add(const T& data)
    {
        unsigned int i;
        for(i=0; i<N && items[i].active; ++i);
        if(i >= N) return -1;
        
        items[i].data = data;
        items[i].active = true;

        ++_size;

        return i;
    }

    
    // Simply re-activates the first available item.
    // It is your responsibility to initialize its data afterwards.
    // Returns -1 if the item couldn't be added due to limited space.
    int add()
    {
        unsigned int i;
        for(i=0; i<N && items[i].active; ++i);
        if(i >= N) return -1;
        
        items[i].active = true;

        ++_size;

        return i;
    }

    void rem(unsigned int index)
    {
        if(items[index].active) 
        {
            items[index].active = false;
            --_size;
        }
    }

    void rem_deinit(unsigned int index)
    {
        if(items[index].active) 
        {
            items[index].active = false;

            items[index].data.deinit();
            --_size;
        }
    }

    void clear()
    {
        for(unsigned int i=0; _size>0 && i<N; ++i)
        {
            if(items[i].active) 
            {
                items[i].active = false;
                --_size;
            }
        }
    }

    void deinit_clear()
    {
        for(unsigned int i=0; _size>0 && i<N; ++i)
        {
            if(items[i].active) 
            {
                items[i].data.deinit();
                items[i].active = false;
                --_size;
            }
        }
    }

    bool active(unsigned int index) const
    {
        return index < N && index >= 0 && items[index].active;
    }

    T& operator[](unsigned int index)
    {
        return items[index%N].data;
    }
};

#endif
