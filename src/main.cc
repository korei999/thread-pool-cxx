#include "threadpool.hh"
#include "utils.hh"

std::atomic<int> what = 0;

void
hello()
{
    COUT("HELLO BIDEN: {}\n", what++);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

int
main()
{
    ThreadPool tPool(4);

    tPool.submit(hello);
    tPool.submit(hello);
    tPool.submit(hello);
    tPool.submit(hello);

    tPool.submit(hello);
    tPool.submit(hello);
    tPool.submit(hello);
    tPool.submit(hello);


    tPool.wait();
}
