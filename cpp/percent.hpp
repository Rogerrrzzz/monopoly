#pragma once

#include <concepts>
#include <stdexcept>

namespace monopoly
{

template <typename T>
concept Numeric = (std::integral<T> && !std::same_as<T, bool>) || std::floating_point<T>;

template <Numeric T>
[[nodiscard]] double Percent(T part, T whole)
{
    if (whole == 0)
    {
        throw std::invalid_argument{ "Нельзя вычислить процент от нуля" };
    }

    return (static_cast<double>(part) / static_cast<double>(whole)) * 100.0;
}

} // namespace monopoly
