#include "DrawingPageIOT.h"

DrawingPageIOT::DrawingPageIOT() //TODO Ïåğåäåëàòü øàáëîíû
{
	operatingTemperaturePattern.assign(LR"((-|\+)?\d{2,3}(\.\.\.\+?\d{2,3})?|AMBIENT \/ ÎÊĞ\. ÑĞÅÄÀ|Íå íèæå \d \/ No lower than \d)");
	operatingPressurePattern.assign(LR"(-?\d,\d{1,6}(\.\.\.\d,\d{1,6})?|ATM \/ ÀÒÌ\.|HYDR\. \/ ÃÈÄĞÎÑÒ|NO MORE \/ ÍÅ ÁÎËÅÅ \d,\d{1,6}|HOLD)");
	tracingPattern.assign(LR"(E|H|N|T|NO \/ ÍÅÒ)");
	pipelineClassPattern.assign(LR"([A-Z0-9]{8,9})");
	technologicalEnvironmentPattern.assign(LR"([A-Z]{2,4})");
	testEnvironmentPattern.assign(LR"((PLANT )?AIR ?/ ?(ÒÅÕÍÈ×ÅÑÊÈÉ )?ÂÎÇÄÓÕ|FW / ÏĞÎÒÈÂÎÏÎÆÀĞÍÛÉ ÂÎÄÎÏĞÎÂÎÄ|NO / ÍÅÒ|WATER / ÂÎÄÀ)");
	paintingSystemPattern.assign(LR"(A|B|C|T|NO \/ ÍÅÒ)");
	postWeldingHeatTreatmentPattern.assign(LR"(NO \/ ÍÅÒ)");
	weldInspectionPattern.assign(LR"(\d{1,3}%?( ÏÂÊ)?|\d ìåòîäà îáúåìíîãî êîíòğîëÿ|NO \/ ÍÅÒ)");
	testPressurePattern.assign(LR"(\d,\d{1,3}|NO \/ ÍÅÒ)");
	GOSTPipelineCategoryPattern.assign(LR"(\w{1,3}(-\w{1,2})?((,|\.) \w{1,2})?( \/ \w{1,3} \(\w{3} \d{3}\))?)"); //[A-Za-zÀ-ßà-ÿ .,/()0-9-]{4,22}
	designTemperaturePattern.assign(LR"(-?\d{1,3} ?( ?\/ ?|\.\.\.?)?\+?(\d{1,3})?)");
	designPressurePattern.assign(LR"((FV|-?\d(,\d{1,9})?)?( ?\/ ?(\d(,|\.)\d{2}|FV)?)?|HYDR\. \/ ÃÈÄĞÎÑÒ\.)");
	cipherDocumentPattern.assign(LR"(GCC-NAG-DDD-\d+-\d+-\d+-\w+-ISO-\d+)");
	diameterPipelinePattern.assign(LR"(DN ?\d{2,4}(( \/ |,) ?(DN)? ?\d{2,4})?)");
	isolationPattern.assign(LR"(\d{2,3}( \d{2,3})?( \d{2,3})?( \d{2,3})?( \d{2,3})?|H|NO \/ ÍÅÒ)");
	categoryPipelinesTRCUPattern.assign(LR"(NO \/ ÍÅÒ|\d|[0-9A-Z() /À-ß]{10,20})");
	schemeNumberPattern.assign(LR"(GCC-NAG-DDD-\d{5}-\d{2}-\d{4}-\w{2}-\w{3}-\d{5}(, ?\d{5}){0,2})");
	lineNumberPattern.assign(LR"(\d{5}-\w{2}-\d{4}\/\d{3}-\w{2,4}-\d{4}-[0-9A-Z]{9}-\d{2})");
	stressCalculationPattern.assign(LR"(NO \/ ÍÅÒ|YES \/ ÄÀ)");
	isometricDrawingPattern.assign(LR"(\d{3}-\w{2,4}-\d{4}([ /0-9-]{2,5})?)");
	fileNamePattern.assign(LR"(GCC-ASP-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5}[-_0-9A-Za-z]+\.dwg)");
}

//bool DrawingPageIOT::tryAddComponent(const std::wstring& componentNumberStr)
//{
//	//if (std::regex_match(componentNumberStr, positionNumberPattern))
//	//{
//	//	components.emplace_back(std::make_shared<BuildComponentASP>(new BuildComponentASP(componentNumberStr)));
//	//	return true;
//	//}
//	return false;
//}
