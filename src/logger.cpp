#include "logger.h"

namespace Console
{
    logger::logger(std::string fn)
    {
        m_Filename = fn;
        m_File.open(m_Filename, std::ios::out);

        if (!m_File)
        {
            throw std::runtime_error("Logger \"" + m_Filename + "\" failed to create output file.\n");
        }
    }

    void logger::info(std::string str)
    {
        std::string line = "[Info]: " + str;
        print(line, WHITE);
        write(line);
    }

    void logger::success(std::string str)
    {
        std::string line = "[Success]: " + str;
        print(line, GREEN);
        write(line);
    }

    void logger::warning(std::string str)
    {
        std::string line = "[Warning]: " + str;
        print(line, YELLOW);
        write(line);
    }

    void logger::error(std::string str)
    {
        std::string line = "[Error]: " + str;
        print(line, RED);
        write(line);
    }

    logger::~logger()
    {
        m_File.close();
    }

    void logger::print(std::string &str, Color color = WHITE)
    {
        std::cout << "\033[1;" + std::to_string(color) + "m" + str + "\033[0m\n";
    }

    void logger::write(std::string str)
    {
        m_File << str << "\n";
    }

    std::string logger::set_color(Color color)
    {
        return "\033[1;" + std::to_string(color) + "m";
    }
}
