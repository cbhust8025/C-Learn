// TinyXmlTools.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR* filename = _T("test.xml");

	//// 初始化日志
	//TLOG_INIT(NULL);
	//TLOG_UPDATELEVEL(MODULELOG_LEVEL_INFO);


	// //初始化xml操作对象
	//XmlProcess oXmlProcess;

	// //初始化文档对象指针
	//if(oXmlProcess.InitXml())
	//{
	//	TLOG_INFO(TEXT("Init XmlDocument Point Successful~"));
	//}
	//else
	//	TLOG_ERROR(TEXT("Init DoPoint Failed~"));
	//
	//添加XMl的根节点


	//oXmlProcess.Output(filename);
	/*FILE* fp = 0;
	int err = _wfopen_s(&fp,  TEXT("test.xml"), TEXT("w"));
	std::cout << err << std::endl;
	*/

	TiXmlDocument *pDocument = new TiXmlDocument;
	TiXmlElement * e1 = new TiXmlElement("newElement1");  // 实例化一个Element类对象

	e1->SetAttribute("href", "www.baidu.com");  // 利用Element类的SetAttribute方法为e1对象设置其属性值
	e1->SetAttribute("Text", "asd");  // 利用Element类的SetAttribute方法为e1对象设置其属性值

	TiXmlText * e1text = new TiXmlText("element1");  // 没有专门设置文本的方法，所以需要为一个节点添加文本节点才能插入一个节点的文本
	e1->LinkEndChild(e1text);  // 类似stl库容器的append方法。

	TiXmlElement * e2 = new TiXmlElement("newElement2");  // 实例化一个Element类对象
	e2->SetAttribute("href", "www.google.com");  // 利用Element类的SetAttribute方法为e1对象设置其属性值
	e2->SetAttribute("Text", "dsa");  // 利用Element类的SetAttribute方法为e1对象设置其属性值

	TiXmlElement * e3 = new TiXmlElement("newElement3");  // 实例化一个Element类对象
	e3->SetAttribute("href", "www.tencent.com");  // 利用Element类的SetAttribute方法为e1对象设置其属性值
	e3->SetAttribute("Text", "sad");  // 利用Element类的SetAttribute方法为e1对象设置其属性值

	pDocument->LinkEndChild(e1);  // 将建立的Element类对象添加到doc类的最后面。
	e1->LinkEndChild(e2);  // 将建立的Element类对象添加到e1类的最后面。
	e2->LinkEndChild(e3);  // 将建立的Element类对象添加到e2类的最后面。
						   // SaveFile函数具有主要的两种重载形式，一种接受字符串作为保存的文件名，
						   // 另一种接受一个FILE对象来保存文档对象到此文件对象中，也是上一种重载函数中实际调用的函数。

	pDocument->SaveFile(filename);

	//pDocument->LoadFile(filename);
	//pDocument->FirstChild
	return 0;
}

