#include "double_rent_policy.hpp"

#include "game_rule_error.hpp"

#include <limits>

namespace monopoly
{

int DoubleRentPolicy::Apply(int rent) const
{
    if (rent < 0 || rent > std::numeric_limits<int>::max() / 2)
    {
        throw GameRuleError{ "Недопустимая сумма двойной аренды" };
    }
    return rent * 2;
}

} // namespace monopoly
