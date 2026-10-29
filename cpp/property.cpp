#include "property.hpp"

#include <array>
#include <iostream>
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

    Property::~Property()
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
        return !IsOwned() || IsMortgaged() ? 0 : GetBaseRent() * RENT_MULTIPLIERS[m_buildLevel];
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

    bool Property::Build()
    {
        if (IsMortgaged() || m_buildLevel >= MAX_BUILD_LEVEL)
        {
            std::cout << "[Отказ] Участок заложен или достигнут предел застройки: " << GetName() << std::endl;
            return false;
        }
        ++m_buildLevel;
        return true;
    }

    bool Property::SellBuilding()
    {
        if (m_buildLevel == 0)
        {
            std::cout << "[Отказ] Нет построек: " << GetName() << std::endl;
            return false;
        }
        --m_buildLevel;
        return true;
    }

    bool Property::Mortgage()
    {
        if (m_buildLevel > 0)
        {
            std::cout << "[Отказ] Нельзя заложить участок с постройкой: " << GetName() << std::endl;
            return false;
        }
        return BaseProperty::Mortgage();
    }

    void Property::ReturnToBank()
    {
        BaseProperty::ReturnToBank();
        m_buildLevel = 0;
    }

} // namespace monopoly
