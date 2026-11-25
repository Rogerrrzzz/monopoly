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
    std::vector<std::weak_ptr<BaseProperty>> m_properties{};
    std::vector<std::weak_ptr<PropertyGroup>> m_propertyGroups{};

public:
    Player(int id, std::string_view name, int balance);
    ~Player() noexcept;

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

    // Нарушение денежных правил вызывает GameRuleError
    void Pay(int amount);
    void Receive(int amount);

    // Игрок сохраняет слабую ссылку, не становясь владельцем объекта в памяти.
    void BuyProperty(const std::shared_ptr<BaseProperty>& property);
    void BuyProperty(const std::shared_ptr<PropertyGroup>& group, std::size_t index);

    // Залог своего участка в обмен на деньги от банка
    void MortgageProperty(BaseProperty& property);
    void MortgageProperty(PropertyGroup& group, std::size_t index);

    // Застройка участка группы за деньги игрока
    void BuildOnProperty(PropertyGroup& group, std::size_t index);

    // Продажа постройки банку за часть её стоимости
    void SellBuildingOnProperty(Property& property);
    void SellBuildingOnProperty(PropertyGroup& group, std::size_t index);

    // Вывод состояния игрока в консоль
    void PrintInfo() const;
    void ReportProperty(const BaseProperty& property) const;
};

} // namespace monopoly
