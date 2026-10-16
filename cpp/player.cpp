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
        // Слабые ссылки не удерживают участки и группы в памяти.
        std::cout << "[Player] Уничтожается игрок \"" << m_name << "\" (доступных участков: " << GetPropertyCount()
            << ")" << std::endl;
    }

    std::size_t Player::GetPropertyCount() const
    {
        std::size_t count = 0;
        for (std::size_t i = 0; i < m_properties.size(); ++i)
        {
            if (std::shared_ptr<Property> property = m_properties[i].lock())
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

    bool Player::BuyProperty(const std::shared_ptr<Property>& property)
    {
        if (!property)
        {
            std::cout << "[Отказ] Участок не указан" << std::endl;
            return false;
        }

        if (property->IsOwned())
        {
            std::cout << "[Отказ] Участок \"" << property->GetName() << "\" уже принадлежит игроку "
                << property->GetOwnerId() << std::endl;
            return false;
        }

        if (!Pay(property->GetPrice()))
        {
            return false;
        }

        property->AssignOwner(m_id);
        bool alreadyStored = false;
        for (std::size_t i = 0; i < m_properties.size(); ++i)
        {
            if (m_properties[i].lock() == property)
            {
                alreadyStored = true;
                break;
            }
        }
        if (!alreadyStored)
        {
            m_properties.push_back(property);
        }

        std::cout << "Игрок \"" << m_name << "\" купил участок \"" << property->GetName() << "\" за " << property->GetPrice()
            << std::endl;
        return true;
    }

    bool Player::BuyProperty(const std::shared_ptr<PropertyGroup>& group, std::size_t index)
    {
        if (!group)
        {
            std::cout << "[Отказ] Группа не указана" << std::endl;
            return false;
        }
        if (!group->BuyProperty(index, *this))
        {
            return false;
        }
        for (std::size_t i = 0; i < m_propertyGroups.size(); ++i)
        {
            if (m_propertyGroups[i].lock() == group)
            {
                return true;
            }
        }
        m_propertyGroups.push_back(group);
        return true;
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
        return group.BuildOn(index, *this);
    }

    bool Player::MortgageProperty(PropertyGroup& group, std::size_t index)
    {
        return group.MortgageProperty(index, *this);
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
            << GetPropertyCount() << std::endl;

        for (std::size_t i = 0; i < m_properties.size(); ++i)
        {
            if (std::shared_ptr<Property> property = m_properties[i].lock())
            {
                if (property->GetOwnerId() == m_id)
                {
                    std::cout << "  - " << property->GetName() << ", уровень застройки " << property->GetBuildLevel()
                        << (property->IsMortgaged() ? ", в залоге" : "") << std::endl;
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

    bool Player::SellBuildingOnProperty(PropertyGroup& group, std::size_t index)
    {
        return group.SellBuildingOnProperty(index, *this);
    }

} // namespace monopoly
