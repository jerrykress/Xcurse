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
        A simple logger that records several types of information and stores the output in a file
    */
    class logger
    {
    public:
        logger(std::string fn);

        void info(std::string str);

        void success(std::string str);

        void warning(std::string str);

        void error(std::string str);
        /* set the color of current cout (does not auto restore) */
        std::string set_color(Color color);

        logger(logger &l) = delete;
        logger(logger &&l) = delete;
        logger &operator=(logger &l) = delete;
        logger &operator=(logger &&l) = delete;

        ~logger();

    protected:
        std::string m_Filename;
        std::fstream m_File;

        void print(std::string &str, Color color);

        void write(std::string str);
    };

    /*

    */
    class Shape
    {
    public:
        virtual void Draw() = 0;
    };
}