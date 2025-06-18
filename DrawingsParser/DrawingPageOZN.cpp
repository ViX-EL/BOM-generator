#include "DrawingPageOZN.h"

DrawingPageOZN::DrawingPageOZN(bool inputCheckOff) : DrawingPage(inputCheckOff)
{
	designPressurePattern.assign(LR"(-?\d{1,3}(?:[\,\.]\d{1,4})?)");
	designTemperaturePattern.assign(LR"([-+]?\d{1,3}(?:[\.,]\d{1,4})?\.{0,3}\+?(?:\d{1,3})?(?:[\.,]\d{1,4})?)");
	operatingPressurePattern.assign(designTemperaturePattern);
	operatingTemperaturePattern.assign(designTemperaturePattern);
	testPressurePattern.assign(designPressurePattern);
	testEnvironmentPattern.assign(LR"(water ?\/ ?вода|air ?/ ?воздух)");
	weldInspectionPattern.assign(LR"(\d{1,3}%*(?: [А-ЯA-Z\+\, ]{0,12} *\/ *\d{1,3}%* [А-ЯA-Z\+\, ]{0,12})?)");
	postWeldingHeatTreatmentPattern.assign(LR"(yes ?\/ ?да|no ?\/ ?нет)");
	stressCalculationPattern.assign(LR"(yes ?\/ ?да|no ?\/ ?нет)");
	paintingSystemPattern.assign(LR"([A-ZА-Я])");
	tracingPattern.assign(LR"([ET]|NO/НЕТ)");
	isolationPattern.assign(LR"([HP]|NO/НЕТ)");
	technologicalEnvironmentPattern.assign(LR"([A-Z0-9]+)");
	categoryPipelinesTRCUPattern.assign(LR"(\d{1,3}(?: ?\((?:\-|\d)\))?)");
	GOSTPipelineCategoryPattern.assign(LR"([А-Яа-яA-Za-z\(\) ]{1,8}(?:\/[А-Яа-яA-Za-z\(\) ]{1,8})?)");
	diameterPipelinePattern.assign(LR"(\d{1,4}mm)");
	pipelineClassPattern.assign(LR"([A-Z0-9]{9})");
	cipherDocumentPattern.assign(LR"(GCC-OZN-DDD-\d+-\d+-\d+-\w+-ISO-\d+)");
	schemeNumberPattern.assign(LR"(GCC-OZN-DDD-\d{5}-\d{2}-\d{4}-\w{2}-PID-\d{5})");
	lineNumberPattern.assign(LR"(\d{3}-\w{1,4}-\d{4})");
	isometricDrawingPattern.assign(lineNumberPattern);
	fileNamePattern.assign(LR"(GCC-OZN-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5}(?:-\d{3}|_\d{2}_\d_source)\.dwg)");
}
