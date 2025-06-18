#include "DrawingPagePTE.h"

DrawingPagePTE::DrawingPagePTE(bool inputCheckOff) : DrawingPage(inputCheckOff)
{
	designPressurePattern.assign(LR"(-?\d{1,3}(\.\d{1,4})?(?:\/\d{1,3})?)");
	designTemperaturePattern.assign(LR"(-?\d{1,3}(?:\.\d{1,3})?\.{0,3}(?: ?\/? ?\d{1,3}(?:\.\d{1,3})?)?|гидроста?т?\.|атм\.)");
	operatingPressurePattern.assign(LR"(-?\d{1,4}(?:\.\d{1,4})?(?:\.{3}\d{1,4}(?:\.\d{1,4})?)?|гидроста?т?\.)");
	operatingTemperaturePattern.assign(LR"(-?\d{1,4}(?:\.\d{1,4})?(?:\.{3}\d{1,4}(?:\.\d{1,4})?)?)");
	testPressurePattern.assign(LR"(\d\.\d{1,3})");
	testEnvironmentPattern.assign(LR"(Air\/Воздух|Water/Вода)");
	weldInspectionPattern.assign(LR"(\d{1,3}%(?: [А-Я]{3})?(?: ?\/ ?)?(?:\d{1,2}(?:\,\d{1,2})?%)? ?[А-Я]{0,3})");
	postWeldingHeatTreatmentPattern.assign(LR"(NO \/ НЕТ)");
	stressCalculationPattern.assign(LR"(NO \/ НЕТ|YES \/ ДА)");
	paintingSystemPattern.assign(LR"(\w [A-Z\(\) 0-9]{10})");
	tracingPattern.assign(LR"([NT])");
	isolationPattern.assign(LR"([HN])");
	technologicalEnvironmentPattern.assign(LR"([A-Z0-9]{2,4})");
	categoryPipelinesTRCUPattern.assign(LR"(\d)");
	GOSTPipelineCategoryPattern.assign(LR"([A-ZА-Яа-яA-Z\/ ]{1,6})");
	diameterPipelinePattern.assign(LR"(\d{1,4})");
	pipelineClassPattern.assign(LR"([A-Z0-9]{9})");
	cipherDocumentPattern.assign(LR"(GCC-PTE-DDD-\d+-\d+-\d+-\w+-ISO-\d+)");
	schemeNumberPattern.assign(LR"(GCC-PTE-DDD-\d{5}-\d{2}-\d{4}-\w{2}-PID-\d{5}(?:;? ?-? ?\d{5};?| л\. \d{1,2}(?:-\d{1,2})?){0,})");
	lineNumberPattern.assign(LR"(\d{3}-[A-Z0-9]+-\d{4}-[A-Z0-9]{9}-\d{2}(?:-\d{2})?)");
	isometricDrawingPattern.assign(lineNumberPattern);
	fileNamePattern.assign(LR"(GCC-PTE-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5,8}(?:-\d{2})?(?:  ?\(\d+\))?\.dwg)");
}
