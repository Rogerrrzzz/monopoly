#include "player.hpp"

#include "game_rule_error.hpp"

#include <iostream>
#include <limits>

namespace monopoly
{

Player::Player(int id, std::string_view name, int balance)
    : m_id{ id }
    , m_name{ name }
    , m_balance{ balance }
{
    if (balance < 0)
    {
        throw GameRuleError{ "Начальный баланс не может быть отрицательным" };
    }
    std::cout << "[Player] Создан игрок \"" << m_name << "\" (id " << m_id << "), баланс " << m_balance << std::endl;
}

Player::~Player() noexcept
{
    std::cout << "[Player] Уничтожается игрок \"" << m_name << "\" (доступных участков: " << GetPropertyCount() << ")"
              << std::endl;
}

std::size_t Player::GetPropertyCount() const
{
    std::size_t count = 0;
    for (std::size_t i = 0; i < m_properties.size(); ++i)
    {
        if (std::shared_ptr<BaseProperty> property = m_properties[i].lock())
        {
            if (property->GetOwnerId() == m_id)
            {
                ++count;
            }
        }
    }
    for (std::size_t i = 0; i < m_propertyGroups.size(); ++i)
    {
        if (std::shared_ptr<PropertyGroup> group = m_propertyGroups[i].lock())
        {
            count += group->GetOwnedPropertyCount(m_id);
        }
    }
    return count;
}

void Player::Pay(int amount)
{
    if (amount < 0)
    {
        throw GameRuleError{ "Нельзя заплатить отрицательную сумму" };
    }
    if (amount > m_balance)
    {
        throw GameRuleError{ "У игрока " + m_name + " недостаточно денег: нужно " + std::to_string(amount) + ", есть " +
                             std::to_string(m_balance) };
    }
    m_balance -= amount;
}

void Player::Receive(int amount)
{
    if (amount < 0 || amount > std::numeric_limits<int>::max() - m_balance)
    {
        throw GameRuleError{ "Недопустимая сумма пополнения баланса" };
    }
    m_balance += amount;
}

void Player::BuyProperty(const std::shared_ptr<BaseProperty>& property)
{
    if (!property)
    {
        throw GameRuleError{ "Недвижимость не указана" };
    }
    if (m_id <= 0)
    {
        throw GameRuleError{ "Перед покупкой задайте положительный id игрока" };
    }
    if (property->IsOwned())
    {
        throw GameRuleError{ "Недвижимость " + std::string(property->GetName()) + " уже принадлежит игроку " +
                             std::to_string(property->GetOwnerId()) };
    }

    bool alreadyStored = false;
    for (const auto& stored : m_properties)
    {
        if (stored.lock() == property)
        {
            alreadyStored = true;
            break;
        }
    }
    if (!alreadyStored)
    {
        // Возможное выделение памяти выполняется до списания денег
        m_properties.reserve(m_properties.size() + 1);
    }
    Pay(property->GetPrice());
    property->AssignOwner(m_id);
    if (!alreadyStored)
    {
        m_properties.push_back(property);
    }
    std::cout << "Игрок " << m_name << " купил недвижимость " << property->GetName() << " за " << property->GetPrice()
              << std::endl;
}

void Player::BuyProperty(const std::shared_ptr<PropertyGroup>& group, std::size_t index)
{
    if (!group)
    {
        throw GameRuleError{ "Группа не указана" };
    }
    bool alreadyStored = false;
    for (const auto& stored : m_propertyGroups)
    {
        if (stored.lock() == group)
        {
            alreadyStored = true;
            break;
        }
    }
    if (!alreadyStored)
    {
        m_propertyGroups.reserve(m_propertyGroups.size() + 1);
    }
    group->BuyProperty(index, *this);
    if (!alreadyStored)
    {
        m_propertyGroups.push_back(group);
    }
}

void Player::MortgageProperty(BaseProperty& property)
{
    if (property.GetOwnerId() != m_id)
    {
        throw GameRuleError{ "Недвижимость не принадлежит игроку " + m_name };
    }
    const int amount = property.GetMortgageValue();
    if (amount > std::numeric_limits<int>::max() - m_balance)
    {
        throw GameRuleError{ "Залог приведёт к переполнению баланса" };
    }
    property.Mortgage();
    Receive(amount);
    std::cout << "Игрок " << m_name << " заложил недвижимость " << property.GetName() << ", получено " << amount
              << std::endl;
}

void Player::BuildOnProperty(PropertyGroup& group, std::size_t index)
{
    group.BuildOn(index, *this);
}

void Player::MortgageProperty(PropertyGroup& group, std::size_t index)
{
    group.MortgageProperty(index, *this);
}

void Player::SellBuildingOnProperty(Property& property)
{
    if (property.GetOwnerId() != m_id)
    {
        throw GameRuleError{ "Участок не принадлежит игроку " + m_name };
    }
    const int amount = property.GetBuildRefund();
    if (amount > std::numeric_limits<int>::max() - m_balance)
    {
        throw GameRuleError{ "Продажа постройки приведёт к переполнению баланса" };
    }
    property.SellBuilding();
    Receive(amount);
    std::cout << "Игрок " << m_name << " продал постройку на участке " << property.GetName() << ", получено " << amount
              << std::endl;
}

void Player::PrintInfo() const
{
    std::cout << "Игрок \"" << m_name << "\" (id " << m_id << "): баланс " << m_balance << ", участков "
              << GetPropertyCount() << std::endl;

    for (std::size_t i = 0; i < m_properties.size(); ++i)
    {
        if (std::shared_ptr<BaseProperty> property = m_properties[i].lock())
        {
            if (property->GetOwnerId() == m_id)
            {
                std::cout << "  - " << property->Describe() << std::endl;
            }
        }
        else
        {
            std::cout << "  - участок больше не существует" << std::endl;
        }
    }
    for (std::size_t i = 0; i < m_propertyGroups.size(); ++i)
    {
        if (std::shared_ptr<PropertyGroup> group = m_propertyGroups[i].lock())
        {
            group->PrintOwnedProperties(m_id);
        }
        else
        {
            std::cout << "  - группа и её участки больше не существуют" << std::endl;
        }
    }
}

void Player::ReportProperty(const BaseProperty& property) const
{
    std::cout << "Игрок \"" << m_name << "\" осматривает: " << property.Describe() << std::endl;
}

void Player::SellBuildingOnProperty(PropertyGroup& group, std::size_t index)
{
    group.SellBuildingOnProperty(index, *this);
}

} // namespace monopoly
