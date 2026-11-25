#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

namespace monopoly
{

class GameRuleError : public std::runtime_error
{
public:
    explicit GameRuleError(std::string_view message)
        : std::runtime_error{ std::string(message) }
    {
    }
};

} // namespace monopoly
