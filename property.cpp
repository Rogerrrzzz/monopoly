#include "property.hpp"

#include <array>
#include <iostream>

namespace monopoly
{

    namespace
    {

        // Множитель аренды для каждого уровня застройки (индекс - уровень)
        constexpr std::array<int, Property::MAX_BUILD_LEVEL + 1> RENT_MULTIPLIERS{ 1, 2, 3, 5, 8 };

    } // namespace

    Property::Property(std::string_view name, int price, int baseRent)
        : m_name{ name }
        , m_price{ price }
        , m_baseRent{ baseRent }
    {
        std::cout << "[Property] Создан участок \"" << m_name << "\", цена " << m_price << std::endl;
    }

    Property::~Property()
    {
        std::cout << "[Property] Уничтожается участок \"" << m_name << "\"" << std::endl;
    }

    int Property::CalcRent() const
    {
        if (!IsOwned() || m_isMortgaged)
        {
            return 0;
        }
        return m_baseRent * RENT_MULTIPLIERS[m_buildLevel];
    }

    bool Property::AssignOwner(int playerId)
    {
        if (IsOwned())
        {
            std::cout << "[Отказ] Участок \"" << m_name << "\" уже принадлежит игроку " << m_ownerId << std::endl;
            return false;
        }

        m_ownerId = playerId;
        return true;
    }

    bool Property::Build()
    {
        if (m_isMortgaged)
        {
            std::cout << "[Отказ] Нельзя строить на заложенном участке \"" << m_name << "\"" << std::endl;
            return false;
        }

        if (m_buildLevel >= MAX_BUILD_LEVEL)
        {
            std::cout << "[Отказ] На участке \"" << m_name << "\" уже максимальный уровень застройки" << std::endl;
            return false;
        }

        ++m_buildLevel;
        return true;
    }

    bool Property::SellBuilding()
    {
        if (m_buildLevel == 0)
        {
            std::cout << "[Отказ] На участке \"" << m_name << "\" нет построек" << std::endl;
            return false;
        }

        --m_buildLevel;
        return true;
    }

    bool Property::Mortgage()
    {
        if (m_buildLevel > 0)
        {
            std::cout << "[Отказ] Нельзя заложить участок \"" << m_name << "\", пока на нём есть постройки" << std::endl;
            return false;
        }

        if (m_isMortgaged)
        {
            std::cout << "[Отказ] Участок \"" << m_name << "\" уже заложен" << std::endl;
            return false;
        }

        m_isMortgaged = true;
        return true;
    }

    bool Property::Redeem()
    {
        if (!m_isMortgaged)
        {
            std::cout << "[Отказ] Участок \"" << m_name << "\" не находится в залоге" << std::endl;
            return false;
        }

        m_isMortgaged = false;
        return true;
    }

    void Property::ReturnToBank()
    {
        m_ownerId = BANK_OWNER_ID;
        m_buildLevel = 0;
        m_isMortgaged = false;
    }

    void Property::PrintInfo() const
    {
        std::cout << "Участок \"" << m_name << "\": цена " << m_price << ", ";

        if (IsOwned())
        {
            std::cout << "владелец - игрок " << m_ownerId;
        }
        else
        {
            std::cout << "владелец - банк";
        }

        std::cout << ", уровень застройки " << m_buildLevel << ", аренда " << CalcRent();

        if (m_isMortgaged)
        {
            std::cout << ", в залоге";
        }
        std::cout << std::endl;
    }
} // namespace monopoly