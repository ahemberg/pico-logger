#include "system.hpp"

void System::measure()
{

    struct mallinfo m = mallinfo();

    uint32_t used_heap = m.uordblks;
    uint32_t total_heap = getTotalHeap();
    uint32_t free_heap = total_heap - used_heap;
    float used_percent = (float)used_heap / (float)total_heap;

    std::list<Datapoint> datapoints = {
        Datapoint("pct_heap", used_percent)
    };

    push(Measurement((uint32_t)get_time(), datapoints));
}

uint32_t System::getTotalHeap()
{
    extern char __StackLimit, __bss_end__;

    return &__StackLimit - &__bss_end__;
}

uint32_t System::getFreeHeap()
{
    struct mallinfo m = mallinfo();

    return getTotalHeap() - m.uordblks;
}