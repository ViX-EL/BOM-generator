#include "DWGDotNetTextLoader.h"
#include "StringConvert.h"
#include <string>
#include <windows.h>
#include <iostream>
#include <wx/log.h> 

DWGDotNetTextLoader::DWGDotNetTextLoader(IMessagePrinter* errorPrinter) : TextLoader(errorPrinter)
{

}

int DWGDotNetTextLoader::loadFile(const std::string& filePath)
{
	text.clear();
	setFilePath(filePath);
	setFileName(getFilePath());

	setlocale(LC_ALL, "ru_RU.UTF-8");

	wxLogMessage(L"[Чтение] Начало чтения файла %s", getFileName());

	std::wstring commandLine = L"\"ConsoleDwgTextReader.exe\" \"" + utf8_decode(filePath) + L"\" \"" + separator + L"\"";

	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	HANDLE hReadPipe, hWritePipe;
	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
		throw std::runtime_error("CreatePipe failed!");
	}

	// Настраиваем процесс
	STARTUPINFOW si = { sizeof(STARTUPINFOW) };
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdOutput = hWritePipe;
	si.hStdError = hWritePipe;

	PROCESS_INFORMATION pi;
	if (!CreateProcessW(
		NULL,                          // Имя модуля (можно NULL, если передаётся commandLine)
		commandLine.data(),            // Командная строка (модифицируемый буфер)
		NULL,                          // Атрибуты процесса
		NULL,                          // Атрибуты потока
		TRUE,                          // Наследование дескрипторов
		CREATE_NO_WINDOW,              // Флаги создания (скрываем консоль)
		NULL,                          // Окружение (наследуем родительское)
		NULL,                          // Текущий каталог (наследуем)
		&si,                           // STARTUPINFO
		&pi                            // PROCESS_INFORMATION
	)) {
		CloseHandle(hReadPipe);
		CloseHandle(hWritePipe);
		throw std::runtime_error("CreateProcess failed!");
	}

	// Закрываем ненужный дескриптор записи
	CloseHandle(hWritePipe);

	// Читаем вывод
	const DWORD BUFFER_SIZE = 4096;
	std::string utf8Output;
	char buffer[BUFFER_SIZE];
	DWORD bytesRead;

	while (ReadFile(hReadPipe, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead != 0) {
		utf8Output.append(buffer, bytesRead);
	}

	if (utf8Output.empty())
	{
		printer->printError(L"Текст не найден в файле " + utf8_decode(filePath) + L'!');
		throw std::exception("Text not found!");
	}

	text = utf8_decode(utf8Output);

	// Закрываем дескрипторы
	CloseHandle(hReadPipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	wxLogMessage(L"[Чтение] Конец чтения файла %s", getFileName());

	return 0;
}
