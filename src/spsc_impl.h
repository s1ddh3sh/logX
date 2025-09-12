#include "spsc.h"
template <typename T>
SPSCQueue<T>::SPSCQueue(size_t capacity) : buffer(capacity), capacity(capacity), head(0), tail(0) {}

template <typename T>
SPSCQueue<T>::~SPSCQueue() {}

template <typename T>
bool SPSCQueue<T>::enqueue(const T &item)
{
    size_t h = head.load(std::memory_order_relaxed);
    size_t next_h = (h + 1) % capacity;
    if (next_h == tail.load(std::memory_order_acquire))
    {
        return false; // queue full
    }
    buffer[h] = item;
    head.store(next_h, std::memory_order_release);
    return true;
}

template <typename T>
bool SPSCQueue<T>::dequeue(T &item)
{
    size_t t = tail.load(std::memory_order_relaxed);
    if (t == head.load(std::memory_order_acquire))
    {
        return false; // queue empty
    }
    item = buffer[t];
    tail.store((t + 1) % capacity, std::memory_order_release);
    return true;
}

template <typename T>
bool SPSCQueue<T>::empty()
{
    size_t t = tail.load(std::memory_order_acquire);
    return (t == head.load(std::memory_order_acquire));
}