#include <iostream>
#include <windows.h>
#include <string>

void SetFileTimes(const std::wstring& filePath, const SYSTEMTIME& newTime) {
    HANDLE hFile = CreateFileW(filePath.c_str(), FILE_WRITE_ATTRIBUTES, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to open file: " << filePath << std::endl;
        return;
    }

    FILETIME fileTime;
    if (SystemTimeToFileTime(&newTime, &fileTime)) {
        if (!SetFileTime(hFile, &fileTime, &fileTime, &fileTime)) {
            std::wcerr << L"Failed to set file times: " << filePath << std::endl;
        }
    }
    else {
        std::wcerr << L"Failed to convert system time to file time: " << filePath << std::endl;
    }

    CloseHandle(hFile);
}

void ProcessDirectory(const std::wstring& folderPath, const SYSTEMTIME& newTime) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((folderPath + L"\\*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to open directory: " << folderPath << std::endl;
        return;
    }

    do {
        const std::wstring fileName = findFileData.cFileName;
        if (fileName != L"." && fileName != L"..") {
            const std::wstring fullPath = folderPath + L"\\" + fileName;
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                // subdirectories if needed
            }
            else {
                SetFileTimes(fullPath, newTime);
                std::wcout << L"Updated times for: " << fullPath << std::endl;
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

int main() {
    std::wstring folderPath = L"C:\\Uni\\6\\eng";  // folder path
    SYSTEMTIME newTime;
    GetSystemTime(&newTime);  // time set to current

    ProcessDirectory(folderPath, newTime);

    return 0;
}
