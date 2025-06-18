#include "DrawingPageFGG.h"

DrawingPageFGG::DrawingPageFGG(bool inputCheckOff) : DrawingPage(inputCheckOff)
{
	operatingTemperaturePattern.assign(LR"(\-?\d{1,3}(?:\.\d{1,2})?\/\-?\d{1,3}(?:\.\d{1,2})?(?:\/\-?\d{1,3}(?:\.\d{1,2})?)?)");
	operatingPressurePattern.assign(LR"(\-?\d{1,2}(?:\.\d{1,3})?(?:\w{2}-\d{2})?\/\-?\d{1,3}(?:\.\d{1,3})?(?:\w{2}-\d{2})?\/\-?\d{1,2}(?:\.\d{1,3})?(?:\w{2}-\d{2})?)");
	tracingPattern.assign(LR"(N|E)");
	pipelineClassPattern.assign(LR"(\w{11})");
	technologicalEnvironmentPattern.assign(LR"(\w{2,3})");
	testEnvironmentPattern.assign(LR"([Aa]ir|[Ww]ater ?|N)");
	paintingSystemPattern.assign(LR"(N|\w\d→\w\d)");
	postWeldingHeatTreatmentPattern.assign(LR"(No)");
	weldInspectionPattern.assign(LR"(\d{1,3}%{1,3}(?:\w{2})? ?(?:[ +]LPT\/ ?MPI ?(?:\(incl\. \d%{3}RT\))?)?(?:\(min \d joints\) ?)?)");
	testPressurePattern.assign(LR"(\d(?:\.\d{1,2})?|N)");
	GOSTPipelineCategoryPattern.assign(LR"(\w{1,3} \/ [\wВ])");
	designTemperaturePattern.assign(LR"(-?\d{1,3}\/\d{1,3})");
	designPressurePattern.assign(LR"((?:\d\/)?\d(?:\.\d{1,2})?(?:\w{2}-\d{2})?(\/\d\.\d{1,2}\w{2}-\d{2})?(\/\d\.\d{1,2}\w{2}-\d{2})?)");
	cipherDocumentPattern.assign(LR"(GCC-FGG-DDD-\d+-\d+-\d+-\w+-ISO-\d+)");
	diameterPipelinePattern.assign(LR"(\d{2,4})");
	isolationPattern.assign(LR"([NCKHP])");
	categoryPipelinesTRCUPattern.assign(LR"(no|\d)");
	schemeNumberPattern.assign(LR"(\/?\d{5}-\w{3}-\d{5}-\d{2})");
	lineNumberPattern.assign(LR"(\d{3}-\w{2,3}-\d{4}-(?:\d{2,3}-)?\w{11})");
	stressCalculationPattern.assign(LR"(No|Yes)");
	isometricDrawingPattern.assign(LR"(\d{3}-\w{2}-\d{4}-\w{11})");
	fileNamePattern.assign(LR"(GCC-FGG-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5}(?:_\d{2})?\.dwg)");
}
