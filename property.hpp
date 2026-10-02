#pragma once

#include <string>
#include <string_view>

namespace monopoly
{

    // Участок: собственность с ценой, уровнем застройки и возможным владельцем
    class Property
    {
    public:
        static constexpr int BANK_OWNER_ID = -1;
        static constexpr int MAX_BUILD_LEVEL = 4;

    private:
        static constexpr auto BASE_NAME = "noname";

        std::string m_name{ BASE_NAME };
        int m_price{ 0 };
        int m_baseRent{ 0 };
        int m_buildLevel{ 0 };
        int m_ownerId{ BANK_OWNER_ID };
        bool m_isMortgaged{ false };

    public:
        Property() = default;
        Property(std::string_view name, int price, int baseRent);
        ~Property();

        [[nodiscard]] std::string_view GetName() const
        {
            return m_name;
        }

        [[nodiscard]] int GetPrice() const
        {
            return m_price;
        }

        [[nodiscard]] int GetBaseRent() const
        {
            return m_baseRent;
        }

        [[nodiscard]] int GetBuildLevel() const
        {
            return m_buildLevel;
        }

        [[nodiscard]] int GetOwnerId() const
        {
            return m_ownerId;
        }

        [[nodiscard]] bool IsOwned() const
        {
            return m_ownerId != BANK_OWNER_ID;
        }

        [[nodiscard]] bool IsMortgaged() const
        {
            return m_isMortgaged;
        }

        // Денежные величины, связанные с участком (сами деньги двигает Player)
        [[nodiscard]] int GetBuildCost() const
        {
            return m_price / 2;
        }

        [[nodiscard]] int GetBuildRefund() const
        {
            return GetBuildCost() / 2;
        }

        [[nodiscard]] int GetMortgageValue() const
        {
            return m_price / 2;
        }

        [[nodiscard]] int GetRedeemCost() const
        {
            return GetMortgageValue() * 11 / 10;
        }

        // Размер аренды с учётом уровня застройки; 0 для ничейного или заложенного участка
        [[nodiscard]] int CalcRent() const;

        // Операции с участком: true - выполнено, false - отказ по правилам игры
        bool AssignOwner(int playerId);
        bool Build();
        bool SellBuilding();
        bool Mortgage();
        bool Redeem();

        // Возврат участка банку (например, при банкротстве владельца)
        void ReturnToBank();

        // Вывод состояния участка в консоль
        void PrintInfo() const;
    };

} // namespace monopoly