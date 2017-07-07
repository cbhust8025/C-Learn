#include "stdafx.h"
#include "VectorHelper.h"


VectorHelper::VectorHelper()
{
}


VectorHelper::~VectorHelper()
{
}

BOOL VectorHelper::GetCommonID(const vecID vi1, const vecID vi2, DWORD& dwCommonID)
{// 找到两个ID集的第一个共同ID，存入dwCommonID
	// 如果两个ID集有一个空集，则直接返回FALSE
	if (vi1.empty() || vi2.empty())
		return FALSE;
	for (auto it1 = vi1.begin(); it1 != vi1.end(); it1++)
	{
		for (auto it2 = vi2.begin(); it2 != vi2.end(); it2++)
		{
			if (*it1 == *it2)
			{
				dwCommonID = *it1;
				return TRUE;
			}
		}
	}
	return FALSE;
}