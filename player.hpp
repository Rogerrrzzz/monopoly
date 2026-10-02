#pragma once

#include "property.hpp"
#include "property_group.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace monopoly
{

    // Игрок: личный баланс и список купленных участков.
    // Агрегация: участки создаются вне игрока, он хранит только указатели на них
    // и не уничтожает участки при собственном уничтожении
    class Player
    {
    private:
        static constexpr auto BASE_NAME = "noname";

        int m_id{ 0 }; // идентификаторы игроков начинаются с 1, 0 - "не задан"
        std::string m_name{ BASE_NAME };
        int m_balance{ 0 };
        std::vector<Property*> m_properties{};

    public:
        Player() = default;
        Player(int id, std::string_view name, int balance);
        ~Player();

        [[nodiscard]] int GetId() const
        {
            return m_id;
        }

        [[nodiscard]] std::string_view GetName() const
        {
            return m_name;
        }

        [[nodiscard]] int GetBalance() const
        {
            return m_balance;
        }

        [[nodiscard]] std::size_t GetPropertyCount() const
        {
            return m_properties.size();
        }

        void SetId(int id)
        {
            m_id = id;
        }

        void SetName(std::string_view name)
        {
            m_name = std::string(name);
        }

        // Деньги: false - отказ (баланс не может стать отрицательным)
        bool Pay(int amount);
        bool Receive(int amount);

        // Покупка участка у банка (перегрузки: по ссылке и по указателю)
        bool BuyProperty(Property& property);
        bool BuyProperty(Property* property);

        // Залог своего участка в обмен на деньги от банка
        bool MortgageProperty(Property& property);

        // Застройка участка группы за деньги игрока
        bool BuildOnProperty(PropertyGroup& group, std::size_t index);

        // Продажа постройки банку за часть её стоимости
        bool SellBuildingOnProperty(Property& property);

        // Вывод состояния игрока в консоль
        void PrintInfo() const;
    };

} // namespace monopoly