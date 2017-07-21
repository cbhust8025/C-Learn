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
		virtual ~Singleton() {} // ����������ȷ��ʹ�û���ָ��ָ�����������ʱ��������ȷ�������������������ͷ��ڴ�
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