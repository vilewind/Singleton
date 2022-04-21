#include <iostream>

using namespace std;

class singleton{
private:
	singleton(){}
	static singleton *p;
public:
	static singleton *instance();
};

/*< 与懒汉模式不同，在类定义时就创建p指向的对象，是线程安全的*/
singleton *singleton::p = new singleton();
singleton* singleton::instance(){
	return p;
}