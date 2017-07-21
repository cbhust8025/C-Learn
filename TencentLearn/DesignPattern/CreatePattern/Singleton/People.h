#pragma once
#include <string>
#include "Singleton.h"
class People : public utils::Singleton<People>
{
public:
	People();
	People(int nWeight, std::string& stdName);
	~People();
	int GetWeight();
	std::string GetName();
	void SetWeight(int iWeight);
	void SetName(std::string& stdName);
	void ShowPeople();
private:
	int m_nWeight;
	std::string m_strName;
};

typedef utils::Singleton<People> SPeople;