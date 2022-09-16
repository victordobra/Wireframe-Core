#include "IO.hpp"
#include <stdio.h>

namespace wfe::console {
    FileOutput logFile;
    vector<Message> messages = {};

    // Log file functions
    void OpenLogFile(const string& path) {
        logFile.Open(path);
    }
    void CloseLogFile() {
        logFile.Close();
    }
    FileOutput& GetLogFileOutput() {
        return logFile;
    }

    // Internal helper functions
    static void Output(char_t c) {
        putchar(c);

        if(logFile.IsOpen())
            logFile.Write(c);
    }
    static void Output(const string& str) {
        puts(str.c_str());

        if(logFile.IsOpen()) {
            logFile.Write(str);
            logFile.Flush();
        }
    }
    static void OutputMessage(const Message& message) {
        string output = "";

        if(messages.size())
            if(messages.back().type != message.type)
                output.append(1, '\n');
        
        messages.push_back(message);

        switch(message.type) {
        case MESSAGE_TYPE_MESSAGE:
            output += "[MESSAGE";
            break;
        case MESSAGE_TYPE_WARNING:
            output += "[WARNING";
            break;
        case MESSAGE_TYPE_ERROR:
            output += "[ERROR";
            break;
        case MESSAGE_TYPE_FATAL_ERROR:
            output += "[FATAL ERROR";
            break;
        }

        if(message.line != -1)
            output += (string)" LINE: " + ToString(message.line);
        if(message.sourceFile.length())
            output += (string)" FILE: " + message.sourceFile;

        output += (string)"] " + message.message + "\n";
        Output(output);
    }

    // Output functions
    void OutMessageFunction   (const string& message,                    size_t line, const string& sourceFile) {
        Message msg = { MESSAGE_TYPE_MESSAGE, message, line, sourceFile };
        OutputMessage(msg);
    }
    void OutWarningFunction   (const string& warning,                    size_t line, const string& sourceFile) {
        Message msg = { MESSAGE_TYPE_WARNING, warning, line, sourceFile };
        OutputMessage(msg);
    }
    void OutErrorFunction     (const string& error  ,                    size_t line, const string& sourceFile) {
        Message msg = { MESSAGE_TYPE_ERROR, error, line, sourceFile };
        OutputMessage(msg);
    }
    void OutFatalErrorFunction(const string& error  , size_t returnCode, size_t line, const string& sourceFile) {
        Message msg = { MESSAGE_TYPE_FATAL_ERROR, error, line, sourceFile };
        OutputMessage(msg);
        exit(returnCode);
    }

    // Other functions
    vector<Message>& GetMessages() {
        return messages;
    }
}