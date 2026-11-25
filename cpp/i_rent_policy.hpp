#pragma once

namespace monopoly
{

class IRentPolicy
{
public:
    virtual ~IRentPolicy() noexcept = default;
    [[nodiscard]] virtual int Apply(int rent) const = 0;
};

} // namespace monopoly
