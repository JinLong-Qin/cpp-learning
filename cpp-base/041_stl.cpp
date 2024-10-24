#include <iostream>
using namespace std;
#include <vector>
#include <algorithm>
void MyPrint(int val)
{
	cout << val << endl;
}
void test01() {
	//创建vector容器对象，并且通过模板参数指定容器中存放的数据的类型
	vector<int> v;
	//向容器中放数据
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	//每一个容器都有自己的迭代器，迭代器是用来遍历容器中的元素
	//v.begin()返回迭代器，这个迭代器指向容器中第一个数据
	//v.end()返回迭代器，这个迭代器指向容器元素的最后一个元素的下一个位置
	//vector<int>::iterator 拿到vector<int>这种容器的[迭代器类型]
	vector<int>::iterator pBegin = v.begin();
	vector<int>::iterator pEnd = v.end();
	//第一种遍历方式：
	while (pBegin != pEnd) {
		cout << *pBegin << endl;
		pBegin++;
	}
	
	//第二种遍历方式：
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		cout << *it << endl;
	}
	cout << endl;
	//第三种遍历方式：
	//使用STL提供标准遍历算法  头文件 algorithm
    // 回调函数
	for_each(v.begin(), v.end(), MyPrint);
}


// 2. vector存放自定义数据类型
class Person {
public:
	Person(string name, int age) {
		mName = name;
		mAge = age;
	}
public:
	string mName;
	int mAge;
};
//存放对象
void test02() {
	vector<Person> v;
	//创建数据
	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);
	Person p5("eee", 50);
	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);
	v.push_back(p5);
	for (vector<Person>::iterator it = v.begin(); it != v.end(); it++) {
		cout << "Name:" << (*it).mName << " Age:" << (*it).mAge << endl;
	}
}
//放对象指针
void test03() {
	vector<Person*> v;
	//创建数据
	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);
	Person p5("eee", 50);
	v.push_back(&p1);
	v.push_back(&p2);
	v.push_back(&p3);
	v.push_back(&p4);
	v.push_back(&p5);
	for (vector<Person*>::iterator it = v.begin(); it != v.end(); it++) {
		Person * p = (*it);
		cout << "Name:" << p->mName << " Age:" << (*it)->mAge << endl;
	}
}

// 嵌套vector
void test04(){
    // 创建大容器
    vector< vector<int> >  v;
    // 各个小容器
	vector<int> v1;
	vector<int> v2;
	vector<int> v3;
	vector<int> v4;
    // 为小容器填数据
	for (int i = 0; i < 4; i++) {
		v1.push_back(i + 1);
		v2.push_back(i + 2);
		v3.push_back(i + 3);
		v4.push_back(i + 4);
	}
	//将小容器元素插入到vector v中
	v.push_back(v1);
	v.push_back(v2);
	v.push_back(v3);
	v.push_back(v4);
    
	for (vector<vector<int>>::iterator it = v.begin(); it != v.end(); it++) {
        // 外层循环便利每一个小容器
		for (vector<int>::iterator vit = (*it).begin(); vit != (*it).end(); vit++) {
            //! *it 就看外层循环中，大容器尖括号中的内容是什么*it就是什么东西
            // 内层循环遍历每一个小容器中的元素
			cout << *vit << " ";
		}
		cout << endl;
	}
}

int main(){
    // stl
    // C++的面向对象和泛型编程思想，目的就是复用性的提升
    /*
    stl概念
    ● STL(Standard Template Library,标准模板库)
    ● STL 从广义上分为: 容器(container) 算法(algorithm) 迭代器(iterator)
    ● [容器]和[算法]之间通过[迭代器]进行无缝连接。
    ● STL 几乎所有的代码都采用了[模板类或者模板函数]
    */

    /*
    STL大体分为六大组件，分别是:容器、算法、迭代器、仿函数、适配器（配接器）、空间配置器
    1. 容器：各种数据结构，如vector、list、deque、set、map等,用来存放数据。
        容器：置物之所也
        STL容器就是将运用最广泛的一些数据结构实现出来
        常用的数据结构：数组, 链表,树, 栈, 队列, 集合, 映射表 等
        这些容器分为序列式容器和关联式容器两种:
        序列式容器:强调值的排序，序列式容器中的每个元素均有固定的位置。  给定的顺序是什么，存储出来就是什么
        关联式容器:二叉树结构，各元素之间没有严格的物理上的顺序关系   给定的顺序和真正存储的顺序不一定一样
    2. 算法：各种常用的算法，如sort、find、copy、for_each等
        算法：问题之解法也
        有限的步骤，解决逻辑或数学上的问题，这一门学科我们叫做算法(Algorithms)
        算法分为:质变算法和非质变算法。
        质变算法：是指运算过程中会更改区间内的元素的内容。例如拷贝，替换，删除等等
        非质变算法：是指运算过程中不会更改区间内的元素内容，例如查找、计数、遍历、寻找极值等等
    3. 迭代器：扮演了[容器]与[算法]之间的胶合剂。
        迭代器：容器和算法之间粘合剂
        提供一种方法，使之能够依序寻访某个容器所含的各个元素，而又无需暴露该容器的内部表示方式。
        每个容器都有自己专属的迭代器
        迭代器使用非常类似于指针，初学阶段我们可以先理解迭代器为指针

        种类	    功能	                                                      支持运算
        输入迭代器	对数据的只读访问	                                            只读，支持++、==、！=
        输出迭代器	对数据的只写访问	                                            只写，支持++
        前向迭代器	读写操作，并能向前推进迭代器	                                 读写，支持++、==、！=
        双向迭代器	读写操作，并能向前和向后操作	                                 读写，支持++、--，
        随机访问迭代器	读写操作，可以以跳跃的方式访问任意数据，功能最强的迭代器	    读写，支持++、--、[n]、-n、<、<=、>、>=

        常用的容器中迭代器种类为双向迭代器，和随机访问迭代器

    4. 仿函数：行为类似函数，可作为算法的某种策略。


    5. 适配器：一种用来修饰容器或者仿函数或迭代器接口的东西。
    6. 空间配置器：负责空间的配置与管理
    */

    // 容器 vector
    // STL中最常用的容器为Vector，可以理解为数组
    // 1. vector存放内置数据类型
    test01();
    // 2. vector存放自定义数据类型
    test02();

    test03();

    // 3. vector嵌套容器，简单理解为多维数组
    test04();

    return 0;
}