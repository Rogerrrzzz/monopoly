#pragma once

#include "i_rent_policy.hpp"

namespace monopoly
{

class DoubleRentPolicy final : public IRentPolicy
{
public:
    [[nodiscard]] int Apply(int rent) const override;
};

} // namespace monopoly
