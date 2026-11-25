#include "property.hpp"

#include "game_rule_error.hpp"

#include <array>
#include <iostream>
#include <limits>
#include <utility>

namespace monopoly
{

namespace
{
constexpr std::array<int, Property::MAX_BUILD_LEVEL + 1> RENT_MULTIPLIERS{ 1, 2, 3, 5, 8 };
} // namespace

Property::Property()
    : BaseProperty{}
{
    std::cout << "[Property] Конструктор по умолчанию" << std::endl;
}

Property::Property(std::string_view name, int price, int baseRent)
    : BaseProperty{ name, price, baseRent }
{
    std::cout << "[Property] Конструктор с параметрами: " << GetName() << std::endl;
}

Property::Property(const Property& other)
    : BaseProperty{ other }
    , m_buildLevel{ other.m_buildLevel }
{
    std::cout << "[Property] Конструктор копирования: " << GetName() << std::endl;
}

Property::Property(Property&& other) noexcept
    : BaseProperty{ std::move(other) }
    , m_buildLevel{ other.m_buildLevel }
{
    other.m_buildLevel = 0;
    std::cout << "[Property] Конструктор перемещения: " << GetName() << std::endl;
}

Property& Property::operator=(const Property& other)
{
    if (this == &other)
    {
        std::cout << "[Property] Самоприсваивание: объект сохранён" << std::endl;
        return *this;
    }
    BaseProperty::operator=(other);
    m_buildLevel = other.m_buildLevel;
    std::cout << "[Property] Присваивание копированием" << std::endl;
    return *this;
}

Property& Property::operator=(Property&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }
    BaseProperty::operator=(std::move(other));
    m_buildLevel = other.m_buildLevel;
    other.m_buildLevel = 0;
    std::cout << "[Property] Присваивание перемещением" << std::endl;
    return *this;
}

Property::~Property() noexcept
{
    std::cout << "[Property] Деструктор: " << GetName() << std::endl;
}

int Property::GetBuildLevel() const
{
    return m_buildLevel;
}

int Property::GetBuildCost() const
{
    return GetPrice() / 2;
}

int Property::GetBuildRefund() const
{
    return GetBuildCost() / 2;
}

int Property::CalcRent() const
{
    if (!IsOwned() || IsMortgaged())
    {
        return 0;
    }
    const int multiplier = RENT_MULTIPLIERS[m_buildLevel];
    if (GetBaseRent() > std::numeric_limits<int>::max() / multiplier)
    {
        throw GameRuleError{ "Сумма аренды участка слишком велика" };
    }
    return GetBaseRent() * multiplier;
}

std::string Property::Describe() const
{
    // Дополняем реализацию базового класса.
    return "Участок " + BaseProperty::Describe() + ", уровень застройки " + std::to_string(m_buildLevel);
}

std::unique_ptr<BaseProperty> Property::ShallowCopy() const
{
    auto copy = std::make_unique<Property>(GetName(), GetPrice(), GetBaseRent());
    CopySharedStateTo(*copy);
    copy->m_buildLevel = m_buildLevel;
    return copy;
}

std::unique_ptr<BaseProperty> Property::Clone() const
{
    return std::make_unique<Property>(*this);
}

void Property::Build()
{
    if (IsMortgaged())
    {
        throw GameRuleError{ "Нельзя строить на заложенном участке" };
    }
    if (m_buildLevel >= MAX_BUILD_LEVEL)
    {
        throw GameRuleError{ "Достигнут предел застройки участка" };
    }
    ++m_buildLevel;
}

void Property::SellBuilding()
{
    if (m_buildLevel == 0)
    {
        throw GameRuleError{ "На участке нет построек для продажи" };
    }
    --m_buildLevel;
}

void Property::Mortgage()
{
    if (m_buildLevel > 0)
    {
        throw GameRuleError{ "Нельзя заложить участок с постройкой" };
    }
    BaseProperty::Mortgage();
}

void Property::ReturnToBank()
{
    BaseProperty::ReturnToBank();
    m_buildLevel = 0;
}

} // namespace monopoly
