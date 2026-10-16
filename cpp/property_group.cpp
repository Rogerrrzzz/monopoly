#include "property_group.hpp"

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

    PropertyGroup::~PropertyGroup()
    {
        std::cout << "[PropertyGroup] Уничтожается группа \"" << m_name << "\" (участков: " << m_properties.size() << ")"
            << std::endl;
    }

    bool PropertyGroup::AddProperty(std::string_view name, int price, int baseRent)
    {
        if (m_properties.size() >= m_capacity)
        {
            std::cout << "[Отказ] Группа \"" << m_name << "\" уже заполнена, участок \"" << name << "\" не добавлен"
                << std::endl;
            return false;
        }

        m_properties.emplace_back(name, price, baseRent);
        return true;
    }

    bool PropertyGroup::HasProperty(std::size_t index) const
    {
        if (index >= m_properties.size())
        {
            std::cout << "[Отказ] В группе \"" << m_name << "\" нет участка с номером " << index << std::endl;
            return false;
        }
        return true;
    }

    bool PropertyGroup::BuyProperty(std::size_t index, Player& player)
    {
        if (!HasProperty(index))
        {
            return false;
        }
        if (m_properties[index].IsOwned())
        {
            std::cout << "[Отказ] Участок \"" << m_properties[index].GetName() << "\" уже принадлежит игроку "
                << m_properties[index].GetOwnerId() << std::endl;
            return false;
        }
        if (!player.Pay(m_properties[index].GetPrice()))
        {
            return false;
        }
        m_properties[index].AssignOwner(player.GetId());
        std::cout << "Игрок \"" << player.GetName() << "\" купил участок \"" << m_properties[index].GetName()
            << "\" за " << m_properties[index].GetPrice() << std::endl;
        return true;
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

    bool PropertyGroup::BuildOn(std::size_t index, Player& player)
    {
        if (!HasProperty(index))
        {
            return false;
        }

        const int cost = m_properties[index].GetBuildCost();
        if (player.GetBalance() < cost)
        {
            std::cout << "[Отказ] У игрока \"" << player.GetName() << "\" не хватает денег на постройку: нужно " << cost
                << ", есть " << player.GetBalance() << std::endl;
            return false;
        }

        if (m_properties[index].GetOwnerId() != player.GetId())
        {
            std::cout << "[Отказ] Участок \"" << m_properties[index].GetName() << "\" не принадлежит игроку " << player.GetId()
                << std::endl;
            return false;
        }

        if (!IsFullyOwnedBy(player.GetId()))
        {
            std::cout << "[Отказ] Игрок " << player.GetId() << " не владеет всей группой \"" << m_name
                << "\", застройка запрещена" << std::endl;
            return false;
        }

        if (!m_properties[index].Build())
        {
            return false;
        }
        player.Pay(cost);
        std::cout << "Игрок \"" << player.GetName() << "\" построил на участке \"" << m_properties[index].GetName()
            << "\" за " << cost << std::endl;
        return true;
    }

    bool PropertyGroup::MortgageProperty(std::size_t index, Player& player)
    {
        if (!HasProperty(index))
        {
            return false;
        }
        return player.MortgageProperty(m_properties[index]);
    }

    bool PropertyGroup::SellBuildingOnProperty(std::size_t index, Player& player)
    {
        if (!HasProperty(index))
        {
            return false;
        }
        return player.SellBuildingOnProperty(m_properties[index]);
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
