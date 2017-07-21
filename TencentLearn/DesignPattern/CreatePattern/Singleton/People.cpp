#include "stdafx.h"
#include "People.h"
#include<iostream>

People::People()
	:m_nWeight(0)
{
}

People::People(int nWeight, std::string& stdName)
	:m_nWeight(nWeight), m_strName(stdName)
{
}

People::~People()
{
}

int People::GetWeight()
{
	return m_nWeight;
}

std::string People::GetName()
{
	return m_strName;
}

void People::SetWeight(int iWeight)
{
	m_nWeight = iWeight;
}
void People::SetName(std::string& stdName)
{
	m_strName = stdName;
}

void People::ShowPeople()
{
	std::cout << "People Name: " << m_strName << "-- Weight:" << m_nWeight << std::endl;
}