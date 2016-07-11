// inside_cpp_object_model.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <typeinfo>

//#pragma pack(1)
class EmptyClass
{

};

void EmptyClassTest(void)
{
	EmptyClass e;
	EmptyClass ee[3];
	std::cout << "ee[0]:" << &ee[0] << " ee[1]:" << &ee[1] << " ee[2]:" << &ee[2] << std::endl;
	std::cout << "Empty class size is: " << sizeof(e) << std::endl;
}

class FullClass
{
public:
	FullClass()
	{}
public:
	void Func1(void)
	{}
	static void Func2(void)
	{}
	virtual void Func3(void)
	{}
private:
	int _var1; 
	static int _var2;
	char _var3; // 内存对齐
};

void FullClassTest(void)
{
	FullClass f;
	std::cout << "size of(int) is: " << sizeof(int) << std::endl;
	std::cout << "Full class size is: " << sizeof(f) << std::endl;
}

typedef void(*pFunc)(void);

// Base 类： First Inheritance Level
class Base
{
public:
	Base(int var = 10) : _base_var(var) {};
public:
	virtual void Run(void)
	{
		std::cout << "Base Class Run()" << std::endl;
	}
	virtual void RunBase(void) 
	{
		std::cout << "Base Class RunBase()" << std::endl;
	}
private:
	int _base_var;
};

void BaseTest2(void)
{
	Base b;
	std::cout << "---------begin base object inner memory layout test-------" << std::endl;

	char* p = (char*)&b;
	int** vfptr = (int**)*(int**)p; // 虚函数表地址

	std::cout << " object size is: " << sizeof(b) << std::endl;
	std::cout << " object addr is: " << &b << std::endl; // 
	std::cout << " object vfptr addr is: " << vfptr << std::endl; //vfptr 在对象第1个位置 
	std::cout << " typeid(b) is: " << typeid(b).name() << std::endl;
	//std::cout << " object vfptr[-1] type info name is: " << ((type_info*)((int*)(*(int*)&b) - 1))->name() << std::endl;
	std::cout << " object vfptr[0] func ptr  is: " << vfptr[0] << std::endl; //vfptr[0]的值 
	std::cout << " object vfptr[0] func invoke res: ";
	pFunc pRun = (pFunc)(vfptr[0]);
	(*pRun)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object vfptr[1] func ptr  is: " << vfptr[1] << std::endl; //vfptr[0]的值 
	std::cout << " object vfptr[1] func invoke res: ";
	pFunc pRunBase = (pFunc)(vfptr[1]);
	(*pRunBase)(); //调用vfptr[1](函数地址值)指向的函数
	std::cout << " object _base_var addr is: " << (int*)&b + 1 << std::endl; //vfptr 在对象第2个位置 
	std::cout << " object _base_var value is: " << *((int*)&b + 1) << std::endl; //vfptr 在对象第2个位置 


	std::cout << "---------end  base object inner memory layout test-------" << std::endl;

}
void BaseTest(void)
{
	Base b;
	std::cout << "---------begin base object inner memory layout test-------" << std::endl;

	std::cout << " object size is: " << sizeof(b) << std::endl;
	std::cout << " object addr is: " << &b << std::endl; // 
	std::cout << " object vfptr addr is: " << (int*)*(int*)&b << std::endl; //vfptr 在对象第1个位置 最外层(int*)是把第一个位置的值转换为int地址类型
	std::cout << " typeid(b) is: " << typeid(b).name() << std::endl;  // b对象类型
	//std::cout << " object vfptr[-1] type info name is: " << ((type_info*)((int*)(*(int*)&b) - 1))->name() << std::endl;
	std::cout << " object vfptr[0] func ptr  is: " << (int*)*(int*)(*(int*)&b)<< std::endl; //vfptr[0]的值 
	std::cout << " object vfptr[0] func invoke res: ";
	pFunc pRun = (pFunc)(*(int*)(*(int*)&b));
	(*pRun)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object vfptr[1] func ptr  is: " << (int*)*((int*)(*(int*)&b) + 1) << std::endl; //vfptr[1]的值 
	std::cout << " object vfptr[1] func invoke res: ";
	pFunc pRunBase = (pFunc)(*((int*)(*(int*)&b) + 1));
	(*pRunBase)(); //调用vfptr[1](函数地址值)指向的函数
	std::cout << " object _base_var addr is: " << (int*)&b + 1 << std::endl; //_base_var 在对象第2个位置 
	std::cout << " object _base_var value is: " << *((int*)&b + 1) << std::endl; //_base_var 在对象第2个位置 

	std::cout << "---------end  base object inner memory layout test-------" << std::endl;
}

// Single Common Derive： Second Inheritance Level
class Derive_Sin_Com : public Base
{
public:
	Derive_Sin_Com(int var = 20) : Base(var), _derive_sin_com_var(var) {}
public:
	virtual void Run(void) // override Base::Run()
	{
		std::cout << "Derive_Sin_Com Class Run()" << std::endl;
	}
	virtual void RunDerive_Sin_Com(void) // new virtual function
	{
		std::cout << "Derive_Sin_Com Class RunDerive_Sin_Com()" << std::endl;
	}
private:
	int _derive_sin_com_var;
};

void SingleCommonDeriveTest(void)
{
	Derive_Sin_Com d;
	std::cout << "---------begin single common derive object inner memory layout test-------" << std::endl;

	std::cout << " object size is: " << sizeof(d) << std::endl;
	std::cout << " object addr is: " << &d << std::endl; // d指针转换为int指针
	std::cout << " object vfptr addr is: " << (int*)*(int*)&d << std::endl; //  vfptr地址
	std::cout << " typeid(d) is: " << typeid(d).name() << std::endl;  // d对象类型
	std::cout << " object vfptr[0] func ptr  is: " << (int*)*(int*)(*(int*)&d) << std::endl; //vfptr[0]的值 
	std::cout << " object vfptr[0] func invoke res: ";
	pFunc pRun = (pFunc)(*(int*)(*(int*)&d));
	(*pRun)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object vfptr[1] func ptr  is: " << (int*)*((int*)(*(int*)&d) + 1) << std::endl; //vfptr[1]的值 
	std::cout << " object vfptr[1] func invoke res: ";
	pFunc pRunBase = (pFunc)(*((int*)(*(int*)&d) + 1));
	(*pRunBase)(); //调用vfptr[1](函数地址值)指向的函数
	std::cout << " object vfptr[2] func ptr  is: " << (int*)*((int*)(*(int*)&d) + 2) << std::endl; //vfptr[2]的值 
	std::cout << " object vfptr[2] func invoke res: ";
	pFunc pRunDerive_Sin_Com = (pFunc)(*((int*)(*(int*)&d) + 2));
	(*pRunDerive_Sin_Com)(); //调用vfptr[2](函数地址值)指向的函数
	std::cout << " object _base_var addr is: " << (int*)&d + 1 << std::endl; //_base_var 在对象第2个位置 
	std::cout << " object _base_var value is: " << *((int*)&d + 1) << std::endl; //_base_var 在对象第2个位置 
	std::cout << " object _derive_sin_com_var addr is: " << (int*)&d + 2 << std::endl; //_derive_sin_com_var 在对象第3个位置 
	std::cout << " object _derive_sin_com_var value is: " << *((int*)&d + 2) << std::endl; //_derive_sin_com_var 在对象第3个位置 
	
	std::cout << "---------end  single common derive  object inner memory layout test-------" << std::endl;
}

// Base2 类 : First Inheritance Level
class Base2
{
public:
	Base2(int var = 10) : _base2_var(var) {};
public:
	virtual void Run(void)
	{
		std::cout << "Base2 Class Run()" << std::endl;
	}
	virtual void RunBase2(void)
	{
		std::cout << "Base2 Class RunBase2()" << std::endl;
	}
private:
	int _base2_var;
};

// Multi Common Derive: Second Inheritance Level
class Derive_Mul_Com : public Base, public Base2 
{
public:
	Derive_Mul_Com(int var = 30) : Base(var), Base2(var), _derive_mul_com_var(var) {}
public:
	virtual void Run(void) // override Base::Run() and Base2::Run()?
	{
		std::cout << "Derive_Mul_Com Class Run()" << std::endl;
	}
	virtual void RunBase(void) // override Base::RunBase()
	{
		std::cout << "Derive_Mul_Com Class RunBase()" << std::endl;
	}
	virtual void RunDerive_Mul_Com(void) // new virtual function
	{
		std::cout << "Derive_Mul_Com Class RunDerive_Mul_Com()" << std::endl;
	}
private:
	int _derive_mul_com_var;
};

void MultiCommonDeriveTest(void)
{
	Derive_Mul_Com d;
	std::cout << "---------begin multi common derive object inner memory layout test-------" << std::endl;

	std::cout << " object size is: " << sizeof(d) << std::endl;
	std::cout << " object addr is: " << &d << std::endl; // d指针转换为int指针
	std::cout << " typeid(d) is: " << typeid(d).name() << std::endl;  // d对象类型
	std::cout << "------ base obj begin -----" << std::endl;
	std::cout << " object base vfptr addr is: " << (int*)*(int*)&d << std::endl; //  base vfptr地址 在对象第1个位置
	std::cout << " object base vfptr[0] func ptr  is: " << (int*)*(int*)(*(int*)&d) << std::endl; //vfptr[0]的值 
	std::cout << " object base vfptr[0] func invoke res: ";
	pFunc pRun = (pFunc)(*(int*)(*(int*)&d));
	(*pRun)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object base vfptr[1] func ptr  is: " << (int*)*((int*)(*(int*)&d) + 1) << std::endl; //vfptr[1]的值 
	std::cout << " object base vfptr[1] func invoke res: ";
	pFunc pRunBase = (pFunc)(*((int*)(*(int*)&d) + 1));
	(*pRunBase)(); //调用vfptr[1](函数地址值)指向的函数
	std::cout << " object base vfptr[2] func ptr  is: " << (int*)*((int*)(*(int*)&d) + 2) << std::endl; //vfptr[2]的值 
	std::cout << " object base vfptr[2] func invoke res: ";
	pFunc pRunDerive_Mul_Com = (pFunc)(*((int*)(*(int*)&d) + 2));
	(*pRunDerive_Mul_Com)(); //调用vfptr[2](函数地址值)指向的函数
	std::cout << " object _base_var addr is: " << (int*)&d + 1 << std::endl; //_base_var 在对象第2个位置 
	std::cout << " object _base_var value is: " << *((int*)&d + 1) << std::endl; 
	std::cout << "------ base obj end -----" << std::endl;

	std::cout << "------ base2 obj begin -----" << std::endl;
	std::cout << " object base2 vfptr addr is: " << (int*)*((int*)&d + 2) << std::endl; //  base2 vfptr地址 在对象第3个位置 
	std::cout << " object base2 vfptr[0] func ptr  is: " << (int*)*(int*)(*((int*)&d + 2)) << std::endl; //vfptr[0]的值 
	std::cout << " object base2 vfptr[0] func invoke res: ";
	pFunc pRun2 = (pFunc)(*(int*)(*((int*)&d + 2)));
	(*pRun2)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object base2 vfptr[1] func ptr  is: " << (int*)*((int*)(*((int*)&d + 2)) + 1) << std::endl; //vfptr[1]的值 
	std::cout << " object base2 vfptr[1] func invoke res: ";
	pFunc pRunBase2 = (pFunc)(*((int*)(*((int*)&d + 2)) + 1));
	(*pRunBase2)(); //调用vfptr[1](函数地址值)指向的函数
	std::cout << " object _base2_var addr is: " << (int*)&d + 3 << std::endl; //_base2_var 在对象第4个位置 
	std::cout << " object _base2_var value is: " << *((int*)&d + 3) << std::endl;
	std::cout << "------ base2 obj end -----" << std::endl;

	std::cout << " object _derive_mul_com_var addr is: " << (int*)&d + 4 << std::endl; //_derive_mul_com_var 在对象第5个位置 
	std::cout << " object _derive_mul_com_var value is: " << *((int*)&d + 4) << std::endl; 

	std::cout << "---------end  multi common derive  object inner memory layout test-------" << std::endl;
}

// Single Virtual Derive: Second Inheritance Level
class Derive_Sin_Vir : virtual public Base
{
public:
	Derive_Sin_Vir(int var = 40) : Base(var), _derive_sin_vir_var(var) {};
public:
	virtual void Run(void) // override Base::Run()
	{
		std::cout << "Derive_Sin_Vir Class Run()" << std::endl;
	}
	virtual void RunDerive_Sin_Vir(void) // new virtual function
	{
		std::cout << "Derive_Sin_Vir Class RunDerive_Sin_Vir()" << std::endl;
	}
private:
	int _derive_sin_vir_var;
};

void SingleVirtualDeriveTest(void)
{
	Derive_Sin_Vir d;
	std::cout << "---------begin single virtual derive object inner memory layout test-------" << std::endl;

	std::cout << " object size is: " << sizeof(d) << std::endl;
	std::cout << " object addr is: " << &d << std::endl; // d指针转换为int指针
	std::cout << " typeid(d) is: " << typeid(d).name() << std::endl;  // d对象类型
	std::cout << " object vfptr addr is: " << (int*)*(int*)&d << std::endl; //  derive vfptr地址 在对象第1个位置
	std::cout << " object vfptr[0] func ptr  is: " << (int*)*(int*)(*(int*)&d) << std::endl; //vfptr[0]的值 
	std::cout << " object vfptr[0] func invoke res: ";
	pFunc pRunDerive_Sin_Vir = (pFunc)(*(int*)(*(int*)&d));
	(*pRunDerive_Sin_Vir)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object vbptr addr is: " << (int*)*((int*)&d + 1) << std::endl; //  derive vbptr 地址 在对象第2个位置
	std::cout << " object vbptr[0] value is: " << *((int*)*((int*)&d + 1)) << std::endl; //  vbptr[0]的值： vbptr在对象中的偏移,负值
	std::cout << " object vbptr[1] value is: " << *((int*)*((int*)&d + 1) + 1) << std::endl; // vbptr[1]的值： 虚基类base在对象中的偏移,正值
	std::cout << " object _derive_sin_vir_var addr is: " << (int*)&d + 2 << std::endl; //_derive_sin_vir_var 在对象第3个位置 
	std::cout << " object _derive_sin_vir_var value is: " << *((int*)&d + 2) << std::endl;
	
	std::cout << " object vtordisp for vbase Base addr is: " << (int*)&d + 3 << std::endl; //vtordisp for virtual base 在对象第4个位置 
	std::cout << " object vtordisp for vbase Base value is: " << *((int*)&d + 3) << std::endl;
	
	std::cout << " object base vfptr addr is: " << (int*)*((int*)&d + 4) << std::endl; //  virtual base vfptr地址在对象第5个位置
	std::cout << " object base vfptr[0] func ptr  is: " << (int*)*(int*)(*((int*)&d + 4)) << std::endl; //vfptr[0]的值 
	std::cout << " object base vfptr[0] func invoke res: ";
	pFunc pRun = (pFunc)(*(int*)(*((int*)&d + 4)));
	(*pRun)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object base vfptr[1] func ptr  is: " << (int*)*((int*)(*((int*)&d + 4)) + 1) << std::endl; //vfptr[1]的值 
	std::cout << " object base vfptr[1] func invoke res: ";
	pFunc pRunBase = (pFunc)(*((int*)(*((int*)&d + 4)) + 1));
	(*pRunBase)(); //调用vfptr[1](函数地址值)指向的函数
	std::cout << " object _base_var addr is: " << (int*)&d + 5 << std::endl; //_base_var 在对象第6个位置 
	std::cout << " object _base_var value is: " << *((int*)&d + 5) << std::endl;

	std::cout << "---------end  single virtual  derive  object inner memory layout test-------" << std::endl;
}

// Multi Half Virtual Derive: Second Inheritance Level
class Derive_Half_Mul_Vir : virtual public Base,  public Base2
{
public:
	Derive_Half_Mul_Vir(int var = 50) : Base(var), Base2(var), _derive_half_mul_vir_var(var) {};
public:
	virtual void Run(void) // override Base::Run()
	{
		std::cout << "Derive_Half_Mul_Vir Class Run()" << std::endl;
	}
	virtual void RunBase2(void) // override Base2::RunBase2()
	{
		std::cout << "Derive_Half_Mul_Vir Class RunBase2()" << std::endl;
	}
	virtual void RunDerive_Half_Mul_Vir(void) // new virtual function
	{
		std::cout << "Derive_Half_Mul_Vir Class RunDerive_Half_Mul_Vir()" << std::endl;
	}
private:
	int _derive_half_mul_vir_var;
};

void HalfMultiVirtualDeriveTest(void)
{
	Derive_Half_Mul_Vir d;
	std::cout << "---------begin half multi virtual derive object inner memory layout test-------" << std::endl;

	std::cout << " object size is: " << sizeof(d) << std::endl;
	std::cout << " object addr is: " << &d << std::endl; // d指针转换为int指针
	std::cout << " typeid(d) is: " << typeid(d).name() << std::endl;  // d对象类型
	std::cout << " object vfptr addr is: " << (int*)*(int*)&d << std::endl; //  derive(base2) vfptr地址 在对象第1个位置
	std::cout << " object vfptr[0] func ptr  is: " << (int*)*(int*)(*(int*)&d) << std::endl; //vfptr[0]的值 
	std::cout << " object vfptr[0] func invoke res: ";
	pFunc pRun = (pFunc)(*(int*)(*(int*)&d));
	(*pRun)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object vfptr[1] func ptr  is: " << (int*)*((int*)(*(int*)&d) + 1) << std::endl; //vfptr[1]的值 
	std::cout << " object vfptr[1] func invoke res: ";
	pFunc pRunBase2 = (pFunc)(*((int*)(*(int*)&d) + 1));
	(*pRunBase2)(); //调用vfptr[1](函数地址值)指向的函数
	std::cout << " object vfptr[2] func ptr  is: " << (int*)*((int*)(*(int*)&d) + 2) << std::endl; //vfptr[2]的值 
	std::cout << " object vfptr[2] func invoke res: ";
	pFunc pRunDerive_Half_Mul_Vir = (pFunc)(*((int*)(*(int*)&d) + 2));
	(*pRunDerive_Half_Mul_Vir)(); //调用vfptr[2](函数地址值)指向的函数
	std::cout << " object _base2_var addr is: " << (int*)&d + 1 << std::endl; //_base2_var 在对象第2个位置 
	std::cout << " object _base2_var value is: " << *((int*)&d + 1) << std::endl;
	std::cout << " object vbptr addr is: " << (int*)*((int*)&d + 2) << std::endl; //  derive vbptr 地址 在对象第3个位置
	std::cout << " object vbptr[0] value is: " << *((int*)*((int*)&d + 2)) << std::endl; //  vbptr[0]的值： vbptr在对象中的偏移,负值
	std::cout << " object vbptr[1] value is: " << *((int*)*((int*)&d + 2) + 1) << std::endl; // vbptr[1]的值： 虚基类base在对象中的偏移,正值
	std::cout << " object _derive_half_mul_vir_var addr is: " << (int*)&d + 3 << std::endl; //_derive_half_mul_vir_var 在对象第4个位置 
	std::cout << " object _derive_half_mul_vir_var value is: " << *((int*)&d + 3) << std::endl;

	std::cout << " object vtordisp for vbase Base addr is: " << (int*)&d + 4 << std::endl; //vtordisp for virtual base 在对象第5个位置 
	std::cout << " object vtordisp for vbase Base value is: " << *((int*)&d + 4) << std::endl;

	std::cout << " object base vfptr addr is: " << (int*)*((int*)&d + 5) << std::endl; //  virtual base vfptr地址在对象第6个位置
	std::cout << " object base vfptr[0] func ptr  is: " << (int*)*(int*)(*((int*)&d + 5)) << std::endl; //vfptr[0]的值 
	std::cout << " object base vfptr[0] func invoke res: ";
	pFunc pRun2 = (pFunc)(*(int*)(*((int*)&d + 5)));
	(*pRun2)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object base vfptr[1] func ptr  is: " << (int*)*((int*)(*((int*)&d + 5)) + 1) << std::endl; //vfptr[1]的值 
	std::cout << " object base vfptr[1] func invoke res: ";
	pFunc pRunBase = (pFunc)(*((int*)(*((int*)&d + 5)) + 1));
	(*pRunBase)(); //调用vfptr[1](函数地址值)指向的函数
	std::cout << " object _base_var addr is: " << (int*)&d + 6 << std::endl; //_base_var 在对象第7个位置 
	std::cout << " object _base_var value is: " << *((int*)&d + 6) << std::endl;

	std::cout << "---------end  half multi virtual  derive  object inner memory layout test-------" << std::endl;
}


// Multi Both Virtual Derive: Second Inheritance Level
class Derive_Both_Mul_Vir : virtual public Base, virtual public Base2
{
public:
	Derive_Both_Mul_Vir(int var = 50) : Base(var), Base2(var), _derive_both_mul_vir_var(var) {};
public:
	virtual void Run(void) // override Base::Run()
	{
		std::cout << "Derive_Both_Mul_Vir Class Run()" << std::endl;
	}
	virtual void RunBase2(void) // override Base2::RunBase2()
	{
		std::cout << "Derive_Both_Mul_Vir Class RunBase2()" << std::endl;
	}
	virtual void RunDerive_Both_Mul_Vir(void) // new virtual function
	{
		std::cout << "Derive_Both_Mul_Vir Class RunDerive_Both_Mul_Vir()" << std::endl;
	}
private:
	int _derive_both_mul_vir_var;
};

void BothMultiVirtualDeriveTest(void)
{
	Derive_Both_Mul_Vir d;
	std::cout << "---------begin both multi virtual derive object inner memory layout test-------" << std::endl;

	std::cout << " object size is: " << sizeof(d) << std::endl;
	std::cout << " object addr is: " << &d << std::endl; // d指针转换为int指针
	std::cout << " typeid(d) is: " << typeid(d).name() << std::endl;  // d对象类型
	std::cout << " object vfptr addr is: " << (int*)*(int*)&d << std::endl; //  derive vfptr地址 在对象第1个位置
	std::cout << " object vfptr[0] func ptr  is: " << (int*)*(int*)(*(int*)&d) << std::endl; //vfptr[0]的值 
	std::cout << " object vfptr[0] func invoke res: ";
	pFunc pRunDerive_Both_Mul_Vir = (pFunc)(*(int*)(*(int*)&d));
	(*pRunDerive_Both_Mul_Vir)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object vbptr addr is: " << (int*)*((int*)&d + 1) << std::endl; //  derive vbptr 地址 在对象第2个位置
	std::cout << " object vbptr[0] value is: " << *((int*)*((int*)&d + 1)) << std::endl; //  vbptr[0]的值： vbptr在对象中的偏移,负值
	std::cout << " object vbptr[1] value is: " << *((int*)*((int*)&d + 1) + 1) << std::endl; // vbptr[1]的值： 虚基类base在对象中的偏移,正值
	std::cout << " object vbptr[2] value is: " << *((int*)*((int*)&d + 1) + 1) << std::endl; // vbptr[2]的值： 虚基类base2在对象中的偏移,正值
	std::cout << " object _derive_both_mul_vir_var addr is: " << (int*)&d + 2 << std::endl; //_derive_both_mul_vir_var 在对象第3个位置 
	std::cout << " object _derive_both_mul_vir_var value is: " << *((int*)&d + 2) << std::endl;

	std::cout << " object vtordisp for vbase Base addr is: " << (int*)&d + 3 << std::endl; //vtordisp for virtual base 在对象第4个位置 
	std::cout << " object vtordisp for vbase Base value is: " << *((int*)&d + 3) << std::endl;

	std::cout << " object base vfptr addr is: " << (int*)*((int*)&d + 4) << std::endl; //  virtual base vfptr地址在对象第5个位置
	std::cout << " object base vfptr[0] func ptr  is: " << (int*)*(int*)(*((int*)&d + 4)) << std::endl; //vfptr[0]的值 
	std::cout << " object base vfptr[0] func invoke res: ";
	pFunc pRun = (pFunc)(*(int*)(*((int*)&d + 4)));
	(*pRun)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object base vfptr[1] func ptr  is: " << (int*)*((int*)(*((int*)&d + 4)) + 1) << std::endl; //vfptr[1]的值 
	std::cout << " object base vfptr[1] func invoke res: ";
	pFunc pRunBase = (pFunc)(*((int*)(*((int*)&d + 4)) + 1));
	(*pRunBase)(); //调用vfptr[1](函数地址值)指向的函数
	std::cout << " object _base_var addr is: " << (int*)&d + 5 << std::endl; //_base_var 在对象第6个位置 
	std::cout << " object _base_var value is: " << *((int*)&d + 5) << std::endl;

	std::cout << " object vtordisp for vbase Base2 addr is: " << (int*)&d + 6 << std::endl; //vtordisp for virtual base2 在对象第7个位置 
	std::cout << " object vtordisp for vbase Base2 value is: " << *((int*)&d + 6) << std::endl;

	std::cout << " object base2 vfptr addr is: " << (int*)*((int*)&d + 7) << std::endl; //  virtual base2 vfptr地址在对象第8个位置
	std::cout << " object base2 vfptr[0] func ptr  is: " << (int*)*(int*)(*((int*)&d + 7)) << std::endl; //vfptr[0]的值 
	std::cout << " object base2 vfptr[0] func invoke res: ";
	pFunc pRun2 = (pFunc)(*(int*)(*((int*)&d + 7)));
	(*pRun2)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object base2 vfptr[1] func ptr  is: " << (int*)*((int*)(*((int*)&d + 7)) + 1) << std::endl; //vfptr[1]的值 
	std::cout << " object base2 vfptr[1] func invoke res: ";
	pFunc pRunBase2 = (pFunc)(*((int*)(*((int*)&d + 7)) + 1));
	(*pRunBase2)(); //调用vfptr[1](函数地址值)指向的函数
	std::cout << " object _base2_var addr is: " << (int*)&d + 8 << std::endl; //_base2_var 在对象第9个位置 
	std::cout << " object _base2_var value is: " << *((int*)&d + 8) << std::endl;

	std::cout << "---------end  both multi virtual  derive  object inner memory layout test-------" << std::endl;
}

class B
{
public:
	B(int var = 10): _b(var){};
	~B() {}
public:
	virtual void Run(void)
	{
		std::cout << "B Run()" << std::endl;
	}
	virtual void RunB(void)
	{
		std::cout << "B RunB()" << std::endl;
	}
private:
	int _b;
};

class D1 : public B
{
public:
	D1(int var = 20): B(var),_d1(var) {};
	~D1() {}
public:
	virtual void Run(void)
	{
		std::cout << "D1 Run()" << std::endl;
	}
	virtual void RunD1(void)
	{
		std::cout << "D1 RunD1()" << std::endl;
	}
private:
	int _d1;
};

class D2 : public B
{
public:
	D2(int var = 20) : B(var), _d2(var) {};
	~D2() {}
public:
	virtual void Run(void)
	{
		std::cout << "D2 Run()" << std::endl;
	}
	virtual void RunD2(void)
	{
		std::cout << "D2 RunD2()" << std::endl;
	}
private:
	int _d2;
};


class Rhombus : public D1, public D2
{
public:
	Rhombus(int var = 30) : D1(var), D2(var), _rhombus(var) {};
	~Rhombus() {}
public:
	virtual void Run(void)
	{
		std::cout << "Rhombus Run()" << std::endl;
	}
	virtual void RunRhombus(void)
	{
		std::cout << "Rhombus RunRhombus()" << std::endl;
	}
private:
	int _rhombus;
};

void RhombusCommonDeriveTest(void)
{
	Rhombus d;
	std::cout << "---------begin rhombus common derive object inner memory layout test-------" << std::endl;

	std::cout << " object size is: " << sizeof(d) << std::endl;
	std::cout << " object addr is: " << &d << std::endl; // d指针转换为int指针
	std::cout << " typeid(d) is: " << typeid(d).name() << std::endl;  // d对象类型
	std::cout << "------ d1 obj begin -----" << std::endl;
	std::cout << " object d1 vfptr addr is: " << (int*)*(int*)&d << std::endl; //  d1 vfptr地址 在对象第1个位置
	std::cout << " object d1 vfptr[0] func ptr  is: " << (int*)*(int*)(*(int*)&d) << std::endl; //vfptr[0]的值 
	std::cout << " object d1 vfptr[0] func invoke res: ";
	pFunc pRun = (pFunc)(*(int*)(*(int*)&d));
	(*pRun)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object d1 vfptr[1] func ptr  is: " << (int*)*((int*)(*(int*)&d) + 1) << std::endl; //vfptr[1]的值 
	std::cout << " object d1 vfptr[1] func invoke res: ";
	pFunc pRunB = (pFunc)(*((int*)(*(int*)&d) + 1));
	(*pRunB)(); //调用vfptr[1](函数地址值)指向的函数
	std::cout << " object d1 vfptr[2] func ptr  is: " << (int*)*((int*)(*(int*)&d) + 2) << std::endl; //vfptr[2]的值 
	std::cout << " object d1 vfptr[2] func invoke res: ";
	pFunc pRunD1 = (pFunc)(*((int*)(*(int*)&d) + 2));
	(*pRunD1)(); //调用vfptr[2](函数地址值)指向的函数
	std::cout << " object d1 vfptr[3] func ptr  is: " << (int*)*((int*)(*(int*)&d) + 3) << std::endl; //vfptr[3]的值 
	std::cout << " object d1 vfptr[3] func invoke res: ";
	pFunc pRunRhombus = (pFunc)(*((int*)(*(int*)&d) + 3));
	(*pRunRhombus)(); //调用vfptr[3](函数地址值)指向的函数

	std::cout << " object _b addr is: " << (int*)&d + 1 << std::endl; //_b 在对象第2个位置 
	std::cout << " object _b value is: " << *((int*)&d + 1) << std::endl;

	std::cout << " object _d1 addr is: " << (int*)&d + 2 << std::endl; //_d1 在对象第3个位置 
	std::cout << " object _d1 value is: " << *((int*)&d + 2) << std::endl;
	std::cout << "------ d1 obj end -----" << std::endl;

	std::cout << "------ d2 obj begin -----" << std::endl;
	std::cout << " object d2 vfptr addr is: " << (int*)*((int*)&d + 3) << std::endl; //  d2 vfptr地址 在对象第4个位置 
	std::cout << " object d2 vfptr[0] func ptr  is: " << (int*)*(int*)(*((int*)&d + 3)) << std::endl; //vfptr[0]的值 
	std::cout << " object d2 vfptr[0] func invoke res: ";
	pFunc pRun2 = (pFunc)(*(int*)(*((int*)&d + 3)));
	(*pRun2)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object d2 vfptr[1] func ptr  is: " << (int*)*((int*)(*((int*)&d + 3)) + 1) << std::endl; //vfptr[1]的值 
	std::cout << " object d2 vfptr[1] func invoke res: ";
	pFunc pRunB2 = (pFunc)(*((int*)(*((int*)&d + 3)) + 1));
	(*pRunB2)(); //调用vfptr[1](函数地址值)指向的函数
	std::cout << " object d2 vfptr[2] func ptr  is: " << (int*)*((int*)(*((int*)&d + 3)) + 2) << std::endl; //vfptr[2]的值 
	std::cout << " object d2 vfptr[2] func invoke res: ";
	pFunc pRunD2 = (pFunc)(*((int*)(*((int*)&d + 3)) + 2));
	(*pRunD2)(); //调用vfptr[2](函数地址值)指向的函数

	std::cout << " object _b addr is: " << (int*)&d + 4 << std::endl; //_b 在对象第5个位置 
	std::cout << " object _b value is: " << *((int*)&d + 4) << std::endl;

	std::cout << " object _d2 addr is: " << (int*)&d + 5 << std::endl; //_base2_var 在对象第6个位置 
	std::cout << " object _d2 value is: " << *((int*)&d + 5) << std::endl;
	std::cout << "------ d2 obj end -----" << std::endl;

	std::cout << " object _rhombus addr is: " << (int*)&d + 6 << std::endl; //_rhombus 在对象第7个位置 
	std::cout << " object _rhombus value is: " << *((int*)&d + 6) << std::endl;

	std::cout << "---------end  rhombus common derive  object inner memory layout test-------" << std::endl;
}

class D1_Virtual : virtual public B
{
public:
	D1_Virtual(int var = 20) : B(var), _d1_virtual(var) {};
	~D1_Virtual() {}
public:
	virtual void Run(void)
	{
		std::cout << "D1_Virtual Run()" << std::endl;
	}
	virtual void RunD1_Virtual(void)
	{
		std::cout << "D1_Virtual RunD1_Virtual()" << std::endl;
	}
private:
	int _d1_virtual;
};

class D2_Virtual : virtual public B
{
public:
	D2_Virtual(int var = 20) : B(var), _d2_virtual(var) {};
	~D2_Virtual() {}
public:
	virtual void Run(void)
	{
		std::cout << "D2_Virtual Run()" << std::endl;
	}
	virtual void RunD2_Virtual(void)
	{
		std::cout << "D2_Virtual RunD2_Virtual()" << std::endl;
	}
private:
	int _d2_virtual;
};

class Rhombus_Virtual : public D1_Virtual, public D2_Virtual
{
public:
	Rhombus_Virtual(int var = 30) : D1_Virtual(var), D2_Virtual(var), _rhombus_virtual(var) {};
	~Rhombus_Virtual() {}
public:
	virtual void Run(void)
	{
		std::cout << "Rhombus_Virtual Run()" << std::endl;
	}
	virtual void RunRhombus_Virtual(void)
	{
		std::cout << "Rhombus_Virtual RunRhombus_Virtual()" << std::endl;
	}
private:
	int _rhombus_virtual;
};

void RhombusVirtualDeriveTest(void)
{
	Rhombus_Virtual d;
	std::cout << "---------begin rhombus virtual derive object inner memory layout test-------" << std::endl;

	std::cout << " object size is: " << sizeof(d) << std::endl;
	std::cout << " object addr is: " << &d << std::endl; // d指针转换为int指针
	std::cout << " typeid(d) is: " << typeid(d).name() << std::endl;  // d对象类型
	std::cout << "------ d1 obj begin -----" << std::endl;
	std::cout << " object d1 vfptr addr is: " << (int*)*(int*)&d << std::endl; //  d1 vfptr地址 在对象第1个位置
	std::cout << " object d1 vfptr[0] func ptr  is: " << (int*)*(int*)(*(int*)&d) << std::endl; //vfptr[0]的值 
	std::cout << " object d1 vfptr[0] func invoke res: ";
	pFunc pRunD1 = (pFunc)(*(int*)(*(int*)&d));
	(*pRunD1)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object d1 vfptr[1] func ptr  is: " << (int*)*((int*)(*(int*)&d) + 1) << std::endl; //vfptr[1]的值 
	std::cout << " object d1 vfptr[1] func invoke res: ";
	pFunc pRunRhombus_Virtual = (pFunc)(*((int*)(*(int*)&d) + 1));
	(*pRunRhombus_Virtual)(); //调用vfptr[1](函数地址值)指向的函数
	std::cout << " object d1 vbptr addr is: " << (int*)*((int*)&d + 1) << std::endl; //  d1 vbptr 地址 在对象第2个位置
	std::cout << " object d1 vbptr[0] value is: " << *((int*)*((int*)&d + 1)) << std::endl; //  vbptr[0]的值： vbptr在对象中的偏移,负值
	std::cout << " object d1 vbptr[1] value is: " << *((int*)*((int*)&d + 1) + 1) << std::endl; // vbptr[1]的值： 虚基类base在对象中的偏移,正值
	std::cout << " object _d1 addr is: " << (int*)&d + 2 << std::endl; //_d1 在对象第3个位置 
	std::cout << " object _d1 value is: " << *((int*)&d + 2) << std::endl;
	std::cout << "------ d1 obj end -----" << std::endl;

	std::cout << "------ d2 obj begin -----" << std::endl;
	std::cout << " object d2 vfptr addr is: " << (int*)*((int*)&d + 3) << std::endl; //  d2 vfptr地址 在对象第4个位置 
	std::cout << " object d2 vfptr[0] func ptr  is: " << (int*)*(int*)(*((int*)&d + 3)) << std::endl; //vfptr[0]的值 
	std::cout << " object d2 vfptr[0] func invoke res: ";
	pFunc pRunD2 = (pFunc)(*(int*)(*((int*)&d + 3)));
	(*pRunD2)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object d2 vbptr addr is: " << (int*)*((int*)&d + 4) << std::endl; //  d1 vbptr 地址 在对象第5个位置
	std::cout << " object d2 vbptr[0] value is: " << *((int*)*((int*)&d + 4)) << std::endl; //  vbptr[0]的值： vbptr在对象中的偏移,负值
	std::cout << " object d2 vbptr[1] value is: " << *((int*)*((int*)&d + 4) + 1) << std::endl; // vbptr[1]的值： 虚基类base在对象中的偏移,正值
	std::cout << " object _d2 addr is: " << (int*)&d + 5 << std::endl; //_d2 在对象第6个位置 
	std::cout << " object _d2 value is: " << *((int*)&d + 5) << std::endl;
	std::cout << "------ d2 obj end -----" << std::endl;

	std::cout << " object _rhombus_virtual addr is: " << (int*)&d + 6 << std::endl; //_rhombus_virtual 在对象第7个位置 
	std::cout << " object _rhombus_virtual value is: " << *((int*)&d + 6) << std::endl;

	std::cout << " object vtordisp for vbase Base addr is: " << (int*)&d + 7 << std::endl; //vtordisp for virtual base 在对象第8个位置 
	std::cout << " object vtordisp for vbase Base value is: " << *((int*)&d + 7) << std::endl;


	std::cout << " object b vfptr addr is: " << (int*)*((int*)&d + 8) << std::endl; //  b vfptr地址 在对象第9个位置 
	std::cout << " object b vfptr[0] func ptr  is: " << (int*)*(int*)(*((int*)&d + 8)) << std::endl; //vfptr[0]的值 
	std::cout << " object b vfptr[0] func invoke res: ";
	pFunc pRun = (pFunc)(*(int*)(*((int*)&d + 8)));
	(*pRun)(); //调用vfptr[0](函数地址值)指向的函数
	std::cout << " object b vfptr[1] func ptr  is: " << (int*)(*((int*)(*((int*)&d + 8)) + 1)) << std::endl; //vfptr[0]的值 
	std::cout << " object b vfptr[1] func invoke res: ";
	pFunc pRunB = (pFunc)(*((int*)(*((int*)&d + 8)) + 1));
	(*pRunB)(); //调用vfptr[0](函数地址值)指向的函数

	std::cout << " object _b addr is: " << (int*)&d + 9 << std::endl; //_b 在对象第10个位置 
	std::cout << " object _b value is: " << *((int*)&d + 9) << std::endl;

	std::cout << "---------end  rhombus virtual derive  object inner memory layout test-------" << std::endl;
}


int main()
{
	//EmptyClassTest();
	//FullClassTest();
	//BaseTest2();
	//BaseTest();
	//SingleCommonDeriveTest();
	//MultiCommonDeriveTest();
	//SingleVirtualDeriveTest();
	//HalfMultiVirtualDeriveTest();
	//BothMultiVirtualDeriveTest();
	//RhombusCommonDeriveTest();
	RhombusVirtualDeriveTest();
    return 0;
}

