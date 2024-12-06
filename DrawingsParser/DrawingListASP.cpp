#include "DrawingListASP.h"

bool DrawingListASP::trySetOperatingTemperature(const std::wstring& operatingTemperatureStr, bool assertionCheck)
{
	if (operatingTemperatureStr == L"окр.среды") {
		operatingTemperature = L"env./окр.ср.";
	}
	return trySetValue(operatingTemperatureStr, operatingTemperature, std::wregex(LR"(-?\d+(\.\d+\/\d+)?|env\.\/окр\.ср\.)"), assertionCheck,
		"Недопустимое значение для рабочей температуры!");
}

bool DrawingListASP::trySetOperatingPressure(const std::wstring& operatingPressureStr, bool assertionCheck)
{
	return trySetValue(operatingPressureStr, operatingPressure, std::wregex(LR"(\d{1}(\.\d{1,2})?|0\.3EX-01|атм\.)"), assertionCheck, "Недопустимое значение для рабочего давления!");
}

bool DrawingListASP::trySetTracing(const std::wstring& tracingStr, bool assertionCheck)
{
	if (tracingStr == L"unset/не уст.") {
		return true;
	}
	return trySetValue(tracingStr, tracing, std::wregex(LR"(NO ?\/ ?НЕТ|N|E|электрообогрев)"), assertionCheck, "Недопустимое значение для спутникового обогрева!");
}

bool DrawingListASP::trySetPipelineClass(const std::wstring& pipelineClassStr, bool assertionCheck)
{
	return trySetValue(pipelineClassStr, pipelineClass, std::wregex(LR"([A-Z0-9]{9})"), assertionCheck, "Недопустимое значение для класса трубопровода!");
}

bool DrawingListASP::trySetTechnologicalEnvironment(const std::wstring& technologicalEnvironmentStr, bool assertionCheck)
{
	return trySetValue(technologicalEnvironmentStr, technologicalEnvironment, std::wregex(LR"([A-Z0-9]{2,4})"), assertionCheck, "Недопустимое значение для технологической среды!");
}

bool DrawingListASP::trySetTestEnvironment(const std::wstring& testEnvironmentStr, bool assertionCheck)
{
	return trySetValue(testEnvironmentStr, testEnvironment, std::wregex(LR"(WATER/ВОДА|вода/water)"), assertionCheck, "Недопустимое значение для среды испытаний!");
}

bool DrawingListASP::trySetPaintingSystem(const std::wstring& paintingSystemStr, bool assertionCheck)
{
	return trySetValue(paintingSystemStr, paintingSystem, std::wregex(LR"(\w\d->?\w\d|NO \/ НЕТ|Система \w, прим\. п\.1|В1-В2)"), assertionCheck,
		"Недопустимое значение для системы покраски!");
}

bool DrawingListASP::trySetPostWeldingHeatTreatment(const std::wstring& postWeldingHeatTreatmentStr, bool assertionCheck)
{
	return trySetValue(postWeldingHeatTreatmentStr, postWeldingHeatTreatment, std::wregex(LR"(NO ?\/ ?НЕТ)"), assertionCheck,
		"Недопустимое значение для послесварочной термообработки!");
}

bool DrawingListASP::trySetWeldInspection(const std::wstring& weldInspectionStr, bool assertionCheck)
{
	return trySetValue(weldInspectionStr, weldInspection, std::wregex(LR"(\d(,\d)?%|операционный|пооперационно)"), assertionCheck,
		"Недопустимое значение для контроля сварных швов!");
}

bool DrawingListASP::trySetTestPressure(const std::wstring& testPressureStr, bool assertionCheck)
{
	return trySetValue(testPressureStr, testPressure, std::wregex(LR"(\d\.\d{1,3}|атм\.)"), assertionCheck, "Недопустимое значение для давления испытаний!");
}

bool DrawingListASP::trySetGOSTPipelineCategory(const std::wstring& GOSTPipelineCategoryStr, bool assertionCheck)
{
	return trySetValue(GOSTPipelineCategoryStr, GOSTPipelineCategory, std::wregex(LR"([A-Z]{2,3}|NO \/ НЕТ)"), assertionCheck, "Недопустимое значение для категории трубопровода Гост!");
}

bool DrawingListASP::trySetDesignTemperature(const std::wstring& designTemperatureStr, bool assertionCheck)
{
	return trySetValue(designTemperatureStr, designTemperature, std::wregex(LR"(-?\d{1,3}(\/\d{1,3})?)"), assertionCheck, "Недопустимое значение для рассчётной температуры!");
}

bool DrawingListASP::trySetDesignPressure(const std::wstring& designPressureStr, bool assertionCheck)
{
	if (designPressureStr == L"окр.среды") {
		designPressure = L"env./окр.ср.";
		return true;
	}
	return trySetValue(designPressureStr, designPressure, std::wregex(LR"(\d(\.\d{1,3})?)"), assertionCheck, "Недопустимое значение для рассчётного давлени!");
}

bool DrawingListASP::trySetCipherDocument(const std::wstring& cipherDocumentStr, bool assertionCheck)
{
	return trySetValue(cipherDocumentStr, cipherDocument, std::wregex(LR"(GCC-ASP-DDD-\d+-\d+-\d+-\w+-ISO-\d+)"), assertionCheck, "Недопустимое значение для шифра документа!");
}

bool DrawingListASP::trySetDiameterPipeline(const std::wstring& diameterPipelineStr, bool assertionCheck)
{
	return trySetValue(diameterPipelineStr, diameterPipeline, std::wregex(LR"(DN\d{2,4})"), assertionCheck, "Недопустимое значение для диаметра трубопровода!");
}

bool DrawingListASP::trySetIsolation(const std::wstring& isolationStr, bool assertionCheck)
{
	return trySetValue(isolationStr, isolation, std::wregex(LR"(H|N|NO \/ НЕТ|YES \/ ДА)"), assertionCheck, "Недопустимое значение для изоляции!");
}

bool DrawingListASP::trySetCategoryPipelinesTRCU(const std::wstring& categoryPipelinesTRCUStr, bool assertionCheck)
{
	if (categoryPipelinesTRCUStr == L"-/-") {
		return true;
	}
	return trySetValue(categoryPipelinesTRCUStr, categoryPipelinesTRCU, std::wregex(LR"(NO \/ НЕТ|гр\. ?2, ?кат\. ?1)"), assertionCheck, 
		"Недопустимое значение категории трубопровода ТР ТС!");
}

bool DrawingListASP::trySetSchemeNumber(const std::wstring& schemeNumberStr, bool assertionCheck)
{
	return trySetValue(schemeNumberStr, schemeNumber, std::wregex(LR"((\/\d{3}-)?GCC-ASP-DDD-\d{5}-\d{2}-\d{4}-\w{2}-\w{3}-\d{5})"), assertionCheck,
		"Недопустимое значение для номера схемы!");
}

bool DrawingListASP::trySetLineNumber(const std::wstring& lineNumberStr, bool assertionCheck)
{
	return trySetValue(lineNumberStr, lineNumber, std::wregex(LR"(\d{3}-\w{2,4}-\d{4}-[A-Z0-9]{9}(_new)?)"), assertionCheck, "Недопустимое значение для номера линии!");
}

bool DrawingListASP::trySetStressCalculation(const std::wstring& stressCalculationStr, bool assertionCheck)
{
	return trySetValue(stressCalculationStr, stressCalculation, std::wregex(LR"(NO ?\/ ?НЕТ|YES ?\/ ?ДА)"), assertionCheck, "Недопустимое значение для расчёта напряжений!");
}

bool DrawingListASP::trySetIsometricDrawing(const std::wstring& isometricDrawingStr, bool assertionCheck)
{
	return trySetValue(isometricDrawingStr, isometricDrawing, std::wregex(LR"(\d{3}-\w{2,4}-\d{4}([ /0-9-]{2,5})?)"), assertionCheck,
		"Недопустимое значение для изометрического чертежа!");
}

bool DrawingListASP::trySetFileName(const std::wstring& fileNameStr, bool assertionCheck)
{
	return trySetValue(fileNameStr, fileName,
		std::wregex(LR"(\d{5}-\d{2}-\d{4}_\d{3}-\w{2}-\d{4}-[A-Z0-9]{9}-\d{2}_Sht_\d{1,2}.dwg|GCC-ASP-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5}[-_0-9A-Za-z]+\.dwg)"), assertionCheck,
		"Недопустимое значение для имени файла!");
}
