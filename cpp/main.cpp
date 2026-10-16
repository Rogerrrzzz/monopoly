#include "player.hpp"
#include "property.hpp"
#include "property_group.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include <windows.h>

int main()
{
    SetConsoleOutputCP(1251);

    // 1. Композиция: участки остаются объектами по значению внутри группы.
    {
        std::cout << "\n1. Застройка и последующий залог" << std::endl;

        std::shared_ptr<monopoly::PropertyGroup> red = std::make_shared<monopoly::PropertyGroup>("Красные", 2);
        red->AddProperty("Арбат", 200, 20);
        red->AddProperty("Тверская", 220, 22);

        monopoly::Player bogdan(1, "Богдан", 1000);
        monopoly::Player mark(2, "Марк", 1000);

        std::cout << "\nСостояние до" << std::endl;
        red->PrintInfo();
        bogdan.PrintInfo();

        std::cout << "\nПокупка первого участка: игрок наблюдает за группой через weak_ptr" << std::endl;
        bogdan.BuyProperty(red, 0);

        std::cout << "\nПравило: нельзя строить без полной монополии" << std::endl;
        bogdan.BuildOnProperty(*red, 0);

        std::cout << "\nПокупка второго участка" << std::endl;
        bogdan.BuyProperty(red, 1);

        std::cout << "\nКорректное действие: застройка через методы целого" << std::endl;
        bogdan.BuildOnProperty(*red, 0);
        red->PrintInfo();

        std::cout << "\nПравило: нельзя купить занятый участок" << std::endl;
        mark.BuyProperty(red, 0);

        std::cout << "\nПравило: нельзя заложить участок с постройкой" << std::endl;
        bogdan.MortgageProperty(*red, 0);

        std::cout << "\nСначала продаём постройку, затем закладываем" << std::endl;
        bogdan.SellBuildingOnProperty(*red, 0);
        bogdan.MortgageProperty(*red, 0);

        std::cout << "\nСостояние после" << std::endl;
        red->PrintInfo();
        bogdan.PrintInfo();

        std::cout << "\nВыход из блока: уничтожаются игроки, затем группа и её участки." << std::endl;
    }

    // 2. Единоличное владение игроком и передача владения.
    {
        std::cout << "\n2. unique_ptr: передача владения и уничтожение при выходе из блока" << std::endl;

        std::shared_ptr<monopoly::Property> dacha = std::make_shared<monopoly::Property>("Дача", 150, 15);
        {
            std::unique_ptr<monopoly::Player> leha = std::make_unique<monopoly::Player>(3, "Лёха", 500);
            leha->BuyProperty(dacha);
            leha->PrintInfo();

            std::unique_ptr<monopoly::Player> owner = std::move(leha);
            std::cout << "После std::move исходный указатель пуст: " << (leha ? "нет" : "да") << std::endl;
            owner->PrintInfo();
            std::cout << "Выход из внутреннего блока: единственный владелец уничтожает игрока." << std::endl;
        }

        std::cout << "\nИгрок уничтожен, участок остаётся у своего владельца:" << std::endl;
        dacha->PrintInfo();
        std::cout << "Выход из внешнего блока: уничтожается Дача." << std::endl;
    }

    // 3. Массив объектов: один владелец всего массива.
    {
        std::cout << "\n3. Массив объектов: unique_ptr<Player[]>" << std::endl;
        const int teamSize = 3;
        std::unique_ptr<monopoly::Player[]> team = std::make_unique<monopoly::Player[]>(teamSize);

        for (int i = 0; i < teamSize; ++i)
        {
            team[i].SetId(10 + i);
            team[i].SetName("Игрок" + std::to_string(i + 1));
            team[i].Receive(100 * (i + 1));
            team[i].PrintInfo();
        }

        std::cout << "Выход из блока: автоматически освобождается весь массив." << std::endl;
    }

    // 4. Массив отдельных владельцев динамических объектов.
    {
        std::cout << "\n4. Массив unique_ptr<Player>" << std::endl;
        const int guestCount = 2;
        std::unique_ptr<monopoly::Player> guests[guestCount];

        guests[0] = std::make_unique<monopoly::Player>(21, "Гость А", 300);
        guests[1] = std::make_unique<monopoly::Player>(22, "Гость Б", 350);

        for (int i = 0; i < guestCount; ++i)
        {
            guests[i]->PrintInfo();
        }

        std::cout << "Выход из блока: каждый unique_ptr освобождает своего игрока." << std::endl;
    }

    // 5. Композиция и агрегация сохраняют смысл из ЛР 2.
    std::cout << "\n5а. Композиция: части уничтожаются вместе с целым" << std::endl;
    {
        monopoly::PropertyGroup blue("Синие", 2);
        blue.AddProperty("Сухэ-Батора", 300, 30);
        blue.AddProperty("50 лет СССР", 320, 32);
        blue.PrintInfo();
        std::cout << "Выход из блока: уничтожается группа, затем её части." << std::endl;
    }

    std::cout << "\n5б. Агрегация: участок переживает игрока" << std::endl;
    std::shared_ptr<monopoly::Property> station = std::make_shared<monopoly::Property>("Рижский вокзал", 200, 25);
    {
        monopoly::Player guest(5, "Гость", 400);
        guest.BuyProperty(station);
        guest.PrintInfo();
        std::cout << "Число владельцев участка: " << station.use_count()
            << " (weak_ptr игрока счётчик не увеличивает)" << std::endl;
    }
    std::cout << "Игрок уничтожен, участок жив:" << std::endl;
    station->PrintInfo();

    // 6. Несколько владельцев и слабый наблюдатель.
    {
        std::cout << "\n6. shared_ptr и weak_ptr" << std::endl;
        std::shared_ptr<monopoly::Property> first = std::make_shared<monopoly::Property>("Общий участок", 100, 10);
        std::weak_ptr<monopoly::Property> watcher = first;
        monopoly::Player observer(6, "Наблюдатель", 500);
        observer.BuyProperty(first);

        std::cout << "Один владелец: " << first.use_count() << std::endl;
        {
            std::shared_ptr<monopoly::Property> second = first;
            std::cout << "После копирования shared_ptr: " << first.use_count() << std::endl;
        }
        std::cout << "После уничтожения копии: " << first.use_count() << std::endl;

        if (std::shared_ptr<monopoly::Property> property = watcher.lock())
        {
            std::cout << "lock(): участок жив, имя: " << property->GetName() << std::endl;
        }

        std::cout << "Последний владелец освобождает участок:" << std::endl;
        first.reset();

        if (std::shared_ptr<monopoly::Property> property = watcher.lock())
        {
            property->PrintInfo();
        }
        else
        {
            std::cout << "lock() вернул пустой указатель: участок уже уничтожен." << std::endl;
        }
        observer.PrintInfo();
    }

    std::cout << "\nКонец main: уничтожается Рижский вокзал, оставшийся у своего владельца." << std::endl;
    return 0;
}
