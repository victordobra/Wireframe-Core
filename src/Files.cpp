#include "Files.hpp"
#include <stdio.h>

namespace wfe {
    //----------------------FileInput----------------------

    //Constructors
    FileInput::FileInput() : file(0) { }
    FileInput::FileInput(const string& fileLocation, StreamType type) : file(0) {
        Open(fileLocation, type);
    }

    //Operators
    bool8_t FileInput::operator!() {
        return !file;
    }
    FileInput::operator bool8_t() {
        return file;
    }

    //Open/Close file
    FileInput& FileInput::Open(const string& fileLocation, StreamType type) {
        // If the file is already open, close it
        if(file)
            Close();

        // Chose the creation tags
        string tags = "r";
        if(type & STREAM_TYPE_BINARY)
            tags.append(1, 'b');
        
        // Open the file
        file = fopen(fileLocation.c_str(), tags.c_str());

        //If the file wasn't created, exit the function
        if(!file)
            return *this;
        
        // Move the file pointer to the end, if needed
        if(type & STREAM_TYPE_AT_THE_END)
            fseek(file, 0, SEEK_END);
        
        return *this;
    }
    FileInput& FileInput::Close() {
        if(!file)
            return *this;

        fclose(file);

        return *this;
    }

    // Get functions
    FileInput& FileInput::Get() {
        if(feof(file))
            return *this;

        fgetc(file);

        return *this;
    }
    FileInput& FileInput::ReadBuffer(char_t& c) {
        c = fgetc(file);

        return *this;
    }
    FileInput& FileInput::ReadBuffer(char_t* buffer, size_t count) {
        fread(buffer, sizeof(char_t), count, file);

        return *this;
    }

    FileInput& FileInput::ReadLine(string& str, size_t size, char_t sep) {
        char_t character; 
        size_t ind = 0;
        bool8_t exitError = false;
        
        do {
            int32_t charTemp = fgetc(file);
            if(charTemp == EOF)
                break;
            character = (char_t)charTemp;
            str.append(1, (char_t)character);
            ++ind;
        } while(character != sep && ind < size && file);

        return *this;
    }

    // Read functions
    FileInput& FileInput::Read(int8_t  & num, const char_t* sep) { 
        string str;
        Read(str, sep);
        num = (int8_t)stoll(str);
        
        return *this;
    }
    FileInput& FileInput::Read(int16_t & num, const char_t* sep) { 
        string str;
        Read(str, sep);
        num = (int16_t)stoll(str);

        return *this;
    }
    FileInput& FileInput::Read(int32_t & num, const char_t* sep) { 
        string str;
        Read(str, sep);
        num = (int32_t)stoll(str);

        return *this;
    }
    FileInput& FileInput::Read(int64_t & num, const char_t* sep) { 
        string str;
        Read(str, sep);
        num = stoll(str);

        return *this;
    }
    FileInput& FileInput::Read(uint8_t  & num, const char_t* sep) { 
        string str;
        Read(str, sep);
        num = (uint8_t)stoull(str);

        return *this;
    }
    FileInput& FileInput::Read(uint16_t & num, const char_t* sep) { 
        string str;
        Read(str, sep);
        num = (uint16_t)stoull(str);

        return *this;
    }
    FileInput& FileInput::Read(uint32_t & num, const char_t* sep) { 
        string str;
        Read(str, sep);
        num = (uint32_t)stoull(str);

        return *this;
    }
    FileInput& FileInput::Read(uint64_t & num, const char_t* sep) { 
        string str;
        Read(str, sep);
        num = stoull(str);

        return *this;
    }
    FileInput& FileInput::Read(float32_t& num, const char_t* sep) { 
        string str;
        Read(str, sep);
        num = stoull(str);

        return *this;
    }
    FileInput& FileInput::Read(float64_t& num, const char_t* sep) { 
        string str;
        Read(str, sep);
        num = stoull(str);

        return *this;
    }
    FileInput& FileInput::Read(string   & str, const char_t* sep) { 
        char_t c;

        do {
            int32_t charTemp = fgetc(file);
            if(charTemp < 0)
                break;
            c = (char_t)charTemp;
            str.append(1, c);
        } while(!strchr(sep, c) && file);

        return *this;
    }

    FileInput& FileInput::Seek(ptrdiff_t pos, SeekRelative relative) {
        fseek(file, (int32_t)pos, relative);
        return *this;
    }

    // Const functions
    bool8_t FileInput::IsOpen() const {
        return file;
    }
    bool8_t FileInput::IsAtTheEnd() const {
        return feof(file);
    }
    bool8_t FileInput::IsBad() const {
        return !file;
    }
    size_t FileInput::Tell() const {
        return (size_t)ftell(file);
    }
    size_t FileInput::GetCount() const {
        size_t pos = ftell(file);
        fseek(file, 0, SEEK_END);
        size_t count = ftell(file);
        fseek(file, pos, SEEK_SET);
        return count;
    }
    char_t FileInput::Peek() const {
        size_t pos = ftell(file);
        char_t result = fgetc(file);
        fseek(file, pos, SEEK_SET);
        return result;
    }

    // Destructor
    FileInput::~FileInput() {
        Close();
    }

    //----------------------FileOutput---------------------

    //Constructors
    FileOutput::FileOutput() : file(0) { }
    FileOutput::FileOutput(const string& fileLocation, StreamType type) : file(0) {
        Open(fileLocation, type);
    }

    // Operators
    bool8_t FileOutput::operator!() {
        return !file;
    }
    FileOutput::operator bool8_t() {
        return file;
    }

    // Open/Close file
    FileOutput& FileOutput::Open(const string& fileLocation, StreamType type) {
        // If the file is already open, close it
        if(file)
            Close();

        // Chose the creation tags
        string tags;
        if(type & STREAM_TYPE_APPEND)
            tags = "a";
        else
            tags = "w";
        
        if(type & STREAM_TYPE_BINARY)
            tags.append(1, 'b');
        
        // Open the file
        file = fopen(fileLocation.c_str(), tags.c_str());

        // If the file wasn't created, exit the function
        if(!file)
            return *this;
        
        // Move the file pointer to the end, if needed
        if(type & STREAM_TYPE_AT_THE_END)
            fseek(file, 0, SEEK_END);
        
        return *this;
    }
    FileOutput& FileOutput::Close() {
        if(!file)
            return *this;

        fclose(file);

        return *this;
    }

    // WriteBuffer functions
    FileOutput& FileOutput::WriteBuffer(char_t c) {
        fputc(c, file);

        return *this;
    }
    FileOutput& FileOutput::WriteBuffer(char_t* buf, size_t size) {
        fwrite(buf, sizeof(char_t), size, file);

        return *this;
    }

    // Write functions
    FileOutput& FileOutput::Write(char_t c) { 
        fputc(c, file);

        return *this;
    }
    FileOutput& FileOutput::Write(const string& str) { 
        fprintf(file, str.c_str());

        return *this;
    }

    // Const functions
    bool8_t FileOutput::IsOpen() const {
        return file;
    }
    bool8_t FileOutput::IsAtTheEnd() const {
        FILE* tempFile = file;
        int32_t result = fgetc(file);
        return result == EOF;
    }
    bool8_t FileOutput::IsBad() const {
        return !file;
    }
    size_t FileOutput::Tell() const {
        return (size_t)ftell(file);
    }
    size_t FileOutput::GetCount() const {
        FILE* tempFile = file;
        fseek(tempFile, 0, SEEK_END);
        return (size_t)ftell(tempFile);
    }
    char_t FileOutput::Peek() const {
        FILE* tempFile = file;
        return fgetc(tempFile);
    }

    // Destructor
    FileOutput::~FileOutput() {
        fclose(file);
    }
}