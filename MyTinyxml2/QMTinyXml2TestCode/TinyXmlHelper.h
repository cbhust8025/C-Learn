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
	BOOL TestProcessing();  // ������������
	BOOL TiXmlNodeTest(); // ����TiXmlNode�෽��
	BOOL TiXmlElementTest(); // ����TiXmlElement�෽��
	BOOL TiXmlDocumentTest(); // ����TiXmlDocument�෽��

private:
	BOOL InitXmls(); // ��������xml�ļ���ָ����ʼ��ѡ��ΪFalse,����־д�뿪ʼ����
	BOOL UninitXmls(); // ����ʼ��������־д��������
	BOOL ResetXmls(); // ��Config.xml�ж�ȡ��ʼ��xmls����
	BOOL LoadXmlFilesName(); // ��Config.xml�ж�ȡxml�ļ���
	BOOL LoadXmls(); // ��������xml�ļ�����������xml�ļ�Ϊָ�����ݲ���bInitFlag = TRUE
	BOOL SaveXmls(); // ��������xml�ļ�����bInitFlag = FALSE

private:
	TCHAR* m_pbyConfigFileName;
	// ��ʼ������xml�ĵ�����ָ�룬�ֱ����ڳ�ʼ��
	TiXmlDocument* m_pDocument1;
	TiXmlDocument* m_pDocument2;
	TiXmlDocument* m_pDocument3;
	// ����xml�ļ��������ֽڣ� �Ѵ���Config.xml�ļ���
	/*TCHAR* p_byFileName1;
	TCHAR* p_byFileName2;
	TCHAR* p_byFileName3;*/
	vecString m_vecXmlFilesName; // �����洢xml�ļ���������
};

//CTinyXmlHelper::Instance();