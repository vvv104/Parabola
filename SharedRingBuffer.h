#pragma once
#include <array>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <optional>

/// @brief This class implements queue-like container of fixed length with multi-threading syncronization of reading and writing 
///        operations. It provides interface in producer-consumer paradigm supporting many producers and many consumers
/// @tparam T - Type of elements in the container
/// @tparam N - static size of the container
template<class T, std::size_t N>
class SharedRingBuffer
{
private:
    // container of data
    std::array<T, N> data_;
    // position of first item (makes sense if count_ > 0)
    std::size_t head_ = 0;
    // position of 'beyond the last' item
    std::size_t tail_ = 0;
    // number of really used items in the ring buffer
    std::size_t count_ = 0;
    // cond variables to signal to the opposite side that we can get (notEmpty_) or we put (notFull_) the value into the container
    std::condition_variable notFull_, notEmpty_;
    // mutex serving cond variables
    std::mutex mtx_;
    // flag to stop flowing the data through the buffer
    std::atomic<bool> endData_{ false };

public:
    /// @brief Puts a value into the container and notifies one of consumers about possibility to take it
    /// @param value 
    void produce(const T& value)
    {
        // don't allow to put more elements when we are stopping
        if (endData_)
            return;
            
        std::unique_lock<std::mutex> lock(mtx_);
        // block insertion (wait for) one value is extracted from the queue
        notFull_.wait(lock, [this]() { return N != count_; });
        // put new value to buffer and move tail position forward
        data_[tail_++] = value;
        // make circular indexing tail is always behind head but we jump over 0
        tail_ %= N;
        // incremet the numver of really used items
        ++count_; 
        // notify that we have at least one value available for reading
        notEmpty_.notify_one();
    }

    /// @brief Extracts a value from the container and notifies one of producers that there is a space available for insertion
    /// @return If return value is nullopt then the will be no data
    std::optional<T> consume()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        // wait until some element is put into the queue of finish flag is set
        notEmpty_.wait(lock, [this]() { return 0 != count_ || endData_; });

        // exit reading from queue by returning nullopt
        if (0 == count_ && endData_)
            return std::nullopt;

        // get value from buffer and move head position forward
        T value = data_[head_++];
        // make circular indexing
        head_ %= N;
        // decrease number of elements
        --count_;
        // notify that there is a place to put a new value
        notFull_.notify_one();
        return value;
    }

    /// @brief Set a queue to status of 'no more data'
    void setFinished()
    {
        endData_ = true;
        notEmpty_.notify_all();
    }
};