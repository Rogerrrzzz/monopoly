#pragma once

#include "base_property.hpp"
#include "i_rentable.hpp"

namespace monopoly
{

class RailwayStation final : public BaseProperty, public IRentable
{
public:
    RailwayStation();
    RailwayStation(std::string_view name, int price, int baseRent);
    RailwayStation(const RailwayStation& other);
    RailwayStation(RailwayStation&& other) noexcept;
    RailwayStation& operator=(const RailwayStation& other);
    RailwayStation& operator=(RailwayStation&& other) noexcept;
    ~RailwayStation() noexcept override;

    [[nodiscard]] int CalcRent() const override;
    [[nodiscard]] std::string Describe() const override;
    [[nodiscard]] std::unique_ptr<BaseProperty> ShallowCopy() const override;
    [[nodiscard]] std::unique_ptr<BaseProperty> Clone() const override;
};

} // namespace monopoly
