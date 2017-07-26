#include "StdAfx.h"
#include "TinyXmlHelper.h"

CTinyXmlHelper::CTinyXmlHelper(void)
:m_pbyConfigFileName(TEXT("Config.xml")),
m_pDocument1(new TiXmlDocument),
m_pDocument2(new TiXmlDocument),
m_pDocument3(new TiXmlDocument)
{
}

CTinyXmlHelper::~CTinyXmlHelper(void)
{
	delete m_pbyConfigFileName;
	m_pbyConfigFileName = NULL;
	delete m_pDocument1;
	m_pDocument1 = NULL;
	delete m_pDocument2;
	m_pDocument2 = NULL;
	delete m_pDocument3;
	m_pDocument3 = NULL;
}


BOOL CTinyXmlHelper::LoadXmlFilesName()
{
	// 从Config.xml中读取xml文件名
	BOOL bRes = FALSE;
	TiXmlDocument* pConfigDocument = new TiXmlDocument;
	TiXmlElement* pElement = NULL;
	TiXmlAttribute* pAttributeTest = NULL;
	SString strElementName;

	m_vecXmlFilesName.clear();
	bRes = pConfigDocument->LoadFile(m_pbyConfigFileName);
	if(bRes)
	{
		TLOG_INFO(TEXT("Load the config file successful~"));
		pElement = pConfigDocument->FirstChildElement();
		pAttributeTest = pElement->FirstAttribute();
		strElementName = pAttributeTest->Name();
		if(pElement->NoChildren())
		{
			bRes = FALSE;
			TLOG_ERROR(TEXT("Load the configure failed~"));
		}
		else
		{
			pElement = pElement->FirstChildElement();
			pAttributeTest = pElement->FirstAttribute();
			strElementName = pAttributeTest->Name();
			while(NULL != pElement)
			{
				strElementName = pElement->Attribute("name");
				m_vecXmlFilesName.push_back(strElementName);
				pElement = pElement->NextSiblingElement();
			}

			TLOG_INFO(TEXT("Load the configure successful~"));
		}
	}
	else
	{
		TLOG_ERROR(TEXT("Load the config file failed~"));
	}
	
	return bRes;
}

BOOL CTinyXmlHelper::InitXmls()
{
	// 创建三个xml文件并指定初始化选项为False,向日志写入开始测试
	BOOL bRes = FALSE;
	const char* declaration ="<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"; // xml声明
	TiXmlDocument* pDocument = new TiXmlDocument;

	pDocument->Parse(declaration);//会覆盖xml所有内容
	bRes = LoadXmlFilesName();
	if(bRes && !m_vecXmlFilesName.empty())
	{
		// 如果载入三个xml文件名，则创建之
		for(UINT i = 0;i < m_vecXmlFilesName.size();i++)
		{
			pDocument->SaveFile(m_vecXmlFilesName[i]);
		}
		TLOG_INFO(TEXT("Create the files successful~"));
	}
	else
	{
		bRes = FALSE;
		TLOG_ERROR(TEXT("Create the files failed~"));
	}

	return bRes;
}

BOOL CTinyXmlHelper::LoadXmls()
{
	// 载入三个xml文件并重置三个xml文件为指定内容
	BOOL bRes = FALSE;

	if(m_pDocument1->LoadFile(m_vecXmlFilesName[0].c_str()) && 
		m_pDocument2->LoadFile(m_vecXmlFilesName[1].c_str()) && 
		m_pDocument3->LoadFile(m_vecXmlFilesName[2].c_str()))
	{
		bRes = ResetXmls();
		TLOG_INFO(TEXT("Load the files successful~"));
	}
	else
	{
		bRes = FALSE;
		TLOG_ERROR(TEXT("Load the files failed~"));
	}

	return bRes;
}

BOOL CTinyXmlHelper::SaveXmls()
{
	// 保存三个xml文件
	BOOL bRes = FALSE;

	if(m_pDocument1->SaveFile(m_vecXmlFilesName[0]) &&
		m_pDocument2->SaveFile(m_vecXmlFilesName[1]) && 
		m_pDocument3->SaveFile(m_vecXmlFilesName[2]))
	{
		bRes = TRUE;
		TLOG_INFO(TEXT("Save the files successful~"));	
	}
	else
	{
		TLOG_ERROR(TEXT("Save the files failed~"));
	}

	return bRes;
}

BOOL CTinyXmlHelper::UninitXmls()
{
	// 反初始化，向日志写入测试完毕
	return TRUE;
}

BOOL CTinyXmlHelper::ResetXmls()
{
	// 从Config.xml中读取初始化xmls配置
	BOOL bRes = FALSE;
	TiXmlDocument* pConfigDocument = new TiXmlDocument;

	bRes = pConfigDocument->LoadFile(m_pbyConfigFileName);
	if(bRes)
	{
		TLOG_INFO(TEXT("Load the config file successful~"));
		if(pConfigDocument->NoChildren())
		{
			bRes = FALSE;
			TLOG_ERROR(TEXT("Load the configure failed~"));
		}
		else
		{
			TiXmlElement* pElement = NULL;
			TiXmlElement* pElementTemp = NULL;
			m_pDocument1->Clear();
			m_pDocument2->Clear();
			m_pDocument3->Clear();

			pElement = pConfigDocument->FirstChildElement()->FirstChildElement();
			pElementTemp = pElement->FirstChildElement();
			while(NULL != pElementTemp)
			{
				m_pDocument1->LinkEndChild(new TiXmlElement(*pElementTemp));
				pElementTemp = pElementTemp->NextSiblingElement();
			}

			pElement = pElement->NextSiblingElement();
			pElementTemp = pElement->FirstChildElement();
			while(NULL != pElementTemp)
			{
				m_pDocument2->LinkEndChild(new TiXmlElement(*pElementTemp));
				pElementTemp = pElementTemp->NextSiblingElement();
			}

			pElement = pElement->NextSiblingElement();
			pElementTemp = pElement->FirstChildElement();
			while(NULL != pElementTemp)
			{
				m_pDocument3->LinkEndChild(new TiXmlElement(*pElementTemp));
				pElementTemp = pElementTemp->NextSiblingElement();
			}

			bRes = SaveXmls();
			TLOG_INFO(TEXT("Load the configure successful~"));
		}
	}
	else
	{
		TLOG_ERROR(TEXT("Load the config file failed~"));
	}

	return bRes;
}

BOOL CTinyXmlHelper::TiXmlNodeTest()
{
	// 测试TiXmlNode类方法
	BOOL bRes = FALSE;
	TiXmlElement* pElement1Test = NULL;
	TiXmlElement* pElement2Test = NULL;
	TiXmlElement* pElement2TestNext = NULL;

	//////////////////////////////////////////////////////////////////////////
	// 开始载入xml文件。。。
	TLOG_INFO(_T("Start the Loading~"));

	bRes = LoadXmls();
	if(!bRes)
	{
		TLOG_ERROR(TEXT("Failed to Load xml."));
		return bRes;
	}
	TLOG_INFO(TEXT("Success to Load xml."));

	TLOG_INFO(_T("End the Loading~"));	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////Test////////////////////////////////////////
	pElement1Test = m_pDocument1->FirstChildElement();
	pElement1Test->SetValue("ElementTest");
	pElement1Test = pElement1Test->NextSiblingElement()->FirstChildElement()->Parent()->ToElement();
	pElement2Test = m_pDocument2->FirstChildElement();
	pElement2TestNext = pElement2Test->NextSiblingElement()->PreviousSibling()->NextSibling()->ToElement();
	m_pDocument2->ReplaceChild(pElement2Test, *pElement1Test);
	m_pDocument2->RemoveChild(pElement2TestNext);

	pElement1Test = pElement1Test->FirstChild()->ToElement();
	pElement2Test = m_pDocument2->LastChild()->ToElement();
	m_pDocument2->ReplaceChild(pElement2Test, *pElement1Test);

	m_pDocument2 = m_pDocument2->FirstChildElement()->GetDocument();
	//pElement2Test = m_pDocument2->FirstChild()->ToElement();
	std::cout << pElement2Test->NoChildren() << std::endl;

	//////////////////////////////////Test////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 开始保存xml文件。。。
	TLOG_INFO(_T("Start the Saving~"));

	bRes = SaveXmls();
	if(!bRes)
	{
		TLOG_ERROR(TEXT("Failed to Save xml."));
		
	}
	TLOG_INFO(TEXT("Suc to Save xml."));

	TLOG_INFO(_T("End the Saving~"));	
	//////////////////////////////////////////////////////////////////////////

	return bRes;
}

BOOL CTinyXmlHelper::TiXmlElementTest()
{
	// 测试TiXmlElement类方法
	BOOL bRes = FALSE;
	int ID = 1;
	int iElement2TestQueryInt = 0;
	std::string pElement2TestName;
	std::string pElement2TestText;
	const char* chNewElement ="<Element7 name=\"element7\" id=\"7\" >";
	TiXmlAttribute* pAttributeTest = NULL;
	TiXmlElement* pElement1Test = NULL;
	TiXmlElement* pElement2Test = NULL;
	TiXmlElement* pElement2TestNext = new TiXmlElement("123");

	//////////////////////////////////////////////////////////////////////////
	// 开始载入xml文件。。。
	TLOG_INFO(_T("Start the Loading~"));

	bRes = LoadXmls();
	if(!bRes)
	{
		TLOG_ERROR(TEXT("Failed to Load"));
		return bRes;
	}
	TLOG_INFO(TEXT("Success to Load~"));

	TLOG_INFO(_T("End the Loading~"));	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////Test////////////////////////////////////////
	pElement1Test = m_pDocument1->FirstChildElement();
	pElement2Test = m_pDocument2->FirstChildElement();
	pElement2TestName = pElement2Test->Attribute("name");
	std::cout << "pElement2TestName:" << pElement2TestName << std::endl;
	iElement2TestQueryInt = pElement2Test->QueryIntAttribute("id", &ID);
	std::cout << "iElement2TestQueryInt: " << iElement2TestQueryInt << std::endl;
	pElement2Test->RemoveAttribute("id");
	pAttributeTest = pElement2Test->NextSiblingElement()->FirstAttribute();
	std::cout << "pElement2Test second element FirstAttribute: " << pAttributeTest->Name() << std::endl;
	pAttributeTest = pElement2Test->NextSiblingElement()->LastAttribute();
	std::cout << "pElement2Test second element LastAttribute: " << pAttributeTest->Name() << std::endl;
	pElement2TestText = pElement2Test->FirstChildElement()->GetText();
	std::cout << "pElement2Test first child element Text: " << pElement2TestText << std::endl;
	pElement2TestNext->Parse(chNewElement, NULL, TIXML_DEFAULT_ENCODING);
	m_pDocument2->LinkEndChild(pElement2TestNext);

	//////////////////////////////////Test////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 开始保存xml文件。。。
	TLOG_INFO(_T("Start the Saving~"));

	bRes = SaveXmls();
	if(!bRes)
	{
		TLOG_ERROR(TEXT("Failed to Save"));
		return bRes;
	}
	TLOG_INFO(TEXT("Success to Save~"));

	TLOG_INFO(_T("End the Saving~"));	
	//////////////////////////////////////////////////////////////////////////


	return bRes;
}

BOOL CTinyXmlHelper::TiXmlDocumentTest()
{
	// 测试TiXmlDocument类方法
	BOOL bRes = FALSE;
	const char* chNewElement ="<Element7 name=\"element7\" id=\"7\" >";
	TiXmlElement* pElement1Test = NULL;
	TiXmlElement* pElement2Test = NULL;
	const char* chOutputFileName = "testdocument3.xml";

	//////////////////////////////////////////////////////////////////////////
	// 开始载入xml文件。。。
	TLOG_INFO(_T("Start the Loading~"));

	bRes = LoadXmls();
	if(!bRes)
	{
		TLOG_ERROR(TEXT("Failed to Load"));
		return bRes;
	}
	TLOG_INFO(TEXT("Success to Load~"));

	TLOG_INFO(_T("End the Loading~"));	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////Test////////////////////////////////////////
	//m_pDocument3 = new TiXmlDocument(*pDocument1);
	/*m_pDocument3 = new TiXmlDocument("testdocument3.xml");
	m_pDocument3->SaveFile();*/
	pElement1Test = m_pDocument1->RootElement();
	std::cout << "m_pDocument1 RootElement href:" << pElement1Test->Attribute("href") << std::endl;
	std::cout << "m_pDocument3 TabSize: " << m_pDocument3->TabSize() << std::endl;
	m_pDocument3->SetTabSize(15);
	std::cout << "m_pDocument3 TabSize: " << m_pDocument3->TabSize() << std::endl;

	if( m_pDocument3->Error() ) 
		std::cout << "m_pDocument1 ErrorDesc: " << m_pDocument3->ErrorDesc() << std::endl;
	FILE * cf = 0;
	fopen_s(&cf, chOutputFileName, "w+");
	m_pDocument3->Print( cf, 1);
	m_pDocument3->SaveFile(m_vecXmlFilesName[2]);
	//////////////////////////////////Test////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 开始保存xml文件。。。
	TLOG_INFO(_T("Start the Saving~"));
	bRes = SaveXmls();
	if(!bRes)
	{
		TLOG_ERROR(TEXT("Failed to Save"));
		return bRes;
	}
	TLOG_INFO(TEXT("Success to Save~"));
	TLOG_INFO(_T("End the Saving~"));	
	//////////////////////////////////////////////////////////////////////////
	return bRes;
}

BOOL CTinyXmlHelper::TestProcessing()
{
	// 整个测试流程：Node->Element->Document
	BOOL bRes = FALSE;

	//////////////////////////////////////////////////////////////////////////
	// 开始初始化xml文件
	TLOG_INFO(_T("Start the initialization~"));

	bRes = InitXmls();
	if(!bRes)
	{
		TLOG_ERROR(TEXT("Failed to Init~"));
		return bRes;
	}
	TLOG_INFO(TEXT("Successful Init ~"));

	TLOG_INFO(_T("End the initialization~"));	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 测试TiXmlNode类 方法
	TLOG_INFO(_T("Start to test TiXmlNode function~"));

	bRes = TiXmlNodeTest();

	if(!bRes)
	{
		TLOG_ERROR(TEXT("Testing TiXmlNode function failed to pass~"));
		return bRes;
		
	}
	TLOG_INFO(TEXT("Testing TiXmlNode function pass~"));

	TLOG_INFO(_T("End to test TiXmlNode function~"));	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 测试TiXmlElement类 方法
	TLOG_INFO(_T("Start to test TiXmlElement function~"));

	bRes = TiXmlElementTest();
	if(!bRes)
	{
		TLOG_ERROR(TEXT("Testing TiXmlElement function failed to pass~"));
		return bRes;
	}
	TLOG_INFO(TEXT("Testing TiXmlElement function pass~"));

	TLOG_INFO(_T("End to test TiXmlElement function~"));	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 测试TiXmlDocument类 方法
	TLOG_INFO(_T("Start to test TiXmlDocument function~"));

	bRes = TiXmlDocumentTest();
	if(!bRes)
	{
		TLOG_ERROR(TEXT("Testing TiXmlDocument function failed to pass~"));
		return bRes;
	}
	TLOG_INFO(TEXT("Testing TiXmlDocument function pass~"));

	TLOG_INFO(_T("End to test TiXmlDocument function~"));	
	//////////////////////////////////////////////////////////////////////////

	return bRes;
}