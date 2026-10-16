#include "player.hpp"

#include <iostream>

namespace monopoly
{

    Player::Player(int id, std::string_view name, int balance)
        : m_id{ id }
        , m_name{ name }
        , m_balance{ balance }
    {
        std::cout << "[Player] Создан игрок \"" << m_name << "\" (id " << m_id << "), баланс " << m_balance << std::endl;
    }

    Player::~Player()
    {
        // Участки здесь НЕ уничтожаются: игрок лишь хранит на них указатели
        std::cout << "[Player] Уничтожается игрок \"" << m_name << "\" (указателей на участки: " << m_properties.size()
            << ")" << std::endl;
    }

    bool Player::Pay(int amount)
    {
        if (amount < 0)
        {
            std::cout << "[Отказ] Нельзя заплатить отрицательную сумму" << std::endl;
            return false;
        }

        if (amount > m_balance)
        {
            std::cout << "[Отказ] У игрока \"" << m_name << "\" недостаточно денег: нужно " << amount << ", есть "
                << m_balance << std::endl;
            return false;
        }

        m_balance -= amount;
        return true;
    }

    bool Player::Receive(int amount)
    {
        if (amount < 0)
        {
            std::cout << "[Отказ] Нельзя получить отрицательную сумму" << std::endl;
            return false;
        }

        m_balance += amount;
        return true;
    }

    bool Player::BuyProperty(Property& property)
    {
        if (property.IsOwned())
        {
            std::cout << "[Отказ] Участок \"" << property.GetName() << "\" уже принадлежит игроку "
                << property.GetOwnerId() << std::endl;
            return false;
        }

        if (!Pay(property.GetPrice()))
        {
            return false;
        }

        property.AssignOwner(m_id);
        m_properties.push_back(&property);

        std::cout << "Игрок \"" << m_name << "\" купил участок \"" << property.GetName() << "\" за " << property.GetPrice()
            << std::endl;
        return true;
    }

    bool Player::BuyProperty(Property* property)
    {
        if (property == nullptr)
        {
            std::cout << "[Отказ] Участок не указан (пустой указатель)" << std::endl;
            return false;
        }
        return BuyProperty(*property);
    }

    bool Player::MortgageProperty(Property& property)
    {
        if (property.GetOwnerId() != m_id)
        {
            std::cout << "[Отказ] Участок \"" << property.GetName() << "\" не принадлежит игроку \"" << m_name << "\""
                << std::endl;
            return false;
        }

        if (!property.Mortgage())
        {
            return false;
        }

        Receive(property.GetMortgageValue());

        std::cout << "Игрок \"" << m_name << "\" заложил участок \"" << property.GetName() << "\", получено "
            << property.GetMortgageValue() << std::endl;
        return true;
    }

    bool Player::BuildOnProperty(PropertyGroup& group, std::size_t index)
    {
        const Property* property = group.GetProperty(index);
        if (property == nullptr)
        {
            return false;
        }

        const int cost = property->GetBuildCost();
        if (m_balance < cost)
        {
            std::cout << "[Отказ] У игрока \"" << m_name << "\" не хватает денег на постройку: нужно " << cost
                << ", есть " << m_balance << std::endl;
            return false;
        }

        if (!group.BuildOn(index, m_id))
        {
            return false;
        }

        Pay(cost);

        std::cout << "Игрок \"" << m_name << "\" построил на участке \"" << property->GetName() << "\" за " << cost
            << std::endl;
        return true;
    }

    bool Player::SellBuildingOnProperty(Property& property)
    {
        if (property.GetOwnerId() != m_id)
        {
            std::cout << "[Отказ] Участок \"" << property.GetName() << "\" не принадлежит игроку \"" << m_name << "\""
                << std::endl;
            return false;
        }

        if (!property.SellBuilding())
        {
            return false;
        }

        Receive(property.GetBuildRefund());

        std::cout << "Игрок \"" << m_name << "\" продал постройку на участке \"" << property.GetName() << "\", получено "
            << property.GetBuildRefund() << std::endl;
        return true;
    }

    void Player::PrintInfo() const
    {
        std::cout << "Игрок \"" << m_name << "\" (id " << m_id << "): баланс " << m_balance << ", участков "
            << m_properties.size() << std::endl;

        for (const Property* property : m_properties)
        {
            std::cout << "  - " << property->GetName() << ", уровень застройки " << property->GetBuildLevel()
                << (property->IsMortgaged() ? ", в залоге" : "") << std::endl;
        }
    }

} // namespace monopoly