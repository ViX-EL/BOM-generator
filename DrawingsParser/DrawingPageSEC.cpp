#include "DrawingPageSEC.h"

DrawingPageSEC::DrawingPageSEC(bool inputCheckOff) : DrawingPage(inputCheckOff)
{
	designPressurePattern.assign(LR"(-?\d{1,4}(?:\.\d{1,4})?\/?-?(?:\d{1,4}(?:\.\d{1,6})?|\w\.\w)?)");
	designTemperaturePattern.assign(LR"(-?\d{1,3}(?:\.\d{1,4})?(?: ?\/? ?-?\d{1,3}(?:\.\d{1,4})?)?)");
	operatingPressurePattern.assign(LR"(-?\d{1,3}(?:\.\d{1,4})|ATM)");
	operatingTemperaturePattern.assign(LR"(-?\.?\d{1,4}(?:\.\d{1,4})?|AMB)");
	testPressurePattern.assign(LR"(\d{1,4}(?:\.\d{1,4})?)");
	testEnvironmentPattern.assign(LR"(HYDR|PNEU)");
	weldInspectionPattern.assign(LR"(\.?\d{1,4}(?:\.\d{1,4})?|N\/A)");
	postWeldingHeatTreatmentPattern.assign(LR"(N\/A|Y)");
	stressCalculationPattern.assign(LR"(\d{3}\w{1,4}\d{4,6}|N\/A)");
	paintingSystemPattern.assign(LR"([A-Z0-9\-]{1,3}|N\/A)");
	tracingPattern.assign(LR"((?: ?[A-Z]\,?){1,3})");
	isolationPattern.assign(tracingPattern);
	technologicalEnvironmentPattern.assign(LR"([A-Z0-9]{1,5})");
	categoryPipelinesTRCUPattern.assign(LR"((?:\d\,? ?){1,2}(?:N\/A)?|[A-Za-zА-Яа-я\(\)]{1,4})");
	GOSTPipelineCategoryPattern.assign(LR"((?: ?[\(\)a-zа-яA-ZА-Я]{1,4}\,?){1,2})");
	diameterPipelinePattern.assign(testPressurePattern);
	pipelineClassPattern.assign(LR"([A-Z0-9]{11})");

	currentPagePattern.assign(LR"(\d{1,2}\w?)");
	isometricDrawingPattern.assign(LR"(\d{3}\-\w{1,4}(?:\d{1,2})?\-\d{3,4}\w?(?:-[A-Z0-9]{1,4}){0,4})");
	lineNumberPattern.assign(isometricDrawingPattern);
	schemeNumberPattern.assign(LR"((?:\d{3}\-)?\d{5})");
	cipherDocumentPattern.assign(LR"(GCC-SEC-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5})");
	fileNamePattern.assign(LR"((?:\w+)?GCC-SEC-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO[-_]\d{5}[\w\-~]+\.dwg)");
}