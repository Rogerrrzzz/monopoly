#include "property_group.hpp"

#include "game_rule_error.hpp"
#include "player.hpp"

#include <iostream>

namespace monopoly
{

PropertyGroup::PropertyGroup(std::string_view name, std::size_t capacity)
    : m_name{ name }
    , m_capacity{ capacity }
{
    // Сразу резервируем место для всех частей группы.
    m_properties.reserve(m_capacity);

    std::cout << "[PropertyGroup] Создана группа \"" << m_name << "\", вместимость " << m_capacity << std::endl;
}

PropertyGroup::~PropertyGroup() noexcept
{
    std::cout << "[PropertyGroup] Уничтожается группа \"" << m_name << "\" (участков: " << m_properties.size() << ")"
              << std::endl;
}

void PropertyGroup::AddProperty(std::string_view name, int price, int baseRent)
{
    if (m_properties.size() >= m_capacity)
    {
        throw GameRuleError{ "Группа " + m_name + " уже заполнена" };
    }
    m_properties.emplace_back(name, price, baseRent);
}

void PropertyGroup::CheckIndex(std::size_t index) const
{
    if (index >= m_properties.size())
    {
        throw GameRuleError{ "В группе " + m_name + " нет участка с номером " + std::to_string(index) };
    }
}

void PropertyGroup::BuyProperty(std::size_t index, Player& player)
{
    CheckIndex(index);
    if (player.GetId() <= 0)
    {
        throw GameRuleError{ "Перед покупкой задайте положительный id игрока" };
    }
    Property& property = m_properties[index];
    if (property.IsOwned())
    {
        throw GameRuleError{ "Участок " + std::string(property.GetName()) + " уже принадлежит игроку " +
                             std::to_string(property.GetOwnerId()) };
    }
    player.Pay(property.GetPrice());
    property.AssignOwner(player.GetId());
    std::cout << "Игрок " << player.GetName() << " купил участок " << property.GetName() << " за "
              << property.GetPrice() << std::endl;
}

std::size_t PropertyGroup::GetOwnedPropertyCount(int playerId) const
{
    std::size_t count = 0;
    for (std::size_t i = 0; i < m_properties.size(); ++i)
    {
        if (m_properties[i].IsOwned() && m_properties[i].GetOwnerId() == playerId)
        {
            ++count;
        }
    }
    return count;
}

bool PropertyGroup::IsFullyOwnedBy(int playerId) const
{
    if (playerId == Property::BANK_OWNER_ID || m_properties.size() != m_capacity || m_capacity == 0)
    {
        return false;
    }

    return GetOwnedPropertyCount(playerId) == m_capacity;
}

void PropertyGroup::BuildOn(std::size_t index, Player& player)
{
    CheckIndex(index);
    Property& property = m_properties[index];
    if (property.GetOwnerId() != player.GetId())
    {
        throw GameRuleError{ "Участок не принадлежит игроку " + std::string(player.GetName()) };
    }
    if (!IsFullyOwnedBy(player.GetId()))
    {
        throw GameRuleError{ "Для строительства нужно владеть всей группой " + m_name };
    }
    const int cost = property.GetBuildCost();
    if (player.GetBalance() < cost)
    {
        throw GameRuleError{ "Недостаточно денег на постройку: нужно " + std::to_string(cost) + ", есть " +
                             std::to_string(player.GetBalance()) };
    }
    property.Build();
    player.Pay(cost);
    std::cout << "Игрок " << player.GetName() << " построил на участке " << property.GetName() << " за " << cost
              << std::endl;
}

void PropertyGroup::MortgageProperty(std::size_t index, Player& player)
{
    CheckIndex(index);
    player.MortgageProperty(m_properties[index]);
}

void PropertyGroup::SellBuildingOnProperty(std::size_t index, Player& player)
{
    CheckIndex(index);
    player.SellBuildingOnProperty(m_properties[index]);
}

void PropertyGroup::PrintInfo() const
{
    std::cout << "Группа \"" << m_name << "\" (" << m_properties.size() << "/" << m_capacity << "):" << std::endl;

    for (std::size_t i = 0; i < m_properties.size(); ++i)
    {
        std::cout << "  ";
        m_properties[i].PrintInfo();
    }
}

void PropertyGroup::PrintOwnedProperties(int playerId) const
{
    for (std::size_t i = 0; i < m_properties.size(); ++i)
    {
        if (m_properties[i].IsOwned() && m_properties[i].GetOwnerId() == playerId)
        {
            std::cout << "  - ";
            m_properties[i].PrintInfo();
        }
    }
}

} // namespace monopoly
