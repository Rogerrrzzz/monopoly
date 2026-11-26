#pragma once

#include "i_rent_policy.hpp"
#include "i_rentable.hpp"

#include <concepts>
#include <memory>
#include <stdexcept>
#include <utility>

namespace monopoly
{

template <typename T>
    requires std::derived_from<T, IRentable>
class RentCalculator
{
private:
    std::unique_ptr<IRentPolicy> m_policy{};

public:
    explicit RentCalculator(std::unique_ptr<IRentPolicy> policy)
    {
        SetPolicy(std::move(policy));
    }

    void SetPolicy(std::unique_ptr<IRentPolicy> policy)
    {
        if (!policy)
        {
            throw std::invalid_argument{ "Правило расчёта аренды не задано" };
        }
        m_policy = std::move(policy);
    }

    [[nodiscard]] int Calculate(const T& property) const
    {
        return Calculate(property, *m_policy);
    }

    // Правило только для этого расчёта
    [[nodiscard]] int Calculate(const T& property, const IRentPolicy& policy) const
    {
        return policy.Apply(property.CalcRent());
    }

    template <typename U>
        requires std::derived_from<U, IRentable>
    [[nodiscard]] long long CalculateTotal(const T& first, const U& second) const
    {
        return static_cast<long long>(Calculate(first)) + m_policy->Apply(second.CalcRent());
    }
};

} // namespace monopoly
