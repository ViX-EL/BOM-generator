#pragma once

#include <string>
#include <regex>
#include <map>

class BuildComponent;

class SplitBuildComponentData
{
public:
	SplitBuildComponentData(const BuildComponent* componentPtr);

	const std::wstring& getElementName() const;
	const std::wstring& getType1() const;
	const std::wstring& getType2() const;
	const std::wstring& getType3() const;
	const std::wstring& getProductStandard() const;
	const std::wstring& getProductStandardENG() const;
	const std::wstring& getSteelGrade() const;
	const std::wstring& getSteelGradeENG() const;
	double getDiameter1() const;
	double getDiameter2() const;
	double getWallThickness1() const;
	double getWallThickness2() const;
	int getNominalDiameter1() const;
	int getNominalDiameter2() const;
	double getPressureNominal() const;
	int getPressureClass() const;
	const std::wstring& getASMEThickness1() const;
	const std::wstring& getASMEThickness2() const;

private:
	std::wstring elementName{ L"-" };
	std::wstring type1{ L"-" };
	std::wstring type2{ L"-" };
	std::wstring type3{ L"-" };
	std::wstring productStandard{ L"-" };
	std::wstring productStandardENG{ L"-" };
	std::wstring steelGrade{ L"-" };
	std::wstring steelGradeENG{ L"-" };
	double diameter1{ 0.0 };
	double diameter2{ 0.0 };
	double wallThickness1{ 0.0 };
	double wallThickness2{ 0.0 };
	int nominalDiameter1{ 0 };
	int nominalDiameter2{ 0 };
	double pressureNominal{ 0 }; //PN
	int pressureClass{ 0 }; //CL
	std::wstring ASMEThickness1{ L"-" };
	std::wstring ASMEThickness2{ L"-" };

	const BuildComponent* componentPtr{ nullptr };

	static inline std::map<std::wstring, std::wregex> elementNamePatterns {
		{L"SPECTACLE BLIND", std::wregex(L"^Spectacle blind")},
		{L"CAP", std::wregex(L"cap|Blind P|Bottom", std::regex::icase)},
		{L"Coupling", std::wregex(L"^Coupling")},
		{L"BLIND FLANGE", std::wregex(LR"(Blind flange|Blind [\d,-]+|Flange Blind)")},
		{L"Clip", std::wregex(L"^Clip")},
		{L"ELBOW", std::wregex(L"Elbow", std::regex::icase)},
		{L"FLANGE", std::wregex(LR"(Flange[^d])", std::regex::icase)},
		{L"NOZZLE", std::wregex(L"Nozzle|Штуцер")},
		{L"PAD", std::wregex(L"Pad")},
		{L"PIPE", std::wregex(L"Pipe|Tube", std::regex::icase)},
		{LR"(REDUCER CON)", std::wregex(LR"(Reducer/Переход (?:К|концентрический)|Reducer (?:P\*|П)? ?[КK]?T?(?:(?:\dx)+)?|Concentric Reducer)", std::regex::icase)},
		{LR"(REDUCER ECC)", std::wregex(LR"(Reducer/Переход Э|Reducer (?:P?\*?|П?Э)? ?E?S?H?T?|Eccentric Reducer)", std::regex::icase)},
		{L"Support", std::wregex(L"Support|Clamp")},
		{L"MIMV", std::wregex(L"MIMV")},
		{L"MIFV", std::wregex(L"MIFV")},
		{L"TEE", std::wregex(L"Tee", std::regex::icase)},
		{L"WELDOLET", std::wregex(L"WELDOLET")},
		{L"Lap joint", std::wregex(L"^Lap joint")},
		{L"Reducing coupling", std::wregex(L"^Reducing coupling")}
	};

	void parseElementName();
	void parseType1();
	void parseType2();
	void parseType3();
	void parseType2Tee();
	void parseType1Tee();
	void parseType2Support();
	void parseType1BLIND_FLANGE();
	void parseType2ELBOW();
	void parseType2FLANGE();
	void parseType2PIPE();
	void parseType2REDUCER_ECC();
	void parseType2REDUCER_CON();
	void parseType2CAP();
	void parseType1SPECTACLE_BLIND();
	void parseProductStandard();
	void parseSteelGrade();
	void parseDiameter1();
	void parseDiameter2();
	void parseNominalDiameter1();
	void parseNominalDiameter2();
	void parseWallThickness1();
	void parseWallThickness2();
	void parsePressureNominal();
	void parsePressureClass();
	void parseASMEThickness1();
	void parseASMEThickness2();
	void parse();
	std::wstring searchDescriptionMatch(const std::wregex& pattern, int matchIndex = 0);
	std::wstring searchDocumentMatch(const std::wregex& pattern);
	std::wstring searchMatch(const std::wregex& pattern);
};

