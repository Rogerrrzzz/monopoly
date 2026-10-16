#include "property_group.hpp"

#include <iostream>

namespace monopoly
{

    PropertyGroup::PropertyGroup(std::string_view name, std::size_t capacity)
        : m_name{ name }
        , m_capacity{ capacity }
    {
        // Память выделяется один раз, чтобы вектор не перемещал участки
        // (иначе указатели игроков на участки станут недействительными)
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

    Property* PropertyGroup::GetProperty(std::size_t index)
    {
        if (index >= m_properties.size())
        {
            std::cout << "[Отказ] В группе \"" << m_name << "\" нет участка с номером " << index << std::endl;
            return nullptr;
        }
        return &m_properties[index];
    }

    const Property* PropertyGroup::GetProperty(std::size_t index) const
    {
        if (index >= m_properties.size())
        {
            std::cout << "[Отказ] В группе \"" << m_name << "\" нет участка с номером " << index << std::endl;
            return nullptr;
        }
        return &m_properties[index];
    }

    bool PropertyGroup::IsFullyOwnedBy(int playerId) const
    {
        if (playerId == Property::BANK_OWNER_ID || m_properties.size() != m_capacity || m_capacity == 0)
        {
            return false;
        }

        for (const auto& property : m_properties)
        {
            if (property.GetOwnerId() != playerId)
            {
                return false;
            }
        }
        return true;
    }

    bool PropertyGroup::BuildOn(std::size_t index, int playerId)
    {
        Property* property = GetProperty(index);
        if (property == nullptr)
        {
            return false;
        }

        if (property->GetOwnerId() != playerId)
        {
            std::cout << "[Отказ] Участок \"" << property->GetName() << "\" не принадлежит игроку " << playerId
                << std::endl;
            return false;
        }

        if (!IsFullyOwnedBy(playerId))
        {
            std::cout << "[Отказ] Игрок " << playerId << " не владеет всей группой \"" << m_name
                << "\", застройка запрещена" << std::endl;
            return false;
        }

        return property->Build();
    }

    void PropertyGroup::PrintInfo() const
    {
        std::cout << "Группа \"" << m_name << "\" (" << m_properties.size() << "/" << m_capacity << "):" << std::endl;

        for (const auto& property : m_properties)
        {
            std::cout << "  ";
            property.PrintInfo();
        }
    }

} // namespace monopoly