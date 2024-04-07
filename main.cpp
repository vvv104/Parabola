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
    constexpr size_t inputBufferSize = 4;
    constexpr size_t outputBufferSize = 4;
    constexpr size_t calculationTreadCount = 4;

    ArrayView<char*> args(argv + 1, argc - 1);
    ParabolaDefinitionReader provider(args);

    SharedRingBuffer<Parabola::Definition, inputBufferSize> input;
    SharedRingBuffer<Parabola::Result, outputBufferSize> output;

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

    std::thread readerThread(readerProc);

    std::vector<std::thread> solverThreads;
    solverThreads.reserve(calculationTreadCount);

    for (size_t i = 0; i < calculationTreadCount; ++i)
        solverThreads.emplace_back(solverProc);

    std::thread outputThread(outputProc);

    readerThread.join();
    std::for_each(solverThreads.begin(), solverThreads.end(), [](auto& thread){ thread.join(); });

    output.setFinished();
    outputThread.join();

    return 0;
}
