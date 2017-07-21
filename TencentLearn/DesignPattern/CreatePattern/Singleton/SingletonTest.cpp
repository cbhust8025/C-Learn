// SingletonTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "People.h"


int main()
{
	std::string str1 = "cheng bo";
	std::string str2 = "lealcheng";
	/*People people1;
	People people2;

	people1.SetName(str1);
	people1.SetWeight(100);
	people2.SetName(str2);
	people2.SetWeight(90);

	people1.ShowPeople();
	people2.ShowPeople();*/
	SPeople::Instance()->ShowPeople();
	SPeople::Instance()->SetName(str1);
	SPeople::Instance()->ShowPeople();

    return 0;
}

