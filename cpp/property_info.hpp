#pragma once

#include <string>
#include <string_view>

namespace monopoly
{

    // Карточка недвижимости. Строка и числа управляют собой сами: правило нуля.
    class PropertyInfo
    {
    private:
        std::string m_name{};
        int m_price{ 0 };
        int m_baseRent{ 0 };

    public:
        PropertyInfo(std::string_view name, int price, int baseRent);
        [[nodiscard]] std::string_view GetName() const;
        [[nodiscard]] int GetPrice() const;
        [[nodiscard]] int GetBaseRent() const;
        void SetName(std::string_view name);
        void SetBaseRent(int baseRent);
    };

} // namespace monopoly
