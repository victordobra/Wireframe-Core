#pragma once

#include "Files.hpp"
#include "vector.hpp"

namespace mge {
    namespace console {
        enum class MessageType : uint8_t {
            MESSAGE,
            WARNING,
            ERROR,
            FATAL_ERROR
        };

        struct Message {
            MessageType type;
            string message;
            size_t line;
            string sourceFile;
        };

        void OpenLogFile(const string& path = "log.txt");
        void CloseLogFile();
        FileOutput& GetLogFileOutput();

        void OutMessageFunction   (const string& message,                    size_t line = -1, const string& sourceFile = "");
        void OutWarningFunction   (const string& warning,                    size_t line = -1, const string& sourceFile = "");
        void OutErrorFunction     (const string& error  ,                    size_t line = -1, const string& sourceFile = "");
        void OutFatalErrorFunction(const string& error  , size_t returnCode, size_t line = -1, const string& sourceFile = "");

        vector<Message> GetMessages();
        
#define OutMessage(message)              OutMessageFunction   (message,             __LINE__, __FILE__)
#define OutWarning(warning)              OutWarningFunction   (warning,             __LINE__, __FILE__)
#define OutError(error)                  OutErrorFunction     (error  ,             __LINE__, __FILE__)
#define OutFatalError(error, returnCode) OutFatalErrorFunction(error  , returnCode, __LINE__, __FILE__)
    }
}