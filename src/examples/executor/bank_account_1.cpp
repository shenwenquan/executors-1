#include <experimental/thread_pool>
#include <iostream>

using std::experimental::post;
using std::experimental::thread_pool;

// Traditional active object pattern.
// Member functions do not block.

class bank_account
{
  int balance_ = 0;
  thread_pool pool_{1};
  mutable thread_pool::executor_type ex_ = pool_.get_executor();

public:
  void deposit(int amount)
  {
    post(ex_, [=]
      {
        balance_ += amount;
      });
  }

  void withdraw(int amount)
  {
    post(ex_, [=]
      {
        if (balance_ >= amount)
          balance_ -= amount;
      });
  }

  void print_balance() const
  {
    post(ex_, [=]
      {
        std::cout << "balance = " << balance_ << "\n";
      });
  }

  ~bank_account()
  {
    pool_.join();
  }
};

int main()
{
  bank_account acct;
  acct.deposit(20);
  acct.withdraw(10);
  acct.print_balance();
}
