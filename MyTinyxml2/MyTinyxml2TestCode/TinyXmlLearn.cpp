// TinyXmlTools.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR* filename = _T("test.xml");

	//// ��ʼ����־
	//TLOG_INIT(NULL);
	//TLOG_UPDATELEVEL(MODULELOG_LEVEL_INFO);


	// //��ʼ��xml��������
	//XmlProcess oXmlProcess;

	// //��ʼ���ĵ�����ָ��
	//if(oXmlProcess.InitXml())
	//{
	//	TLOG_INFO(TEXT("Init XmlDocument Point Successful~"));
	//}
	//else
	//	TLOG_ERROR(TEXT("Init DoPoint Failed~"));
	//
	//���XMl�ĸ��ڵ�


	//oXmlProcess.Output(filename);
	/*FILE* fp = 0;
	int err = _wfopen_s(&fp,  TEXT("test.xml"), TEXT("w"));
	std::cout << err << std::endl;
	*/

	TiXmlDocument *pDocument = new TiXmlDocument;
	TiXmlElement * e1 = new TiXmlElement("newElement1");  // ʵ����һ��Element�����

	e1->SetAttribute("href", "www.baidu.com");  // ����Element���SetAttribute����Ϊe1��������������ֵ
	e1->SetAttribute("Text", "asd");  // ����Element���SetAttribute����Ϊe1��������������ֵ

	TiXmlText * e1text = new TiXmlText("element1");  // û��ר�������ı��ķ�����������ҪΪһ���ڵ�����ı��ڵ���ܲ���һ���ڵ���ı�
	e1->LinkEndChild(e1text);  // ����stl��������append������

	TiXmlElement * e2 = new TiXmlElement("newElement2");  // ʵ����һ��Element�����
	e2->SetAttribute("href", "www.google.com");  // ����Element���SetAttribute����Ϊe1��������������ֵ
	e2->SetAttribute("Text", "dsa");  // ����Element���SetAttribute����Ϊe1��������������ֵ

	TiXmlElement * e3 = new TiXmlElement("newElement3");  // ʵ����һ��Element�����
	e3->SetAttribute("href", "www.tencent.com");  // ����Element���SetAttribute����Ϊe1��������������ֵ
	e3->SetAttribute("Text", "sad");  // ����Element���SetAttribute����Ϊe1��������������ֵ

	pDocument->LinkEndChild(e1);  // ��������Element�������ӵ�doc�������档
	e1->LinkEndChild(e2);  // ��������Element�������ӵ�e1�������档
	e2->LinkEndChild(e3);  // ��������Element�������ӵ�e2�������档
						   // SaveFile����������Ҫ������������ʽ��һ�ֽ����ַ�����Ϊ������ļ�����
						   // ��һ�ֽ���һ��FILE�����������ĵ����󵽴��ļ������У�Ҳ����һ�����غ�����ʵ�ʵ��õĺ�����

	pDocument->SaveFile(filename);

	//pDocument->LoadFile(filename);
	//pDocument->FirstChild
	return 0;
}

