#include "base_property.hpp"
#include "player.hpp"
#include "property.hpp"
#include "property_group.hpp"
#include "railway_station.hpp"

#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include <windows.h>

int main()
{
    SetConsoleOutputCP(1251);

    std::cout << "ЛР 4. Наследование" << std::endl;
    std::cout << "Создано объектов недвижимости: " << monopoly::BaseProperty::GetCreatedCount() << std::endl;

    // 1. Один базовый тип, разные реализации расчёта аренды и описания
    {
        std::cout << "\n1. Абстрактный класс и два наследника" << std::endl;
        auto street = std::make_unique<monopoly::Property>("Арбат", 200, 20);
        street->AssignOwner(1);
        street->Build();

        std::array<std::unique_ptr<monopoly::BaseProperty>, 2> properties{
            std::move(street),
            std::make_unique<monopoly::RailwayStation>("Рижский вокзал", 200, 25)
        };
        properties[1]->AssignOwner(1);
        for (std::size_t i = 0; i < properties.size(); ++i)
        {
            properties[i]->PrintInfo();
            std::cout << "Аренда через BaseProperty: " << properties[i]->CalcRent() << std::endl;
        }
        std::cout << "Выход из блока: для каждого объекта сначала деструктор наследника, затем базы." << std::endl;
    }

    // 2. Четыре вида конструкторов и согласованные операции присваивания
    {
        std::cout << "\n2. Конструкторы, копирование и перемещение" << std::endl;
        monopoly::Property empty;
        monopoly::Property original("Тверская", 220, 22);
        monopoly::Property copy(original);
        monopoly::Property moved(std::move(copy));
        empty = original;
        empty = empty; // this == &other
        monopoly::Property assigned;
        assigned = empty = original; // оператор возвращает *this
        assigned = std::move(moved);
        assigned.PrintInfo();
        copy.PrintInfo(); // перемещённый объект остаётся пригодным для вывода и присваивания
        copy = original;

        monopoly::RailwayStation emptyStation;
        monopoly::RailwayStation station("Казанский вокзал", 200, 25);
        monopoly::RailwayStation stationCopy(station);
        monopoly::RailwayStation movedStation(std::move(stationCopy));
        emptyStation = station;
        emptyStation = std::move(movedStation);
        emptyStation.PrintInfo();
        std::cout << "BaseProperty и наследники согласуют все пять специальных операций." << std::endl;
        std::cout << "PropertyInfo использует правило нуля: своих операций копирования и деструктора нет." << std::endl;
        std::cout << "Выход из блока: объекты уничтожаются в обратном порядке создания." << std::endl;
    }

    // 3. Вложенная карточка либо общая, либо независимая
    {
        std::cout << "\n3. Поверхностная и глубокая копии" << std::endl;
        monopoly::Property original("Арбат", 200, 20);
        original.AssignOwner(1);
        auto shallow = original.ShallowCopy();
        shallow->Rename("Арбат: общая карточка");
        shallow->SetBaseRent(30);
        std::cout << "После изменения поверхностной копии изменился и оригинал:" << std::endl;
        original.PrintInfo();
        shallow->PrintInfo();

        std::unique_ptr<monopoly::BaseProperty> deep = original.Clone();
        deep->Rename("Арбат: независимая карточка");
        deep->SetBaseRent(90);
        std::cout << "После изменения Clone оригинал сохраняет своё имя и аренду 30:" << std::endl;
        original.PrintInfo();
        deep->PrintInfo();

        monopoly::Property assigned;
        assigned = original;
        assigned.SetBaseRent(70);
        std::cout << "Присваивание тоже глубокое: аренда оригинала " << original.CalcRent()
            << ", копии " << assigned.CalcRent() << std::endl;

        // Clone вызывается через базовый указатель и сохраняет тип вокзала
        std::unique_ptr<monopoly::BaseProperty> station =
            std::make_unique<monopoly::RailwayStation>("Ленинградский вокзал", 200, 25);
        auto stationCopy = station->Clone();
        stationCopy->PrintInfo();
    }

    // 4. Удаление через базовый тип безопасно благодаря виртуальному деструктору
    {
        std::cout << "\n4. Виртуальный деструктор" << std::endl;
        std::unique_ptr<monopoly::BaseProperty> property =
            std::make_unique<monopoly::RailwayStation>("Павелецкий вокзал", 200, 25);
        std::cout << "reset: сначала RailwayStation, затем BaseProperty" << std::endl;
        property.reset();
        std::cout << "Без virtual уничтожение наследника через unique_ptr<BaseProperty> - неопределённое поведение."
            << std::endl;
    }

    // 5. Операции со строками по теме проекта
    {
        std::cout << "\n5. Строки" << std::endl;
        const std::string name = "Рижский вокзал";
        const std::string label = "Недвижимость: " + name;
        std::cout << "Склеивание: " << label << std::endl;
        const std::string word = "вокзал";
        const std::size_t position = name.find(word);
        if (position != std::string::npos)
        {
            const std::string part = name.substr(position, word.size());
            std::cout << "find и substr: " << part << std::endl;
            std::cout << "Сравнение содержимого: " << (part == word ? "совпало" : "не совпало") << std::endl;
        }
    }

    // 6. Передача текущего объекта через *this и статическое поле
    {
        std::cout << "\n6. this и статические члены" << std::endl;
        monopoly::Player viewer(7, "Покупатель", 500);
        monopoly::RailwayStation station("Рижский вокзал", 200, 25);
        station.ShowTo(viewer); // внутри: player.ReportProperty(*this)
        std::cout << "Всего создано объектов, включая копии: " << monopoly::BaseProperty::GetCreatedCount() << std::endl;
    }

    // 7. Исходный игровой сценарий сохраняется, вокзал теперь имеет свой тип
    {
        std::cout << "\n7. Композиция, агрегация и правила игры" << std::endl;
        auto red = std::make_shared<monopoly::PropertyGroup>("Красные", 2);
        red->AddProperty("Арбат", 200, 20);
        red->AddProperty("Тверская", 220, 22);
        monopoly::Player bogdan(1, "Богдан", 1000);
        monopoly::Player mark(2, "Марк", 1000);
        bogdan.BuyProperty(red, 0);
        bogdan.BuildOnProperty(*red, 0); // отказ: ещё нет полной монополии
        bogdan.BuyProperty(red, 1);
        bogdan.BuildOnProperty(*red, 0);
        mark.BuyProperty(red, 0); // отказ: участок занят
        bogdan.MortgageProperty(*red, 0); // отказ: есть постройка
        bogdan.SellBuildingOnProperty(*red, 0);
        bogdan.MortgageProperty(*red, 0);
        bogdan.PrintInfo(); // баланс 630

        auto station = std::make_shared<monopoly::RailwayStation>("Рижский вокзал", 200, 25);
        mark.BuyProperty(station);
        mark.PrintInfo();
    }

    std::cout << "\nИтоговый счётчик созданных объектов: " << monopoly::BaseProperty::GetCreatedCount() << std::endl;
    std::cout << "Счётчик показывает все создания, а не число живых объектов." << std::endl;
    return 0;
}
