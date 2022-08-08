#include "logger.h"

namespace Console
{
    Logger *Logger::m_instance = NULL;

    Logger::Logger()
    {
        m_File.open(m_Filename, std::ios::out);

        if (!m_File)
        {
            throw std::runtime_error("Logger \"" + m_Filename + "\" failed to create output file.\n");
        }
    }

    void Logger::set_name(std::string name)
    {
        m_Filename = name;
    }

    Logger *Logger::get_logger()
    {
        if (!m_instance)
            m_instance = new Logger();
        return m_instance;
    }

    void Logger::info(std::string str)
    {
        std::string line = "[Info]: " + str;
        print(line, WHITE);
        write(line);
    }

    void Logger::success(std::string str)
    {
        std::string line = "[Success]: " + str;
        print(line, GREEN);
        write(line);
    }

    void Logger::warning(std::string str)
    {
        std::string line = "[Warning]: " + str;
        print(line, YELLOW);
        write(line);
    }

    void Logger::error(std::string str)
    {
        std::string line = "[Error]: " + str;
        print(line, RED);
        write(line);
    }

    Logger::~Logger()
    {
        m_File.close();
    }

    void Logger::print(std::string &str, Color color = WHITE)
    {
        std::cout << "\033[1;" + std::to_string(color) + "m" + str + "\033[0m\n";
    }

    void Logger::write(std::string str)
    {
        m_File << str << "\n";
    }

    std::string Logger::set_color(Color color)
    {
        return "\033[1;" + std::to_string(color) + "m";
    }
}
