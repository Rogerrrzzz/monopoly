#include "property_info.hpp"

namespace monopoly
{

    PropertyInfo::PropertyInfo(std::string_view name, int price, int baseRent)
        : m_name{ name }
        , m_price{ price }
        , m_baseRent{ baseRent }
    {
    }

    std::string_view PropertyInfo::GetName() const
    {
        return m_name;
    }

    int PropertyInfo::GetPrice() const
    {
        return m_price;
    }

    int PropertyInfo::GetBaseRent() const
    {
        return m_baseRent;
    }

    void PropertyInfo::SetName(std::string_view name)
    {
        m_name = name;
    }

    void PropertyInfo::SetBaseRent(int baseRent)
    {
        m_baseRent = baseRent;
    }

} // namespace monopoly
