#pragma once
class MapHelper
{
protected:
	BOOL bTrayFlag;
public:
	mhhMAP WndMap;
public:
	MapHelper(BOOL bFlag);
	~MapHelper();
	BOOL IsTrayMap();
	BOOL printMap();
	BOOL printMap(const mhhMAP& mhhmap);
	BOOL GetMap(mhhMAP& mhhmap);
	BOOL AddToMap(HWND NowhWnd, HWND ParhWnd, TString WndName, TString ClassName);
};

