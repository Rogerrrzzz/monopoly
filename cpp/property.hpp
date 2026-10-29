#pragma once

#include "base_property.hpp"

namespace monopoly
{

    // Участок с возможностью строительства.
    class Property : public BaseProperty
    {
    public:
        static constexpr int MAX_BUILD_LEVEL = 4;

    private:
        int m_buildLevel{ 0 };

    public:
        Property();
        Property(std::string_view name, int price, int baseRent);
        Property(const Property& other);
        Property(Property&& other) noexcept;
        Property& operator=(const Property& other);
        Property& operator=(Property&& other) noexcept;
        ~Property() override;

        [[nodiscard]] int GetBuildLevel() const;
        [[nodiscard]] int GetBuildCost() const;
        [[nodiscard]] int GetBuildRefund() const;
        [[nodiscard]] int CalcRent() const override;
        [[nodiscard]] std::string Describe() const override;
        [[nodiscard]] std::unique_ptr<BaseProperty> ShallowCopy() const override;
        [[nodiscard]] std::unique_ptr<BaseProperty> Clone() const override;

        bool Build();
        bool SellBuilding();
        bool Mortgage() override;
        void ReturnToBank() override;
    };

} // namespace monopoly
