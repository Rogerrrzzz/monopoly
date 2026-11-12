#pragma once

#include "property_info.hpp"

#include <memory>
#include <string>
#include <string_view>

namespace monopoly
{

    class Player;

    class BaseProperty
    {
    public:
        static constexpr int BANK_OWNER_ID = -1;

    private:
        inline static int m_createdCount{ 0 };
        std::shared_ptr<PropertyInfo> m_info{};
        int m_ownerId{ BANK_OWNER_ID };
        bool m_isMortgaged{ false };

    protected:
        BaseProperty();
        BaseProperty(std::string_view name, int price, int baseRent);
        BaseProperty(const BaseProperty& other);
        BaseProperty(BaseProperty&& other) noexcept;
        BaseProperty& operator=(const BaseProperty& other);
        BaseProperty& operator=(BaseProperty&& other) noexcept;

        // Детали реализации: нужны наследникам, но не вызывающему коду.
        [[nodiscard]] std::string Header() const;
        void CopySharedStateTo(BaseProperty& target) const;

    public:
        virtual ~BaseProperty();
        [[nodiscard]] std::string_view GetName() const;
        [[nodiscard]] int GetPrice() const;
        [[nodiscard]] int GetBaseRent() const;
        [[nodiscard]] int GetOwnerId() const;
        [[nodiscard]] bool IsOwned() const;
        [[nodiscard]] bool IsMortgaged() const;
        [[nodiscard]] int GetMortgageValue() const;
        [[nodiscard]] int GetRedeemCost() const;

        void Rename(std::string_view name);
        void SetBaseRent(int baseRent);
        bool AssignOwner(int playerId);
        virtual bool Mortgage();
        bool Redeem();
        virtual void ReturnToBank();

        [[nodiscard]] virtual int CalcRent() const = 0;
        [[nodiscard]] virtual std::string Describe() const;
        [[nodiscard]] virtual std::unique_ptr<BaseProperty> ShallowCopy() const = 0;
        [[nodiscard]] virtual std::unique_ptr<BaseProperty> Clone() const = 0;

        void PrintInfo() const;
        void ShowTo(const Player& player) const;
        [[nodiscard]] static int GetCreatedCount();
    };

} // namespace monopoly
