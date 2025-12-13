#include <thread>
#include <mutex>
#include <condition_variable>
#include<iostream>

using namespace std;

int main()
{
	std::mutex mtx;
	condition_variable c;
	int n = 100;
	bool flag = true;

	thread t1([&]() {
		int i = 0;
		while (i < n)
		{
			unique_lock<mutex> lock(mtx);
			while (!flag)
				c.wait(lock);
			cout << i << endl;
			flag = false;
			i += 2; // 偶数
			c.notify_one();
		}
		});

	thread t2([&]() {
		int j = 1;
		while (j < n)
		{
			unique_lock<mutex> lock(mtx);
			while (flag)
				c.wait(lock);
			cout << j << endl;
			j += 2; // 奇数
			flag = true;
			c.notify_one();
		}
		});


	t1.join();
	t2.join();
	return 0;
}
