#include "player.hpp"
#include "property.hpp"
#include "property_group.hpp"

#include <iostream>
#include <string>

#include <windows.h>

int main()
{
    SetConsoleOutputCP(1251);

    // 1. Статическая инициализация. Сценарий 4: застройка и последующий залог
    {
        std::cout << "\n1. Статические объекты, сценарий 4" << std::endl;

        monopoly::PropertyGroup red("Красные", 2); // композиция: участки создаются внутри группы
        red.AddProperty("Арбат", 200, 20);
        red.AddProperty("Тверская", 220, 22);

        monopoly::Player bogdan(1, "Богдан", 1000); // игроки создаются отдельно от участков
        monopoly::Player mark(2, "Марк", 1000);

        std::cout << "\nСостояние до" << std::endl;
        red.PrintInfo();
        bogdan.PrintInfo();

        std::cout << "\nАгрегация: игрок получает участок, созданный вне него (по указателю)" << std::endl;
        bogdan.BuyProperty(red.GetProperty(0));

        std::cout << "\nПравило: нельзя строить без полной монополии" << std::endl;
        bogdan.BuildOnProperty(red, 0);

        std::cout << "\nПокупка второго участка (по ссылке)" << std::endl;
        bogdan.BuyProperty(*red.GetProperty(1));

        std::cout << "\nКорректное действие: застройка через методы целого" << std::endl;
        bogdan.BuildOnProperty(red, 0);
        red.PrintInfo();

        std::cout << "\nПравило: нельзя купить занятый участок" << std::endl;
        mark.BuyProperty(red.GetProperty(0));

        std::cout << "\nПравило: нельзя заложить участок с постройкой" << std::endl;
        bogdan.MortgageProperty(*red.GetProperty(0));

        std::cout << "\nСначала продаём постройку, затем закладываем" << std::endl;
        bogdan.SellBuildingOnProperty(*red.GetProperty(0));
        bogdan.MortgageProperty(*red.GetProperty(0));

        std::cout << "\nСостояние после" << std::endl;
        red.PrintInfo();
        bogdan.PrintInfo();

        std::cout << "\nВыходим из блока: статические объекты уничтожаются в обратном порядке создания..."
            << std::endl;
    }

    // 2. Динамическая инициализация: new / delete
    {
        std::cout << "\n2. Динамические объекты (new / delete)" << std::endl;

        monopoly::Property* dacha = new monopoly::Property("Дача", 150, 15);
        monopoly::Player* leha = new monopoly::Player(3, "Лёха", 500);

        leha->BuyProperty(dacha);
        leha->PrintInfo();

        std::cout << "\ndelete игрока" << std::endl;
        delete leha;

        std::cout << "\nучасток после уничтожения игрока" << std::endl;
        dacha->PrintInfo();

        std::cout << "\ndelete участка" << std::endl;
        delete dacha;
    }

    // 3. Динамический массив объектов (используется конструктор по умолчанию)
    {
        std::cout << "\n3. Динамический массив объектов: new Player[3]" << std::endl;

        const int teamSize = 3;
        monopoly::Player* team = new monopoly::Player[teamSize];

        for (int i = 0; i < teamSize; ++i)
        {
            team[i].SetId(10 + i);
            team[i].SetName("Игрок" + std::to_string(i + 1));
            team[i].Receive(100 * (i + 1));
            team[i].PrintInfo();
        }

        std::cout << "\ndelete[]" << std::endl;
        delete[] team;
    }

    // 4. Массив динамических объектов
    {
        std::cout << "\n4. Массив динамических объектов: Player**" << std::endl;

        const int guestCount = 2;
        monopoly::Player** guests = new monopoly::Player * [guestCount];

        guests[0] = new monopoly::Player(21, "Гость А", 300);
        guests[1] = new monopoly::Player(22, "Гость Б", 350);

        for (int i = 0; i < guestCount; ++i)
        {
            guests[i]->PrintInfo();
        }

        std::cout << "\ndelete каждого элемента, затем delete[] массива указателей" << std::endl;
        for (int i = 0; i < guestCount; ++i)
        {
            delete guests[i];
        }
        delete[] guests;
    }

    // 5. Разница во времени жизни
    std::cout << "\n5а. Композиция: часть умирает вместе с целым" << std::endl;
    {
        monopoly::PropertyGroup blue("Синие", 2);
        blue.AddProperty("Сухэ-Батора", 300, 30);
        blue.AddProperty("50 лет СССР", 320, 32);
        blue.PrintInfo();

        std::cout << "Выходим из вложенного блока..." << std::endl;
    }
    std::cout << "Блок закончился: группы и её участков больше нет." << std::endl;

    std::cout << "\n5б. Агрегация: объект переживает агрегатор" << std::endl;
    monopoly::Property station("Рижский вокзал", 200, 25); // создан во внешнем блоке, раньше игрока
    {
        monopoly::Player guest(5, "Гость", 400);
        guest.BuyProperty(station);
        guest.PrintInfo();

        std::cout << "Выходим из вложенного блока..." << std::endl;
    }
    std::cout << "Игрок уничтожен, но участок жив и отвечает на вызовы методов:" << std::endl;
    station.PrintInfo();

    std::cout << "\nКонец main: уничтожается участок из внешнего блока." << std::endl;
    return 0;
}