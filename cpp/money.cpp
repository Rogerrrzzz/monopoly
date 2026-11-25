#include "money.hpp"

#include <limits>
#include <ostream>
#include <stdexcept>

namespace monopoly
{

Money::Money(long long amount)
    : m_amount{ amount }
{
}

Money& Money::operator+=(const Money& other)
{
    if ((other.m_amount > 0 && m_amount > std::numeric_limits<long long>::max() - other.m_amount) ||
        (other.m_amount < 0 && m_amount < std::numeric_limits<long long>::min() - other.m_amount))
    {
        throw std::overflow_error{ "Денежная сумма не помещается в long long" };
    }

    m_amount += other.m_amount;
    return *this;
}

Money operator+(Money left, const Money& right)
{
    left += right;
    return left;
}

std::ostream& operator<<(std::ostream& out, const Money& money)
{
    return out << money.m_amount;
}

} // namespace monopoly
