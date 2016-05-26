#ifndef CCINFRA_CONCURRENCY_MULTITHREADWRAPPER_H_
#define CCINFRA_CONCURRENCY_MULTITHREADWRAPPER_H_

#include "ccinfra/base/Assertions.h"
#include "ccinfra/log/log.h"

template<typename T> 
struct ConstThreadElemVisitor
{
    virtual Status visit(const T&) = 0;
    virtual ~ConstThreadElemVisitor(){}
};

///////////////////////////////////////////////////////////
template <typename T, typename THREAD_INFO>
struct MultiThreadWrapper
{
    enum{ THREAD_NUM = THREAD_INFO::MAX_THREAD_NUM };

    T* operator->()
    {
        return const_cast<T*>(getPointer(THREAD_INFO::getCurrentId()));
    }

    const T* operator->() const
    {
        return getPointer(THREAD_INFO::getCurrentId());
    }

    T& operator*()
    {
        return getValue(THREAD_INFO::getCurrentId());
    }

    const T& operator*() const
    {
        return getValue(THREAD_INFO::getCurrentId());
    }

    T& operator[](unsigned int index)
    {
        return getValue(index);
    }

    const T& operator[](unsigned int index) const
    {
        return getValue(index);
    }

    Status visitAllThreadElems(ConstThreadElemVisitor<T>& visitor)
    {
        for(unsigned int index = 0; index < THREAD_NUM; index++)
        {
            CCINFRA_ASSERT_SUCC_CALL(visitor.visit(t[index]));
        }
        return CCINFRA_SUCCESS;
    }

private:
    const T* getPointer(unsigned int id) const
    {
        if(id < THREAD_NUM)
        {
            return &getValue(id);
        }
        
        DBG_LOG("MultiThreadWrapper: id(%d) overflow!", id);
        return 0;
    }

    const T& getValue(unsigned int id) const
    {
        return t[id];
    }

    T& getValue(unsigned int id)
    {
        return t[id];
    }
private:
    T  t[THREAD_NUM];
};

#endif
