#ifndef SPSC_H
#define SPSC_H

#include <vector>
#include <atomic>

/**
 * @class SPSCQueue
 * @brief Single-producer single-consumer lock-free queue.
 * @tparam T Type of elements stored in the queue.
 */
template <typename T>
class SPSCQueue
{
public:
    SPSCQueue(size_t capacity);
    ~SPSCQueue();

    /**
     * @brief Enqueues an item into the queue.
     * @param item The item to enqueue.
     * @return True if the item was enqueued, false if the queue is full.
     */
    bool enqueue(const T &item);

    /**
     * @brief Dequeues an item from the queue.
     * @param item Reference to store the dequeued item.
     * @return True if an item was dequeued, false if the queue is empty.
     */
    bool dequeue(T &item);
    bool empty();

private:
    std::vector<T> buffer;
    size_t capacity;
    std::atomic<size_t> head;
    std::atomic<size_t> tail;
};

#include "spsc_impl.h"

#endif