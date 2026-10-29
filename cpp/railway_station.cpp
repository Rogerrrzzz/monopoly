#include "railway_station.hpp"

#include <iostream>
#include <utility>

namespace monopoly
{

    RailwayStation::RailwayStation()
        : BaseProperty{}
    {
        std::cout << "[RailwayStation] Конструктор по умолчанию" << std::endl;
    }

    RailwayStation::RailwayStation(std::string_view name, int price, int baseRent)
        : BaseProperty{ name, price, baseRent }
    {
        std::cout << "[RailwayStation] Конструктор с параметрами: " << GetName() << std::endl;
    }

    RailwayStation::RailwayStation(const RailwayStation& other)
        : BaseProperty{ other }
    {
        std::cout << "[RailwayStation] Конструктор копирования: " << GetName() << std::endl;
    }

    RailwayStation::RailwayStation(RailwayStation&& other) noexcept
        : BaseProperty{ std::move(other) }
    {
        std::cout << "[RailwayStation] Конструктор перемещения: " << GetName() << std::endl;
    }

    RailwayStation& RailwayStation::operator=(const RailwayStation& other)
    {
        if (this == &other)
        {
            return *this;
        }
        BaseProperty::operator=(other);
        std::cout << "[RailwayStation] Присваивание копированием" << std::endl;
        return *this;
    }

    RailwayStation& RailwayStation::operator=(RailwayStation&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        BaseProperty::operator=(std::move(other));
        std::cout << "[RailwayStation] Присваивание перемещением" << std::endl;
        return *this;
    }

    RailwayStation::~RailwayStation()
    {
        std::cout << "[RailwayStation] Деструктор: " << GetName() << std::endl;
    }

    int RailwayStation::CalcRent() const
    {
        return !IsOwned() || IsMortgaged() ? 0 : GetBaseRent();
    }

    std::string RailwayStation::Describe() const
    {
        // Полная замена Describe без вызова BaseProperty::Describe.
        return "Вокзал " + Header() + ", фиксированная аренда " + std::to_string(CalcRent());
    }

    std::unique_ptr<BaseProperty> RailwayStation::ShallowCopy() const
    {
        auto copy = std::make_unique<RailwayStation>(GetName(), GetPrice(), GetBaseRent());
        CopySharedStateTo(*copy);
        return copy;
    }

    std::unique_ptr<BaseProperty> RailwayStation::Clone() const
    {
        return std::make_unique<RailwayStation>(*this);
    }

} // namespace monopoly
