#pragma once

#include "property.hpp"
#include "property_group.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace monopoly
{

    // Игрок: личный баланс и список купленных участков.
    // Агрегация: weak_ptr наблюдает за созданными снаружи участками и группами.
    class Player
    {
    private:
        static constexpr auto BASE_NAME = "noname";

        int m_id{ 0 }; // идентификаторы игроков начинаются с 1, 0 - "не задан"
        std::string m_name{ BASE_NAME };
        int m_balance{ 0 };
        std::vector<std::weak_ptr<Property>> m_properties{};
        std::vector<std::weak_ptr<PropertyGroup>> m_propertyGroups{};

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

        [[nodiscard]] std::size_t GetPropertyCount() const;

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

        // Игрок сохраняет слабую ссылку, не становясь владельцем объекта в памяти.
        bool BuyProperty(const std::shared_ptr<Property>& property);
        bool BuyProperty(const std::shared_ptr<PropertyGroup>& group, std::size_t index);

        // Залог своего участка в обмен на деньги от банка
        bool MortgageProperty(Property& property);
        bool MortgageProperty(PropertyGroup& group, std::size_t index);

        // Застройка участка группы за деньги игрока
        bool BuildOnProperty(PropertyGroup& group, std::size_t index);

        // Продажа постройки банку за часть её стоимости
        bool SellBuildingOnProperty(Property& property);
        bool SellBuildingOnProperty(PropertyGroup& group, std::size_t index);

        // Вывод состояния игрока в консоль
        void PrintInfo() const;
    };

} // namespace monopoly
