#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

namespace Console
{
    /*
        Definitions of colors that can be used to print in consoles
    */
    enum Color
    {
        WHITE = 0,
        GREEN = 32,
        YELLOW = 33,
        RED = 31
    };

    /*
        A simple Logger that records several types of information and stores the output in a file
    */
    class Logger
    {
    public:
        static Logger *get_logger();

        void set_name(std::string name);

        void info(std::string str);

        void success(std::string str);

        void warning(std::string str);

        void error(std::string str);

        std::string set_color(Color color);

    protected:
        std::string m_Filename = "log.txt";
        Logger();
        Logger(Logger &l) = delete;
        Logger(Logger &&l) = delete;
        Logger &operator=(Logger &l) = delete;
        Logger &operator=(Logger &&l) = delete;
        ~Logger();

        std::fstream m_File;
        void print(std::string &str, Color color);
        void write(std::string str);

    private:
        static Logger *m_instance;
    };
}