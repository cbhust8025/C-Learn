#pragma once
class VectorHelper
{
public:
	VectorHelper();
	~VectorHelper();

	template <typename T>
	BOOL PrintVector(const std::vector<T>& vT)
	{// ��ӡvector���������Ϊ0����False�����򷵻�True����ӡvector
		if (vT.empty())
			return FALSE;
		for (auto it = vT.begin(); it != vT.end(); it++)
		{
			COUT << *it << "  ";
		}
		COUT << "end" << ENDL;
		return TRUE;
	}

	BOOL GetCommonID(const vecID vi1, const vecID vi2, DWORD& dwCommonID);
};

