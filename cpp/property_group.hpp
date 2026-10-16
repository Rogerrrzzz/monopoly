#pragma once

#include "property.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace monopoly
{

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
        PropertyGroup() = default;
        PropertyGroup(std::string_view name, std::size_t capacity);
        ~PropertyGroup();

        // Участки хранятся по значению, а игроки держат на них указатели: копировать группу нельзя
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

        // Доступ к части через целое; nullptr, если индекс неверный
        [[nodiscard]] Property* GetProperty(std::size_t index);
        [[nodiscard]] const Property* GetProperty(std::size_t index) const;

        // Группа укомплектована, и все её участки принадлежат одному игроку (игрок собрал монополию)
        [[nodiscard]] bool IsFullyOwnedBy(int playerId) const;

        // Застройка участка с проверкой правил; деньги списывает Player
        bool BuildOn(std::size_t index, int playerId);

        // Вывод состояния группы и всех её участков в консоль
        void PrintInfo() const;
    };

} // namespace monopoly