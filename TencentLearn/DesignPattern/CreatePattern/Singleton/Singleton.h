#pragma once
#include<memory>
namespace utils
{
	template <class T>
	class Singleton
	{
	public:
		static T* Instance();
	protected:
		static T* m_pInstance; 

		Singleton() {}
		virtual ~Singleton() {} // 虚析构函数确保使用基类指针指向派生类对象时，可以正确调用派生类析构函数释放内存
	};

	template <class T>
	T* Singleton<T>::m_pInstance = NULL;

	template <class T>
	inline T* Singleton<T>::Instance()
	{
		if ( NULL == m_pInstance )
		{
			if ( NULL == m_pInstance )
			{
				m_pInstance = new T;
			}
		}
		return m_pInstance;
	}


}