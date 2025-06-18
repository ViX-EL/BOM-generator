#include "DrawingPageAFT.h"
#include "DrawingPageASP.h"

DrawingPageAFT::DrawingPageAFT(bool inputCheckOff) : DrawingPageASP(inputCheckOff)
{
	pipelineClassPattern.assign(LR"([A-Z0-9]{9,11})");
	lineNumberPattern.assign(LR"(\d{3}-\w{1,4}-\d{4}(?:-\d{2}-)?(?:\d{3}-)?(?:[A-Z0-9]{9})?(?:_\d{1,2}\.\d{1,2})?(?:-\w)?(?:_new)?)");
	cipherDocumentPattern.assign(LR"(GCC-AFT-DDD-\d+-\d+-\d+-\w+-ISO-\d+)");
	categoryPipelinesTRCUPattern.assign(LR"(NO \/ НЕТ|gr\.\/гр\. ?2, ?cat\.\/кат\.? ?1?-?)");
	isolationPattern.assign(LR"(H|N|P|NO ?\/ ?НЕТ|YES ?\/ ?ДА)");
	paintingSystemPattern.assign(LR"(\w\d->?\w\d|NO ?\/ ?НЕТ|Система \w, прим\. п\.1|В1-В2)");
	fileNamePattern.assign(LR"(\d{5}-\d{2}-\d{4}_\d{3}-\w{2,4}-\d{4}-[A-Z0-9]{9}-\d{2}_Sht__?\d{1,2}.dwg|GCC-AFT-DDD-\d{5}-\d{2}-\d{4}-\w{2,4}-ISO-\d{5}[-_0-9A-Za-z]+\.dwg)");
	postWeldingHeatTreatmentPattern.assign(LR"((?:NO|YES) ?\/ ?(?:НЕТ|ДА))");
	operatingTemperaturePattern.assign(LR"(-?\d+(?:(?:\.\d+)?\/\d+| ?- ?\d{1,3})?|env\.\/окр\.ср\.)");
}
