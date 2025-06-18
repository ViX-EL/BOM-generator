#include "DrawingPageIOT.h"

DrawingPageIOT::DrawingPageIOT(bool inputCheckOff) : DrawingPage(inputCheckOff)
{
	cipherDocumentPattern.assign(LR"(GCC-IOT-DDD-\d+-\d+-\d+-\w+-ISO-\d+)");
	lineNumberPattern.assign(LR"(\d{2,3}-\w{1,4}(\d{1,2})?-\d{4}(?:-Generic)?)");
	isometricDrawingPattern.assign(LR"((?: +)?\d{2,3}-\w{1,4}(?:\d{1,2})?-\d{4}(?: part \d)?(?:-Generic)?)");
	fileNamePattern.assign(LR"(GCC-IOT-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5}[\-_0-9A-Za-z]+\.(?:pdf|dxf|dwg))");
	diameterPipelinePattern.assign(LR"(DN \d{2,4})");
	operatingTemperaturePattern.assign(LR"(-?\d{1,3}(?:\.\d)?(?:…\d{1,3})?|AMB\/ОКР\.СР\.)");
	operatingPressurePattern.assign(LR"(\d\.?(?:\d{1,4})??(?:\w{2}-\d{2})?(?:…\d(\.\d{1,2})?)?|ATM\/атм\.)");
	tracingPattern.assign(LR"(E|NO\/НЕТ)");
	pipelineClassPattern.assign(LR"([A-Z0-9]{9}(?:_KOS)?|(?:Generic)?(?:_spec)?)");
	technologicalEnvironmentPattern.assign(LR"([A-Z]{1,4}(?:\d{1,2})?)");
	testEnvironmentPattern.assign(LR"(AIR\/ВОЗДУХ|WATER\/ВОДА|NO\/НЕТ)");
	paintingSystemPattern.assign(LR"([A-ZА-Я]\d( ?(?:→|->) ?[A-ZА-Я]\d{1,2})?(,[A-ZА-Я]\d ?(?:→|->) ?[A-ZА-Я]\d)?|N[Oo](?:\/НЕТ)?)");
	postWeldingHeatTreatmentPattern.assign(LR"(NO\/НЕТ)");
	weldInspectionPattern.assign(LR"(\d{1,3}%?(?:[,\.]\d{1,3}%?)?(?:,см.ТТ)?|NO\/НЕТ|YES\/ДА)");
	testPressurePattern.assign(LR"(\d(?:\.\d{1,4})?(?:EX-\d{2})?|NO\/НЕТ|HP\/гидростат\.)");
	GOSTPipelineCategoryPattern.assign(LR"(NO\/НЕТ|[A-ZА-Яa-zа-я]{2,4})");
	designTemperaturePattern.assign(LR"(-?\d{1,3}(?:\.\d)?(?:\/\d{1,3})?)");
	designPressurePattern.assign(LR"(\d(?:\.\d{1,3})?(?:\w{2}-\d{2})?|ATM\/атм\.)");
	isolationPattern.assign(LR"(M|P|H|NO\/НЕТ)");
	categoryPipelinesTRCUPattern.assign(LR"(NO\/НЕТ|\d)");
	schemeNumberPattern.assign(LR"(GCC-[TI]OT-DDD-\d{5}-\d{2}-\d{4}-[A-ZА-Я]{2,4}\d?-\w{3}-\d{5}(-\d+)?|NO\/НЕТ)");
	stressCalculationPattern.assign(LR"(YES\/ДА|NO\/НЕТ)");
}