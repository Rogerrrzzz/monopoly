public class Main {
    public static void main(String[] args) {
        System.out.println("ЛР 6. Статический полиморфизм Java");

        Property street = new Property("Арбат", 200, 20);
        street.assignOwner(1);
        street.build();
        RailwayStation station = new RailwayStation("Рижский вокзал", 200, 25);
        station.assignOwner(1);
        RentCalculator<Property> streetCalculator = new RentCalculator<>(new StandardRentPolicy());
        RentCalculator<RailwayStation> stationCalculator = new RentCalculator<>(new StandardRentPolicy());

        // 1. Перегрузка методов и доступ в пределах пакета
        System.out.println("\n1. Перегрузка calculate и доступ из того же пакета");
        System.out.println("Аренда Арбата по текущему правилу: " + streetCalculator.calculate(street));
        System.out.println("Аренда Арбата по разовому правилу: " + streetCalculator.calculate(street, new DoubleRentPolicy()));

        // 2. Методы вместо арифметических операторов
        System.out.println("\n2. Денежные операции plus и times");
        Money streetPrice = new Money(street.getPrice());
        Money tverskayaPrice = new Money(220);
        System.out.println(streetPrice + " + " + tverskayaPrice + " = " + streetPrice.plus(tverskayaPrice));
        Money streetRent = new Money(streetCalculator.calculate(street));
        System.out.println("Два платежа по аренде Арбата: " + streetRent.times(2));

        // 3. Обобщённый метод с ограничением Number
        System.out.println("\n3. Обобщённый метод percent");
        System.out.println("Доля цены Арбата от бюджета 1000: " + percent(street.getPrice(), 1000) + "%");
        System.out.println("Доля аренды вокзала от его цены: " + percent((stationCalculator.calculate(station)), station.getPrice()) + "%");

        // 4. Калькуляторы для двух типов недвижимости и обобщённый метод внутри класса
        System.out.println("\n4. Обобщённый класс RentCalculator");
        System.out.println("Аренда участка: " + streetCalculator.calculate(street));
        System.out.println("Аренда вокзала: " + stationCalculator.calculate(station));
        System.out.println("Общая аренда: " + streetCalculator.calculateTotal(street, station));
    }

    private static <T extends Number> double percent(T part, T whole) {
        double total = whole.doubleValue();
        if (total == 0.0) {
            throw new IllegalArgumentException("Нельзя вычислить процент от нуля");
        }
        return (part.doubleValue() / total) * 100.0;
    }
}
