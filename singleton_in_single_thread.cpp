/*
 * @Author: Vilewind
 * @Date: 2022-04-21 20:15:48
 * @LastEditors: Vilewind
 * @LastEditTime: 2022-04-21 20:18:01
 * @FilePath: /Singleton/singleton_in_single_thread.cpp
 * @Version: 
 * @Description: 
 */
#include <iostream>

using namespace std;

class singleton {
private:
	static singleton* p;
	singleton(){}
public:
	singleton *instance();
};

singleton *singleton::instance(){
	static singleton p;
	return &p;
}
