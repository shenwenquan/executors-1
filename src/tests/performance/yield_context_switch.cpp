#include <experimental/loop_scheduler>
#include <experimental/yield>
#include <chrono>
#include <iostream>

using namespace std::experimental;

const int chains = 4;
const int iterations = 1000000;

int main()
{
  loop_scheduler s(1);
  auto ex = s.get_executor();

  for (int c = 0; c < chains; ++c)
  {
    dispatch(
      wrap(ex,
        [](basic_yield_context<loop_scheduler::executor_type> yield)
        {
          for (int i = 0; i < iterations; ++i)
          {
            post(yield);
          }
        }));
  }

  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  s.run();
  std::chrono::steady_clock::duration elapsed = std::chrono::steady_clock::now() - start;

  std::cout << "time per switch: ";
  std::chrono::steady_clock::duration per_iteration = elapsed / iterations / chains;
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(per_iteration).count();
  std::cout << " nanoseconds\n";

  std::cout << "switches per second: ";
  std::cout << (std::chrono::seconds(1) / per_iteration) << "\n";
}
