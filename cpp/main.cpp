#include "base_property.hpp"
#include "double_rent_policy.hpp"
#include "game_rule_error.hpp"
#include "player.hpp"
#include "property.hpp"
#include "property_group.hpp"
#include "railway_station.hpp"
#include "rent_calculator.hpp"
#include "standard_rent_policy.hpp"

#include <exception>
#include <iostream>
#include <memory>

#include <windows.h>

int main()
{
    SetConsoleOutputCP(1251);
    std::cout << "ЛР 5. Полиморфизм" << std::endl;

    // 1-3. Интерфейс, две реализации и множественное наследование
    std::cout << "\n1. Интерфейс IRentable и множественное наследование" << std::endl;
    monopoly::Property street("Арбат", 200, 20);
    street.AssignOwner(1);
    street.Build();
    monopoly::RailwayStation station("Рижский вокзал", 200, 25);
    station.AssignOwner(1);
    monopoly::RentCalculator calculator(std::make_unique<monopoly::StandardRentPolicy>());

    const monopoly::IRentable& rentableStreet = street;
    const monopoly::IRentable& rentableStation = station;
    std::cout << "Аренда участка через IRentable: " << calculator.Calculate(rentableStreet) << std::endl;
    std::cout << "Аренда вокзала через IRentable: " << calculator.Calculate(rentableStation) << std::endl;

    // 4. Виртуальный и невиртуальный методы через ссылку на базовый класс
    std::cout << "\n2. Раннее и позднее связывание" << std::endl;
    const monopoly::BaseProperty& base = street;
    std::cout << "Раннее, BaseProperty& -> GetName(): " << base.GetName() << std::endl;
    std::cout << "Позднее, BaseProperty& -> Describe(): " << base.Describe() << std::endl;

    // 5. Проверка поддержки интерфейса во время выполнения
    std::cout << "\n3. Проверка типа: dynamic_cast" << std::endl;
    if (const auto* rentable = dynamic_cast<const monopoly::IRentable*>(&base))
    {
        std::cout << base.GetName() << " поддерживает IRentable, аренда: " << calculator.Calculate(*rentable) << std::endl;
    }
    else
    {
        std::cout << base.GetName() << " не поддерживает IRentable" << std::endl;
    }

    // 6. Смена объекта поведения у существующего калькулятора
    std::cout << "\n4. Динамическое конфигурирование аренды" << std::endl;
    std::cout << "Обычная аренда: " << calculator.Calculate(street) << std::endl;
    calculator.SetPolicy(std::make_unique<monopoly::DoubleRentPolicy>());
    std::cout << "Двойная аренда: " << calculator.Calculate(street) << std::endl;

    // 7. Успешное действие и перехваченное нарушение правила
    std::cout << "\n5. Исключения и игровые правила" << std::endl;
    auto red = std::make_shared<monopoly::PropertyGroup>("Красные", 2);
    red->AddProperty("Арбат", 200, 20);
    red->AddProperty("Тверская", 220, 22);
    monopoly::Player bogdan(1, "Богдан", 1000);

    try
    {
        bogdan.BuyProperty(red, 0);
        bogdan.BuildOnProperty(*red, 0);
    }
    catch (const monopoly::GameRuleError& error)
    {
        std::cout << "[Нарушение правила] " << error.what() << ". Строительство отменено" << std::endl;
    }
    catch (const std::exception& error)
    {
        std::cout << "[Ошибка действия] " << error.what() << ". Действие отменено" << std::endl;
    }

    return 0;
}
