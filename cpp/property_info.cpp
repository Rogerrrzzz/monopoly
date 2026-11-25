#include "property_info.hpp"

#include "game_rule_error.hpp"

namespace monopoly
{

PropertyInfo::PropertyInfo(std::string_view name, int price, int baseRent)
    : m_name{ name }
    , m_price{ price }
    , m_baseRent{ baseRent }
{
    if (price < 0 || baseRent < 0)
    {
        throw GameRuleError{ "Цена и базовая аренда не могут быть отрицательными" };
    }
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
    if (baseRent < 0)
    {
        throw GameRuleError{ "Базовая аренда не может быть отрицательной" };
    }
    m_baseRent = baseRent;
}

} // namespace monopoly
