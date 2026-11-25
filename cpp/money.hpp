#pragma once

#include <compare>
#include <iosfwd>

namespace monopoly
{

class Money
{
private:
    long long m_amount;

public:
    explicit Money(long long amount);

    [[nodiscard]] bool operator==(const Money& other) const = default;
    [[nodiscard]] auto operator<=>(const Money& other) const = default;
    Money& operator+=(const Money& other);

    friend std::ostream& operator<<(std::ostream& out, const Money& money);
};

[[nodiscard]] Money operator+(Money left, const Money& right);

} // namespace monopoly
