#include "IO.hpp"
#include <stdio.h>

namespace wfe {
    namespace console {
        FileOutput logFile;
        vector<Message> messages = {};

        //Log file functions
        void OpenLogFile(const string& path) {
            logFile.Open(path);
        }
        void CloseLogFile() {
            logFile.Close();
        }
        FileOutput& GetLogFileOutput() {
            return logFile;
        }

        //Internal helper functions
        static void Output(char_t c) {
            putchar(c);

            if (logFile.IsOpen())
                logFile.Write(c);
        }
        static void Output(const string& str) {
            puts(str.c_str());

            if (logFile.IsOpen())
                logFile.Write(str);
        }
        static void OutputMessage(const Message& message) {
            if(messages.size())
                if(messages.back().type != message.type)
                    Output('\n');
            
            messages.push_back(message);

            switch (message.type) {
            case MessageType::MESSAGE:
                Output("[MESSAGE");
                break;
            case MessageType::WARNING:
                Output("[WARNING");
                break;
            case MessageType::ERROR:
                Output("[ERROR");
                break;
            case MessageType::FATAL_ERROR:
                Output("[FATAL ERROR");
                break;
            default:
                break;
            }

            if (message.line != -1)
                Output((string)" LINE: " + ToString(message.line));
            if (message.sourceFile.length())
                Output((string)" FILE: " + message.sourceFile);

            Output("] ");
            Output(message.message);
            Output('\n');
        }

        //Output functions
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

        //Other functions
        vector<Message> GetMessages() {
            return messages;
        }
    }
}