#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include "ArrayView.h"
#include "Parabola.h"
#include "ParabolaDefinitionReader.h"
#include "SharedRingBuffer.h"

int main(int argc, char* argv[])
{
    // Runtime constraints
    constexpr size_t inputBufferSize = 4;
    constexpr size_t outputBufferSize = 4;
    constexpr size_t calculationTreadCount = 4;

    // Create an array view over command line parameters excluding the 1st which is the name of the module
    ArrayView<char*> args(argv + 1, argc - 1);

    // Conctruct reader of 'Parabola' objects from input array
    ParabolaDefinitionReader provider(args);

    SharedRingBuffer<Parabola::Definition, inputBufferSize> input;
    SharedRingBuffer<Parabola::Result, outputBufferSize> output;

    // Thread function for reader thread, it gets objects from reader while they are available and puts them to input buffer
    auto readerProc = [&input, &provider]
        {
            while (true)
            {
                auto value = provider.Provide();
                if (!value.has_value())
                    break;

                input.produce(value.value());
            }
            input.setFinished();
        };

    // Thread function for solver consumers, it takes first available object to be processed or waits on consume() until it's ready
    // At the same time it works as a prodocer for output thread. The calculation of quadratic equations is done here
    auto solverProc = [&input, &output]
        {
            while (true)
            {
                auto value = input.consume();
                if (!value.has_value())
                    break;

                output.produce(std::make_pair(value.value(), Parabola::Solve(value.value())));
            }
        };

    // Thread function for output thread that prints the results of computations
    auto outputProc = [&output]()
        {
            while (true)
            {
                auto value = output.consume();
                if (!value.has_value())
                    break;

                std::cout << value.value().first << " => " << value.value().second << std::endl;
            }
        };

    // one single input thread
    std::thread readerThread(readerProc);

    // threads for solvers
    std::vector<std::thread> solverThreads;
    solverThreads.reserve(calculationTreadCount);

    for (size_t i = 0; i < calculationTreadCount; ++i)
        solverThreads.emplace_back(solverProc);

    // printing thread
    std::thread outputThread(outputProc);

    readerThread.join();
    std::for_each(solverThreads.begin(), solverThreads.end(), [](auto& thread){ thread.join(); });

    // once we waited all the computation threads to be finished, we know that there will be to data to output
    // and set output thread to release
    output.setFinished();
    outputThread.join();

    return 0;
}
