/**
 * @file mutex_lazy_man_singleton.cpp
 * @author your name (you@domain.com)
 * @brief 加锁的懒汉模式，线程安全
 * @version 0.1
 * @date 2022-04-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>
#include <thread>
#include <mutex>

class SingletonInstance {
public:
/*< 获取单例对象*/
	static SingletonInstance *get_instance();
/*< 释放单例对象*/
	static void delete_instance();
/*< 打印单例地址*/
	void print_instance();
/*< 自动回收类及对象*/
	// class CG {
	// public:
	// 	~CG() {
	// 		if (SingletonInstance::si) {
	// 			delete si;
	// 			si = nullptr;
	// 		}
	// 	}
	// };
	// /*<< 程序结束时，自动调用CG的析构函数*/
	// static CG cg; 
private:
	SingletonInstance();
	~SingletonInstance();

	SingletonInstance(const SingletonInstance &singleton);
	const SingletonInstance &operator=(const SingletonInstance &singleton);
private:
/*< 唯一单例对象指针*/	
	static SingletonInstance *si;
	static std::mutex lock_;
};

// SingletonInstance::CG cg;
SingletonInstance *SingletonInstance::si = nullptr;

SingletonInstance *SingletonInstance::get_instance() {
/*< 每次调用本函数都会进行加锁、解锁操作，但实际上只有在需要new操作时才会需要加锁、解锁*/
	// std::lock_guard<std::mutex> guard(lock_);
	// if (si == nullptr) {
	// 	si = new (std::nothrow) SingletonInstance;
	// }
/*< 双重检查锁，由于上述的原因，我们在判定实例对象为空时才会进行加锁和解锁操作，同时为了防止获得锁后实例被初始化了再次进行判断*/
	/*<< 由于双重检查的存在，可以实现自动回收功能，即在类中定义一个回收类及其静态对象，23-34行*/
	//if (si == nullptr) {
		// 	std::lock_guard<std::mutex> guard(lock_);
		// 	if (si == nullptr) {
		// 		si = new (std::nothrow) SingletonInstance;
		// 	}
		// }
/*> 双重检查锁可能会出现内存读写的乱序执行，导致new操作的乱序*/
	/*>> new的步骤为分配内存，构造对象，使指针指向被分配的内存；
	*	第一步是最先执行的，然而第二三步则不一定。
	*	当一个线程调用本函数时，可能执行一三步，此时指针不为空，二另一个线程调用本函数时，没有对象存在
	* 可以使用memory barrier指令来解决这个问题
	*/
/*< 基于operator new + placement new强制使new按顺序执行*/
	// if (si == nullptr) {
	//  	std::lock_guard<std::mutex> guard(lock_);
	//  	if (si == nullptr) {
	//  		SingletonInstance *tmp = static_cast<SingletonInstance*>(operator new(sizeof(SingletonInstance)));
	//			new(si)SingletonInstance();
	//			si = tmp;
	//  	}
	//  }
/*< c++11后，使用静态局部变量，不需要在类中声明静态变量，
 *	新的C++标准规定了当一个线程正在初始化一个变量的时候，其他线程必须得等到该初始化完成以后才能访问它。*/
	// static SingletonInstance si;
	// return &si;
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

