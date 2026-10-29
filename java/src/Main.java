public class Main {
    public static void main(String[] args) {
        System.out.println("Создано объектов недвижимости: " + BaseProperty.getCreatedCount());

        // 1. Абстрактный класс и два наследника
        System.out.println("\n1. Наследование и полиморфизм");
        Property street = new Property("Арбат", 200, 20);
        street.assignOwner(1);
        street.build();
        BaseProperty[] properties = {
            street,
            new RailwayStation("Рижский вокзал", 200, 25)
        };
        properties[1].assignOwner(1);
        for (BaseProperty property : properties) {
            System.out.println(property);
            System.out.println("Аренда через BaseProperty: " + property.calcRent());
        }

        // 2. Конструкторы базы и наследников
        System.out.println("\n2. Конструкторы");
        Property empty = new Property();
        Property original = new Property("Тверская", 220, 22);
        Property constructorCopy = new Property(original);
        System.out.println(empty);
        System.out.println(constructorCopy);
        RailwayStation emptyStation = new RailwayStation();
        RailwayStation station = new RailwayStation("Казанский вокзал", 200, 25);
        RailwayStation stationCopy = new RailwayStation(station);
        System.out.println(emptyStation);
        System.out.println(stationCopy);
        System.out.println("Сначала выполняется конструктор BaseProperty, затем конструктор наследника.");

        // 3. Общая и независимая карточки недвижимости
        System.out.println("\n3. Поверхностная и глубокая копии");
        Property copySource = new Property("Арбат", 200, 20);
        copySource.assignOwner(1);
        BaseProperty shallow = copySource.shallowCopy();
        shallow.rename("Арбат: общая карточка");
        shallow.setBaseRent(30);
        System.out.println("После изменения поверхностной копии изменился и оригинал:");
        System.out.println(copySource);
        System.out.println(shallow);

        BaseProperty deep = copySource.deepCopy();
        deep.rename("Арбат: независимая карточка");
        deep.setBaseRent(90);
        System.out.println("После изменения глубокой копии оригинал сохраняет имя и аренду 30:");
        System.out.println(copySource);
        System.out.println(deep);

        BaseProperty railway = new RailwayStation("Ленинградский вокзал", 200, 25);
        BaseProperty railwayCopy = railway.deepCopy();
        System.out.println("Глубокая копия через BaseProperty сохраняет тип вокзала:");
        System.out.println(railwayCopy);

        // 4. Ссылка на тот же объект и равенство разных объектов по содержимому
        System.out.println("\n4. equals и hashCode");
        Property first = new Property("Арбат", 200, 20);
        Property second = first.deepCopy();
        System.out.println("first == second: " + (first == second));
        System.out.println("first.equals(second): " + first.equals(second));
        System.out.println("Одинаковые hashCode: " + (first.hashCode() == second.hashCode()));
        second.setBaseRent(35);
        System.out.println("После изменения копии equals: " + first.equals(second));
        Property alias = first;
        System.out.println("После alias = first, alias == first: " + (alias == first));

        // 5. this и общий счётчик
        System.out.println("\n5. this и статические члены");
        Player viewer = new Player(7, "Покупатель", 500);
        station.showTo(viewer);
        System.out.println("Всего создано объектов, включая копии: " + BaseProperty.getCreatedCount());

        // 6. Исходные игровые правила и покупка разных видов недвижимости
        System.out.println("\n6. Композиция, агрегация и правила игры");
        PropertyGroup red = new PropertyGroup("Красные",
                new String[]{"Арбат", "Тверская"}, new int[]{200, 220}, new int[]{20, 22});
        Player bogdan = new Player(1, "Богдан", 1000);
        Player mark = new Player(2, "Марк", 1000);
        bogdan.buyProperty(red, 0);
        bogdan.buildOnProperty(red, 0);
        bogdan.buyProperty(red, 1);
        bogdan.buildOnProperty(red, 0);
        mark.buyProperty(red, 0);
        bogdan.mortgageProperty(red, 0);
        bogdan.sellBuildingOnProperty(red, 0);
        bogdan.mortgageProperty(red, 0);
        System.out.println(bogdan);
        RailwayStation purchasedStation = new RailwayStation("Рижский вокзал", 200, 25);
        mark.buyProperty(purchasedStation);
        System.out.println(mark);

        System.out.println("\nИтоговый счётчик созданных объектов: " + BaseProperty.getCreatedCount());
    }
}
