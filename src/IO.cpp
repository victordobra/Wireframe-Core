#include "IO.hpp"
#include <stdio.h>

namespace wfe {
    namespace console {
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

            if(logFile.IsOpen())
                logFile.Write(str);
        }
        static void OutputMessage(const Message& message) {
            string output = "";

            if(messages.size())
                if(messages.back().type != message.type)
                    output.append(1, '\n');
            
            messages.push_back(message);

            switch(message.type) {
            case MessageType::MESSAGE:
                output += "[MESSAGE";
                break;
            case MessageType::WARNING:
                output += "[WARNING";
                break;
            case MessageType::ERROR:
                output += "[ERROR";
                break;
            case MessageType::FATAL_ERROR:
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
            Message msg = { MessageType::MESSAGE, message, line, sourceFile };
            OutputMessage(msg);
        }
        void OutWarningFunction   (const string& warning,                    size_t line, const string& sourceFile) {
            Message msg = { MessageType::WARNING, warning, line, sourceFile };
            OutputMessage(msg);
        }
        void OutErrorFunction     (const string& error  ,                    size_t line, const string& sourceFile) {
            Message msg = { MessageType::ERROR, error, line, sourceFile };
            OutputMessage(msg);
        }
        void OutFatalErrorFunction(const string& error  , size_t returnCode, size_t line, const string& sourceFile) {
            Message msg = { MessageType::FATAL_ERROR, error, line, sourceFile };
            OutputMessage(msg);
            exit(returnCode);
        }

        // Other functions
        vector<Message> GetMessages() {
            return messages;
        }
    }
}