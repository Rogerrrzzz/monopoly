#pragma once

#include "base_property.hpp"
#include "i_rentable.hpp"

namespace monopoly
{

// Участок с возможностью строительства.
class Property : public BaseProperty, public IRentable
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
    ~Property() noexcept override;

    [[nodiscard]] int GetBuildLevel() const;
    [[nodiscard]] int GetBuildCost() const;
    [[nodiscard]] int GetBuildRefund() const;
    [[nodiscard]] int CalcRent() const override;
    [[nodiscard]] std::string Describe() const override;
    [[nodiscard]] std::unique_ptr<BaseProperty> ShallowCopy() const override;
    [[nodiscard]] std::unique_ptr<BaseProperty> Clone() const override;

    void Build();
    void SellBuilding();
    void Mortgage() override;
    void ReturnToBank() override;
};

} // namespace monopoly
