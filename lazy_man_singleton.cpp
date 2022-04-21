/*
 * @Author: Vilewind
 * @Date: 2022-04-21 20:15:48
 * @LastEditors: Vilewind
 * @LastEditTime: 2022-04-21 20:18:01
 * @FilePath: /Singleton/singleton_in_single_thread.cpp
 * @Version: 
 * @Description: 懒汉模式,运行时创建实例
 */
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

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
private:
/*< 唯一单例对象指针*/	
	static SingletonInstance *si;
};

SingletonInstance *SingletonInstance::si = nullptr;

SingletonInstance *SingletonInstance::get_instance() {
	if (si == nullptr) {
		si = new (std::nothrow) SingletonInstance;
	}
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

/*******************懒汉模式，多线程同时调用get_instance且发现si为空时，会创建多个实例对象指针*******************/
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