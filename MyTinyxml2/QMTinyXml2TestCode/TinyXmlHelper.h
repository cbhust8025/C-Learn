#pragma once
#include "stdafx.h"
#include "Singleton.h"

class CTinyXmlHelper
	: public utils::Singleton<CTinyXmlHelper>
{
public:
	CTinyXmlHelper(void);
	~CTinyXmlHelper(void);

public:
	BOOL TestProcessing();  // 整个测试流程
	BOOL TiXmlNodeTest(); // 测试TiXmlNode类方法
	BOOL TiXmlElementTest(); // 测试TiXmlElement类方法
	BOOL TiXmlDocumentTest(); // 测试TiXmlDocument类方法

private:
	BOOL InitXmls(); // 创建三个xml文件并指定初始化选项为False,向日志写入开始测试
	BOOL UninitXmls(); // 反初始化，向日志写入测试完毕
	BOOL ResetXmls(); // 从Config.xml中读取初始化xmls配置
	BOOL LoadXmlFilesName(); // 从Config.xml中读取xml文件名
	BOOL LoadXmls(); // 载入三个xml文件并重置三个xml文件为指定内容并置bInitFlag = TRUE
	BOOL SaveXmls(); // 保存三个xml文件并置bInitFlag = FALSE

private:
	TCHAR* m_pbyConfigFileName;
	// 初始化三个xml文档类型指针，分别用于初始化
	TiXmlDocument* m_pDocument1;
	TiXmlDocument* m_pDocument2;
	TiXmlDocument* m_pDocument3;
	// 三个xml文件名（宽字节） 已存入Config.xml文件中
	/*TCHAR* p_byFileName1;
	TCHAR* p_byFileName2;
	TCHAR* p_byFileName3;*/
	vecString m_vecXmlFilesName; // 创建存储xml文件名的数组
};

//CTinyXmlHelper::Instance();