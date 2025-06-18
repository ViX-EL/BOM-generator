#include "DrawingPageHMS.h"

DrawingPageHMS::DrawingPageHMS(bool inputCheckOff) : DrawingPage(inputCheckOff)
{
	designPressurePattern.assign(LR"(-?\d{1,3}(?:[\,\.]\d{1,4})?|Атм \/ Atm)");
	designTemperaturePattern.assign(LR"([-+]?\d{1,3}(?:\.\d{1,3})?(?:\.{0,3}|…)?\+?\d{1,3}(?:\.\d{1,3})?)");
	operatingPressurePattern.assign(LR"(-?\d{1,3}(?:[\,\.]\d{1,4})?|Атм \/ Atm|Продувка \/ Purging)");
	operatingTemperaturePattern.assign(designTemperaturePattern);
	testPressurePattern.assign(designPressurePattern);
	testEnvironmentPattern.assign(LR"(Жидкость \/ Liquid)");
	weldInspectionPattern.assign(LR"(\d{1,3}%?)");
	postWeldingHeatTreatmentPattern.assign(LR"(NO \/ НЕТ|\d+)");
	stressCalculationPattern.assign(LR"(NO(?: \/ НЕТ)?|YES \/ ДА)");
	paintingSystemPattern.assign(LR"([A-Z])");
	tracingPattern.assign(LR"([TNR])");
	isolationPattern.assign(LR"([HRN])");
	technologicalEnvironmentPattern.assign(LR"([А-Яа-я \-0-9]+\/[A-Za-z \-0-9]+)");
	categoryPipelinesTRCUPattern.assign(LR"(\d)");
	GOSTPipelineCategoryPattern.assign(LR"([A-ZА-Яа-яA-Z\(\) |]{1,8})");
	diameterPipelinePattern.assign(LR"(DN \d{1,4})");
	pipelineClassPattern.assign(LR"([A-Z0-9]{9})");
	cipherDocumentPattern.assign(LR"(GCC-HMS-DDD-\d+-\d+-\d+-\w+-ISO-\d+)");
	schemeNumberPattern.assign(LR"(GCC-HMS-DDD-\d{5}-\d{2}-\d{4}-\w{2}-PID-\d{5})");
	lineNumberPattern.assign(LR"(\d{5}-\d{2}-\d{4}\/\d{3}-[A-Z0-9]+-\d{4}-[A-Z0-9]{9}-\d{2})");
	isometricDrawingPattern.assign(LR"(\d{3}-\w{2,4}-\d{4})");
	fileNamePattern.assign(LR"(GCC-HMS-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5,8}_Sht_\d{1,2}\.dwg)");
}
