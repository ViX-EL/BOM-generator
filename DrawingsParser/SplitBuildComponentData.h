#pragma once

#ifndef SplitBuildComponentData_h__
#define SplitBuildComponentData_h__

#include <string>
#include <regex>
#include <map>
#include <string_view>

class BuildComponent;

class SplitBuildComponentData
{
public:
	explicit SplitBuildComponentData(const BuildComponent* componentPtr);

	std::wstring_view getElementName() const;
	std::wstring_view getType1() const;
	std::wstring_view getType2() const;
	std::wstring_view getType3() const;
	std::wstring_view getProductStandard() const;
	std::wstring_view getProductStandardENG() const;
	std::wstring_view getSteelGrade() const;
	std::wstring_view getSteelGradeENG() const;
	std::wstring_view getDiameter1() const;
	std::wstring_view getDiameter2() const;
	std::wstring_view getWallThickness1() const;
	std::wstring_view getWallThickness2() const;
	std::wstring_view getNominalDiameter1() const;
	std::wstring_view getNominalDiameter2() const;
	double getPressureNominal() const;
	int getPressureClass() const;
	std::wstring_view getASMEThickness1() const;
	std::wstring_view getASMEThickness2() const;
	void setSteelGrade(const std::wstring& steelGrade);
	void parse();

private:
	std::wstring elementName{ L"-" };
	std::wstring type1{ L"-" };
	std::wstring type2{ L"-" };
	std::wstring type3{ L"-" };
	std::wstring productStandard{ L"-" };
	std::wstring productStandardENG{ L"-" };
	std::wstring steelGrade{ L"-" };
	std::wstring steelGradeENG{ L"-" };
	std::wstring diameter1{ L"-" };
	std::wstring diameter2{ L"-" };
	std::wstring wallThickness1{ L"-" };
	std::wstring wallThickness2{ L"-" };
	std::wstring nominalDiameter1{ L"-" };
	std::wstring nominalDiameter2{ L"-" };
	double pressureNominal{ 0 }; //PN
	int pressureClass{ 0 }; //CL
	std::wstring ASMEThickness1{ L"-" };
	std::wstring ASMEThickness2{ L"-" };

	const BuildComponent* componentPtr{ nullptr };

	static inline std::map<std::wstring, std::wregex> elementNamePatterns {
		{L"Заглушка поворотная", std::wregex(L"^Заглушка поворотная|SPECTACLE BLIND|заглушка-кольцо|ПОВОРОТНАЯ ЗАГЛУШКА", std::regex::icase)},
		{L"Заглушка", std::wregex(L"cap|Blind P|Bottom", std::regex::icase)},
		{L"Муфта", std::wregex(L"Coupling", std::regex::icase)},
		{L"Переходная муфта", std::wregex(L"^Reducing coupling|MIFV|MIMV")},
		{L"Заглушка фланцевая", std::wregex(LR"(Blind flange|Blind [\d,-]+|Flange Blind)")},
		{L"Крепление для труб", std::wregex(L"^Clip")},
		{L"Отвод", std::wregex(L"Elbow", std::regex::icase)},
		{L"Фланец", std::wregex(LR"(Flange[^d]|Фланец)", std::regex::icase)},
		{L"Штуцер", std::wregex(L"Nozzle|Штуцер")},
		{L"Накладка", std::wregex(L"Pad")},
		{L"Труба", std::wregex(L"Pipe|Tube|Труба", std::regex::icase)},
		{L"Концентрический переход", std::wregex(LR"((?:Reducer/)?Переход (?:К|концентрический)|Reducer (?:P\*|П)? ?[КK]?T?(?:(?:\dx)+)?|Concentric Reducer)", std::regex::icase)},
		{L"Эксцентрический переход", std::wregex(LR"((?:Reducer/)?Переход (?:Э|ЭКСЦЕНТРИЧЕСКИЙ)|Reducer (?:P?\*?|П?Э)? ?E?S?H?T?|Eccentric Reducer)", std::regex::icase)},
		{L"Опора", std::wregex(L"Support|Hanger|ОПОРА", std::regex::icase)},
		{L"Тройник", std::wregex(L"Tee|Тройник", std::regex::icase)},
		{L"Велдолет", std::wregex(L"WELDOLET")},
		{L"Бурт", std::wregex(L"^Lap joint|Втулка")},
		{L"Ответвление", std::wregex(L"Branch")},
		{L"Прокладка", std::wregex(L"Gasket", std::regex::icase)},
		{L"Шпилька", std::wregex(L"[Ss]tud", std::regex::icase)},
		{L"Гайка", std::wregex(L"Nut")},
		{L"Болт", std::wregex(L"Bolt")},
		{L"Кожух", std::wregex(L"casing|Кожух")},
		{L"Рукав", std::wregex(L"[Ss]leeve")},
		{L"Шайба", std::wregex(L"[Ww]asher")},
		{L"Камлок", std::wregex(L"камлок|Camlock")},
		{L"Звеньевой уплотнитель", std::wregex(L"LINK-SEAL")},
		{L"Кольцо", std::wregex(L"Кольцо|ring")},
		{L"Ниппель", std::wregex(L"Ниппель|Nipple", std::regex::icase)}
	};

	void parseElementName();
	void parseType1();
	void parseType2();
	void parseType3();
	void parseType2Tee();
	void parseType1Tee();
	//void parseType2Support();
	void parseType1BLIND_FLANGE();
	void parseType2BLIND_FLANGE();
	void parseType2ELBOW();
	void parseType2FLANGE();
	void parseType2PIPE();
	void parseType2REDUCER_ECC();
	void parseType2REDUCER_CON();
	void parseType3REDUCER_ECC();
	void parseType2Cap();
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
	std::wstring searchDescriptionMatch(const std::wregex& pattern, int matchIndex = 0);
	std::wstring searchDocumentMatch(const std::wregex& pattern);
	std::wstring searchMatch(const std::wregex& pattern);
};

#endif // SplitBuildComponentData_h__

