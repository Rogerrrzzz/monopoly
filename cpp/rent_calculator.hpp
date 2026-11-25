#pragma once

#include "i_rent_policy.hpp"
#include "i_rentable.hpp"

#include <memory>

namespace monopoly
{

class RentCalculator
{
private:
    std::unique_ptr<IRentPolicy> m_policy{};

public:
    explicit RentCalculator(std::unique_ptr<IRentPolicy> policy);
    void SetPolicy(std::unique_ptr<IRentPolicy> policy);
    [[nodiscard]] int Calculate(const IRentable& property) const;
};

} // namespace monopoly
