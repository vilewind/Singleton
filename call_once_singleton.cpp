#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <functional>
#include <algorithm>

class SingletonInstance {
public:
/*< 获取单例对象*/
	static SingletonInstance *get_instance();
/*< 释放单例对象*/
	static void delete_instance();
/*< 打印单例地址*/
	void print_instance();

private:
	SingletonInstance();
	~SingletonInstance();

	SingletonInstance(const SingletonInstance &singleton);
	const SingletonInstance &operator=(const SingletonInstance &singleton);
	// static void create_instance() {
	// 	si = new SingletonInstance;
	// }
private:
/*< 唯一单例对象指针*/	
	static SingletonInstance *si;
};

SingletonInstance *SingletonInstance::si = nullptr;

SingletonInstance *SingletonInstance::get_instance() {
	static std::once_flag once_flag_;
	// std::call_once(once_flag_, create_instance);
	std::call_once(once_flag_, []()
				   { si = new SingletonInstance; });
	return si;
}

void SingletonInstance::delete_instance(){
	if (si != nullptr) {
		delete si;
		si = nullptr;
	}
}

void SingletonInstance::print_instance() {
	std::cout << "SingletonInstance address " << this << std::endl;
}

SingletonInstance::SingletonInstance() {
	std::cout << "Constructor" << std::endl;
}

SingletonInstance::~SingletonInstance() {
	std::cout << "Destructor" << std::endl;
}

void print_hello(void *threadid) {
	int tid = *static_cast<int *>(threadid);
	std::cout << tid << std::endl;
	SingletonInstance::get_instance()->print_instance();
}

int main(int argc, char *argv[]) {
	std::vector<std::thread> threads(10);
	std::vector<int> indexes(5, 0);

	int i = 0;
	std::cout << "Starting" << std::endl;

	for (; i < 10; ++i) {
		std::cout << "create thread " << i << std::endl;
		indexes[i] = i;
		threads[i] = std::thread(std::bind(print_hello, (void *)&i));
	}

	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

	SingletonInstance::delete_instance();
	std::cout << "End" << std::endl;
	return 0;
}