import java.io.IOException;
import java.lang.ref.WeakReference;
import java.nio.file.Path;

public class Main {
    public static void main(String[] args) {
        // 1. Композиция и исходный сценарий ЛР 2.
        System.out.println("\n1. Композиция: застройка и последующий залог");
        PropertyGroup red = new PropertyGroup("Красные",
                new String[]{"Арбат", "Тверская"}, new int[]{200, 220}, new int[]{20, 22});
        Player bogdan = new Player(1, "Богдан", 1000);
        Player mark = new Player(2, "Марк", 1000);

        System.out.println("\nСостояние до");
        System.out.println(red);
        System.out.println(bogdan);

        System.out.println("\nПокупка первого участка через методы группы");
        bogdan.buyProperty(red, 0);
        System.out.println("\nПравило: нельзя строить без полной монополии");
        bogdan.buildOnProperty(red, 0);

        System.out.println("\nПокупка второго участка");
        bogdan.buyProperty(red, 1);
        System.out.println("\nКорректное действие: застройка через методы целого");
        bogdan.buildOnProperty(red, 0);
        System.out.println(red);

        System.out.println("\nПравило: нельзя купить занятый участок");
        mark.buyProperty(red, 0);
        System.out.println("\nПравило: нельзя заложить участок с постройкой");
        bogdan.mortgageProperty(red, 0);

        System.out.println("\nСначала продаём постройку, затем закладываем");
        bogdan.sellBuildingOnProperty(red, 0);
        bogdan.mortgageProperty(red, 0);

        System.out.println("\nСостояние после");
        System.out.println(red);
        System.out.println(bogdan);

        // 2. Агрегация: участки созданы вне игроков.
        System.out.println("\n2. Агрегация: участок создан отдельно от игрока");
        Property dacha = new Property("Дача", 150, 15);
        Player leha = new Player(3, "Лёха", 500);
        leha.buyProperty(dacha);
        System.out.println(leha);
        System.out.println("Участок доступен отдельно от игрока по исходной ссылке:");
        System.out.println(dacha);

        Property station = new Property("Рижский вокзал", 200, 25);
        Player guest = new Player(5, "Гость", 400);
        guest.buyProperty(station);
        System.out.println(guest);
        System.out.println("Участок остаётся достижимым через внешнюю ссылку:");
        System.out.println(station);

        // 3. Массивы ссылок вместо массивов объектов и указателей C++.
        System.out.println("\n3. Массивы в Java содержат ссылки; объекты создаются отдельно");
        Player[] team = new Player[3];
        System.out.println("До создания игроков team[0] == null: " + (team[0] == null));
        for (int i = 0; i < team.length; i++) {
            team[i] = new Player(10 + i, "Игрок" + (i + 1), 100 * (i + 1));
            System.out.println(team[i]);
        }

        Player[] guests = {
            new Player(21, "Гость А", 300),
            new Player(22, "Гость Б", 350)
        };
        for (Player visitingPlayer : guests) {
            System.out.println(visitingPlayer);
        }
        System.out.println("После потери достижимости массивы и игроки могут быть собраны GC; delete[] не нужен.");

        // 4. Две переменные ссылаются на один объект.
        System.out.println("\n4. Присваивание копирует ссылку, а не объект");
        Player first = new Player(30, "Первый", 500);
        Player second = first;
        System.out.println("До изменения: " + first);
        second.pay(100);
        second.setName("Общее имя");
        System.out.println("После изменения через second:");
        System.out.println("first: " + first);
        System.out.println("second: " + second);
        System.out.println("first == second: " + (first == second));
        System.out.println("Независимая копия не создавалась: это один объект с двумя ссылками.");

        // 5. Гарантированное освобождение файлового ресурса.
        System.out.println("\n5. Освобождение ресурса: AutoCloseable и try-with-resources");
        try {
            GameLog log = new GameLog(Path.of("out", "game-session.log"));
            try (log) {
                log.write("Богдан купил Арбат, продал постройку и заложил участок.");
                System.out.println("Внутри блока журнал закрыт: " + log.isClosed());
            }
            System.out.println("После блока журнал закрыт: " + log.isClosed());
            System.out.println("Ссылка log ещё существует: close() освободил файл, а не память объекта.");
        } catch (IOException exception) {
            throw new IllegalStateException("Не удалось выполнить демонстрацию журнала", exception);
        }

        // 6. Сборщик мусора.
        System.out.println("\n6. Сборщик мусора");
        Property temp = new Property("Временный участок", 100, 10);
        WeakReference<Property> watcher = new WeakReference<>(temp);
        System.out.println("Объект жив: " + (watcher.get() != null));

        temp = null;
        System.gc();
        System.out.println("Объект жив: " + (watcher.get() != null));

        System.out.println("\nОтличие от ЛР 2: в C++ локальные объекты уничтожались при выходе из блока,"
                + " а созданные через new — при вызове delete.");
        System.out.println("В Java память недоступных объектов освобождает сборщик мусора;"
                + " момент освобождения заранее неизвестен, а System.gc() лишь запрашивает сборку.");

        System.out.println("\nКонец программы");
    }
}
