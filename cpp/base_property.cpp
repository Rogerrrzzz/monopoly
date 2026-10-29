#include "base_property.hpp"

#include "player.hpp"

#include <iostream>
#include <utility>

namespace monopoly
{

    BaseProperty::BaseProperty()
        : m_info{ std::make_shared<PropertyInfo>("Без названия", 0, 0) }
    {
        ++m_createdCount;
        std::cout << "[BaseProperty] Конструктор по умолчанию" << std::endl;
    }

    BaseProperty::BaseProperty(std::string_view name, int price, int baseRent)
        : m_info{ std::make_shared<PropertyInfo>(name, price, baseRent) }
    {
        ++m_createdCount;
        std::cout << "[BaseProperty] Конструктор с параметрами: " << GetName() << std::endl;
    }

    BaseProperty::BaseProperty(const BaseProperty& other)
        : m_info{ other.m_info ? std::make_shared<PropertyInfo>(*other.m_info) : nullptr }
        , m_ownerId{ other.m_ownerId }
        , m_isMortgaged{ other.m_isMortgaged }
    {
        ++m_createdCount;
        std::cout << "[BaseProperty] Конструктор копирования (глубокая копия): " << GetName() << std::endl;
    }

    BaseProperty::BaseProperty(BaseProperty&& other) noexcept
        : m_info{ std::move(other.m_info) }
        , m_ownerId{ other.m_ownerId }
        , m_isMortgaged{ other.m_isMortgaged }
    {
        other.m_ownerId = BANK_OWNER_ID;
        other.m_isMortgaged = false;
        ++m_createdCount;
        std::cout << "[BaseProperty] Конструктор перемещения: " << GetName() << std::endl;
    }

    BaseProperty& BaseProperty::operator=(const BaseProperty& other)
    {
        if (this == &other)
        {
            return *this;
        }
        // Сначала выделяем память: если это не удалось, старое состояние сохранится.
        std::shared_ptr<PropertyInfo> info = other.m_info
            ? std::make_shared<PropertyInfo>(*other.m_info) : nullptr;
        m_info = std::move(info);
        m_ownerId = other.m_ownerId;
        m_isMortgaged = other.m_isMortgaged;
        std::cout << "[BaseProperty] Присваивание копированием: " << GetName() << std::endl;
        return *this;
    }

    BaseProperty& BaseProperty::operator=(BaseProperty&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        m_info = std::move(other.m_info);
        m_ownerId = other.m_ownerId;
        m_isMortgaged = other.m_isMortgaged;
        other.m_ownerId = BANK_OWNER_ID;
        other.m_isMortgaged = false;
        std::cout << "[BaseProperty] Присваивание перемещением: " << GetName() << std::endl;
        return *this;
    }

    BaseProperty::~BaseProperty()
    {
        std::cout << "[BaseProperty] Деструктор: " << GetName() << std::endl;
    }

    std::string_view BaseProperty::GetName() const
    {
        return m_info ? m_info->GetName() : "Перемещённый объект";
    }

    int BaseProperty::GetPrice() const
    {
        return m_info ? m_info->GetPrice() : 0;
    }

    int BaseProperty::GetBaseRent() const
    {
        return m_info ? m_info->GetBaseRent() : 0;
    }

    int BaseProperty::GetOwnerId() const
    {
        return m_ownerId;
    }

    bool BaseProperty::IsOwned() const
    {
        return m_ownerId != BANK_OWNER_ID;
    }

    bool BaseProperty::IsMortgaged() const
    {
        return m_isMortgaged;
    }

    int BaseProperty::GetMortgageValue() const
    {
        return GetPrice() / 2;
    }

    int BaseProperty::GetRedeemCost() const
    {
        return GetMortgageValue() * 11 / 10;
    }

    void BaseProperty::Rename(std::string_view name)
    {
        if (!m_info)
        {
            m_info = std::make_shared<PropertyInfo>(name, 0, 0);
        }
        else
        {
            m_info->SetName(name);
        }
    }

    void BaseProperty::SetBaseRent(int baseRent)
    {
        if (!m_info)
        {
            m_info = std::make_shared<PropertyInfo>("Без названия", 0, baseRent);
        }
        else
        {
            m_info->SetBaseRent(baseRent);
        }
    }

    bool BaseProperty::AssignOwner(int playerId)
    {
        if (IsOwned() || playerId <= 0)
        {
            std::cout << "[Отказ] Недвижимость занята или указан неверный владелец: " << GetName() << std::endl;
            return false;
        }
        m_ownerId = playerId;
        return true;
    }

    bool BaseProperty::Mortgage()
    {
        if (m_isMortgaged)
        {
            std::cout << "[Отказ] Недвижимость уже в залоге: " << GetName() << std::endl;
            return false;
        }
        m_isMortgaged = true;
        return true;
    }

    bool BaseProperty::Redeem()
    {
        if (!m_isMortgaged)
        {
            std::cout << "[Отказ] Недвижимость не находится в залоге: " << GetName() << std::endl;
            return false;
        }
        m_isMortgaged = false;
        return true;
    }

    void BaseProperty::ReturnToBank()
    {
        m_ownerId = BANK_OWNER_ID;
        m_isMortgaged = false;
    }

    std::string BaseProperty::Header() const
    {
        const std::string owner = IsOwned() ? "игрок " + std::to_string(m_ownerId) : "банк";
        return "\"" + std::string(GetName()) + "\": цена " + std::to_string(GetPrice())
            + ", владелец - " + owner + (m_isMortgaged ? ", в залоге" : "");
    }

    std::string BaseProperty::Describe() const
    {
        return Header() + ", аренда " + std::to_string(CalcRent());
    }

    void BaseProperty::CopySharedStateTo(BaseProperty& target) const
    {
        target.m_info = m_info;
        target.m_ownerId = m_ownerId;
        target.m_isMortgaged = m_isMortgaged;
    }

    void BaseProperty::PrintInfo() const
    {
        std::cout << Describe() << std::endl;
    }

    void BaseProperty::ShowTo(const Player& player) const
    {
        player.ReportProperty(*this);
    }

    int BaseProperty::GetCreatedCount()
    {
        return m_createdCount;
    }

} // namespace monopoly
