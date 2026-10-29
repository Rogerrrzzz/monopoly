#pragma once

#include "base_property.hpp"

namespace monopoly
{

    // Вокзал нельзя застраивать. Продолжение иерархии здесь не требуется.
    class RailwayStation final : public BaseProperty
    {
    public:
        RailwayStation();
        RailwayStation(std::string_view name, int price, int baseRent);
        RailwayStation(const RailwayStation& other);
        RailwayStation(RailwayStation&& other) noexcept;
        RailwayStation& operator=(const RailwayStation& other);
        RailwayStation& operator=(RailwayStation&& other) noexcept;
        ~RailwayStation() override;

        [[nodiscard]] int CalcRent() const override;
        [[nodiscard]] std::string Describe() const override;
        [[nodiscard]] std::unique_ptr<BaseProperty> ShallowCopy() const override;
        [[nodiscard]] std::unique_ptr<BaseProperty> Clone() const override;
    };

} // namespace monopoly
