#include "MultiTextField.h"

namespace Xcurse
{
    MultiTextField::MultiTextField() {}

    MultiTextField::MultiTextField(const std::vector<std::string> &lines, int weight, Alignment a)
    {
        m_weight = weight;
        alignment = a;
        set_data(lines);
    }

    MultiTextField::MultiTextField(const std::vector<std::wstring> &lines, int weight, Alignment a)
    {
        m_weight = weight;
        alignment = a;
        set_data(lines);
    }

    inline void MultiTextField::find_text_size()
    {
        m_text_size.width = std::accumulate(m_lines.begin(), m_lines.end(), 0, [](int i, std::wstring &s)
                                            { return std::max(i, static_cast<int>(s.size())); });
        m_text_size.height = m_lines.size();
    }

    void MultiTextField::set_data(const std::vector<std::string> &lines)
    {
        for (auto &l : lines)
        {
            m_lines.emplace_back(to_wstring(l));
        }
        find_text_size();
    }

    void MultiTextField::set_data(const std::vector<std::wstring> &lines)
    {
        m_lines = lines;
        find_text_size();
    }

    void MultiTextField::draw()
    {
        const int margin_lr = m_size.width - m_text_size.width;
        const int margin_l = margin_lr / 2;
        const int margin_r = margin_lr - margin_l;

        const int margin_tb = m_size.height - m_text_size.height;
        const int margin_t = margin_tb / 2;
        const int margin_b = margin_tb - margin_t;

        // TODO: add styling to this section
        switch (alignment)
        {
        case ALIGN_LEFT:
            for (int i = 0; i < m_lines.size(); i++)
            {
                m_display_ptr->set_pixels(this, 0, margin_t + i, m_lines[i]);
            }
            break;

        case ALIGN_CENTER:
            for (int i = 0; i < m_lines.size(); i++)
            {
                m_display_ptr->set_pixels(this, margin_l, margin_t + i, m_lines[i]);
            }
            break;

        case ALIGN_RIGHT:
            for (int i = 0; i < m_lines.size(); i++)
            {
                m_display_ptr->set_pixels(this, margin_lr, margin_t + i, m_lines[i]);
            }
            break;

        default:
            break;
        }
        return;
    }
}