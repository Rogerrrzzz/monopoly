#include "rent_calculator.hpp"

#include <stdexcept>
#include <utility>

namespace monopoly
{

RentCalculator::RentCalculator(std::unique_ptr<IRentPolicy> policy)
{
    SetPolicy(std::move(policy));
}

void RentCalculator::SetPolicy(std::unique_ptr<IRentPolicy> policy)
{
    if (!policy)
    {
        throw std::invalid_argument{ "Правило расчёта аренды не задано" };
    }
    m_policy = std::move(policy);
}

int RentCalculator::Calculate(const IRentable& property) const
{
    return m_policy->Apply(property.CalcRent());
}

} // namespace monopoly
