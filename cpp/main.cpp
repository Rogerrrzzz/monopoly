#include "base_property.hpp"
#include "double_rent_policy.hpp"
#include "money.hpp"
#include "percent.hpp"
#include "property.hpp"
#include "railway_station.hpp"
#include "rent_calculator.hpp"
#include "standard_rent_policy.hpp"

#include <iostream>
#include <memory>

#include <windows.h>

int main()
{
    SetConsoleOutputCP(1251);
    std::cout << "ЛР 6. Статический полиморфизм" << std::endl;

    monopoly::Property street("Арбат", 200, 20);
    street.AssignOwner(1);
    street.Build();
    monopoly::RailwayStation station("Рижский вокзал", 200, 25);
    station.AssignOwner(1);
    monopoly::RentCalculator<monopoly::Property> streetCalculator(
        std::make_unique<monopoly::StandardRentPolicy>());
    monopoly::RentCalculator<monopoly::RailwayStation> stationCalculator(
        std::make_unique<monopoly::StandardRentPolicy>());

    // 1. Одно имя метода для расчёта с обычным или разовым правилом
    std::cout << "\n1. Перегрузка Calculate" << std::endl;
    const monopoly::DoubleRentPolicy doublePolicy;
    std::cout << "Аренда Арбата по текущему правилу: " << streetCalculator.Calculate(street) << std::endl;
    std::cout << "Аренда Арбата по разовому правилу: "
              << streetCalculator.Calculate(street, doublePolicy) << std::endl;

    // 2. Сложение и сравнение денежных сумм
    std::cout << "\n2. Операторы класса Money" << std::endl;
    const monopoly::Money streetPrice{ street.GetPrice() };
    const monopoly::Money tverskayaPrice{ 220 };
    const monopoly::Money totalPrice = streetPrice + tverskayaPrice;
    std::cout << streetPrice << " + " << tverskayaPrice << " = " << totalPrice << std::endl;

    monopoly::Money purchaseCost{ streetPrice };
    purchaseCost += tverskayaPrice;
    std::cout << "Сумма после +=: " << purchaseCost << std::endl;
    std::cout << std::boolalpha;
    std::cout << "Суммы получились равными: " << (purchaseCost == totalPrice) << std::endl;
    std::cout << "Арбат дешевле Тверской: " << (streetPrice < tverskayaPrice) << std::endl;

    // 3. Дружественный вывод денег и вывод недвижимости через базовую ссылку
    std::cout << "\n3. Вывод через operator<<" << std::endl;
    std::cout << "Денежная сумма: " << totalPrice << std::endl;
    const monopoly::BaseProperty& streetBase = street;
    const monopoly::BaseProperty& stationBase = station;
    std::cout << streetBase << std::endl;
    std::cout << stationBase << std::endl;

    // 4. Шаблонная функция для целых и дробных чисел
    std::cout << "\n4. Шаблон Percent" << std::endl;
    std::cout << "Доля цены Арбата от бюджета 1000: "
              << monopoly::Percent(street.GetPrice(), 1000) << "%" << std::endl;
    std::cout << "Доля аренды вокзала от его цены: "
              << monopoly::Percent<double>(stationCalculator.Calculate(station), station.GetPrice()) << "%" << std::endl;
    // std::cout << monopoly::Percent("200", "1000") << std::endl; - здесь строки не удовлетворяют концепту Numeric

    // 5. Два типа калькулятора и общий расчёт аренды участка и вокзала
    std::cout << "\n5. Шаблонный RentCalculator" << std::endl;
    std::cout << "Аренда участка: " << streetCalculator.Calculate(street) << std::endl;
    std::cout << "Аренда вокзала: " << stationCalculator.Calculate(station) << std::endl;
    std::cout << "Общая аренда: " << streetCalculator.CalculateTotal(street, station) << std::endl;

    return 0;
}
