#pragma once

#include "property.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace monopoly
{

    class Player;

    // Монополия: группа участков одной категории.
    // Композиция: участки хранятся по значению, создаются и уничтожаются вместе с группой
    class PropertyGroup
    {
    private:
        static constexpr auto BASE_NAME = "nogroup";

        std::string m_name{ BASE_NAME };
        std::vector<Property> m_properties{};
        std::size_t m_capacity{ 0 };

    public:
        PropertyGroup(std::string_view name, std::size_t capacity);
        ~PropertyGroup();

        // Группа имеет собственные части и не копируется.
        PropertyGroup(const PropertyGroup&) = delete;
        PropertyGroup& operator=(const PropertyGroup&) = delete;

        [[nodiscard]] std::string_view GetName() const
        {
            return m_name;
        }

        [[nodiscard]] std::size_t GetCount() const
        {
            return m_properties.size();
        }

        [[nodiscard]] std::size_t GetCapacity() const
        {
            return m_capacity;
        }

        // Участок создаётся прямо внутри группы; false - если группа уже заполнена
        bool AddProperty(std::string_view name, int price, int baseRent);

        // Операции с частями выполняются через целое; ссылки на части наружу не выдаются.
        bool BuyProperty(std::size_t index, Player& player);
        bool MortgageProperty(std::size_t index, Player& player);
        bool SellBuildingOnProperty(std::size_t index, Player& player);

        [[nodiscard]] std::size_t GetOwnedPropertyCount(int playerId) const;

        // Группа укомплектована, и все её участки принадлежат одному игроку (игрок собрал монополию)
        [[nodiscard]] bool IsFullyOwnedBy(int playerId) const;

        // Группа проверяет правила участка, Player выполняет денежные операции.
        bool BuildOn(std::size_t index, Player& player);

        // Вывод состояния группы и всех её участков в консоль
        void PrintInfo() const;
        void PrintOwnedProperties(int playerId) const;

    private:
        [[nodiscard]] bool HasProperty(std::size_t index) const;
    };

} // namespace monopoly
