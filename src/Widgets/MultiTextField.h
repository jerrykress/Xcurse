#pragma once

#include "GenericTextObject.h"

namespace Xcurse
{
    /**
     * @brief Text field but containing multiple lines and flexible size
     *
     */
    class MultiTextField : public GenericTextObject
    {
    public:
        MultiTextField();
        explicit MultiTextField(const std::vector<std::string> &lines, int weight, Alignment a = ALIGN_CENTER);
        explicit MultiTextField(const std::vector<std::wstring> &lines, int weight, Alignment a = ALIGN_CENTER);

        void set_data(const std::vector<std::string> &lines);
        void set_data(const std::vector<std::wstring> &w_lines);

    protected:
        void draw() override;
        inline void find_text_size();
        Size m_text_size = {1, 1};
        std::vector<std::wstring> m_lines;
    };
}