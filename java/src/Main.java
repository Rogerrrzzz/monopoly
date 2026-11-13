import java.io.IOException;
import java.nio.file.Path;
import java.util.Arrays;

public class Main {
    public static void main(String[] args) {
        System.out.println("ЛР 5. Полиморфизм Java");

        // 1-3. Наследование базового класса и реализация интерфейса
        System.out.println("\n1. Интерфейс Rentable");
        Property street = new Property("Арбат", 200, 20);
        street.assignOwner(1);
        street.build();
        RailwayStation station = new RailwayStation("Рижский вокзал", 200, 25);
        station.assignOwner(1);
        RentCalculator calculator = new RentCalculator(new StandardRentPolicy());

        Rentable rentableStreet = street;
        Rentable rentableStation = station;
        System.out.println("Аренда участка через Rentable: " + calculator.calculate(rentableStreet));
        System.out.println("Аренда вокзала через Rentable: " + calculator.calculate(rentableStation));

        // 4. Статический метод выбирается заранее, describe - по фактическому типу объекта
        System.out.println("\n2. Раннее и позднее связывание");
        BaseProperty base = street;
        System.out.println("Раннее, static BaseProperty.getCreatedCount(): " + BaseProperty.getCreatedCount());
        System.out.println("Позднее, BaseProperty -> describe(): " + base.describe());

        // 5. Проверка поддержки интерфейса во время выполнения
        System.out.println("\n3. Проверка типа: instanceof");
        if (base instanceof Rentable rentable) {
            System.out.println(base.getName() + " поддерживает Rentable, аренда: " + calculator.calculate(rentable));
        } else {
            System.out.println(base.getName() + " не поддерживает Rentable");
        }

        // 6. Смена объекта поведения у существующего калькулятора
        System.out.println("\n4. Динамическое конфигурирование аренды");
        System.out.println("Обычная аренда: " + calculator.calculate(street));
        calculator.setPolicy(new DoubleRentPolicy());
        System.out.println("Двойная аренда: " + calculator.calculate(street));

        // Стандартный интерфейс Java: сортировка недвижимости по названию
        System.out.println("\n5. Comparable: порядок по названию");
        BaseProperty[] properties = {station, street};
        Arrays.sort(properties);
        for (BaseProperty property : properties) {
            System.out.println(property.getName());
        }

        // 7. Успешная покупка, нарушение правила и закрытие файла при любом исходе
        System.out.println("\n6. Исключения и try-with-resources");
        PropertyGroup red = new PropertyGroup("Красные", 2);
        red.addProperty("Арбат", 200, 20);
        red.addProperty("Тверская", 220, 22);
        Player bogdan = new Player(1, "Богдан", 1000);

        try (GameLog log = new GameLog(Path.of("game.log"))) {
            bogdan.buyProperty(red, 0);
            log.write("Игрок Богдан купил участок Арбат");
            log.write("Попытка построить на Арбате");
            bogdan.buildOnProperty(red, 0);
        } catch (GameRuleException error) {
            System.out.println("[Нарушение правила] " + error.getMessage() + ". Строительство отменено");
        } catch (IOException error) {
            System.out.println("[Ошибка журнала] " + error.getMessage());
        }
    }
}
