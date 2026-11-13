#pragma once

namespace monopoly
{

class IRentable
{
public:
    virtual ~IRentable() noexcept = default;
    [[nodiscard]] virtual int CalcRent() const = 0;
};

} // namespace monopoly
