#include <gtest/gtest.h>
#include <thread>
#include <algorithm>
#include "../SharedRingBuffer.h"

// Test producer-consumer synchronization
TEST(SharedRingBufferTest, ProducerConsumerSync)
{
    SharedRingBuffer<int, 5> buffer;

    std::thread producer([&]()
    {
        for (int i = 1; i <= 10; ++i)
        {
            buffer.produce(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });

    std::thread consumer([&]()
    {
        int sum = 0;
        for (int i = 1; i <= 10; ++i)
        {
            auto value = buffer.consume();
            if (value.has_value())
                sum += value.value();
        }
        EXPECT_EQ(sum, 55); // Sum of numbers from 1 to 10
    });

    producer.join();
    consumer.join();
}

// Test producer-consumer synchronization with no more data
TEST(SharedRingBufferTest, ProducerConsumerSyncEndData)
{
    SharedRingBuffer<int, 5> buffer;

    std::thread producer([&]()
    {
        for (int i = 1; i <= 5; ++i)
        {
            buffer.produce(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        buffer.setFinished();
    });

    std::thread consumer([&]()
    {
        int sum = 0;
        while (true)
        {
            auto value = buffer.consume();
            if (!value.has_value())
                break;
            sum += value.value();
        }
        EXPECT_EQ(sum, 15); // Sum of numbers from 1 to 5
    });

    producer.join();
    consumer.join();
}

// Test multiple producers and consumers
TEST(SharedRingBufferTest, MultipleProducersConsumers)
{
    SharedRingBuffer<int, 10> buffer;

    std::vector<std::thread> producers;
    for (int i = 0; i < 5; ++i)
    {
        producers.emplace_back([&]()
        {
            for (int j = 1; j <= 10; ++j)
            {
                buffer.produce(j);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
    }

    std::vector<std::thread> consumers;
    std::atomic<int> sum{ 0 };
    for (int i = 0; i < 5; ++i)
    {
        consumers.emplace_back([&]()
        {
            int localSum = 0;
            while (true)
            {
                auto value = buffer.consume();
                if (!value.has_value())
                    break;
                localSum += value.value();
            }
            sum += localSum;
        });
    }

    for (auto& producer : producers)
        producer.join();
    buffer.setFinished();
    for (auto& consumer : consumers)
        consumer.join();

    EXPECT_EQ(sum, 275); // Sum of numbers from 1 to 10 produced by each producer, total 5 producers
}

// Test with two producers (even and odd) and one consumer
TEST(SharedRingBufferTest, TwoProducersOneConsumer)
{
    // Create input array with 100 random integers
    std::vector<int> inputArray(100);
    std::generate(inputArray.begin(), inputArray.end(), []() { return rand() % 100; });

    SharedRingBuffer<int, 200> buffer;

    // Producer for even elements
    std::thread evenProducer([&]()
    {
        for (const auto& num : inputArray)
        {
            if (num % 2 == 0)
            {
                buffer.produce(num);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    });

    // Producer for odd elements
    std::thread oddProducer([&]()
    {
        for (const auto& num : inputArray)
        {
            if (num % 2 != 0)
            {
                buffer.produce(num);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    });

    // Consumer
    std::vector<int> resultArray;
    std::thread consumer([&]()
    {
        while (true)
        {
            auto value = buffer.consume();
            if (!value.has_value())
                break;
            resultArray.push_back(value.value());
        }
    });

    evenProducer.join();
    oddProducer.join();
    buffer.setFinished();
    consumer.join();

    // Ensure both arrays have the same length
    ASSERT_EQ(resultArray.size(), inputArray.size());

    // Sort both arrays
    std::sort(resultArray.begin(), resultArray.end());
    std::sort(inputArray.begin(), inputArray.end());

    // Ensure both arrays are equal after sorting
    EXPECT_EQ(resultArray, inputArray);
}