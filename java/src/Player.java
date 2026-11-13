import java.util.ArrayList;
import java.util.List;

/** Игрок хранит ссылки на созданные снаружи участки и группы (агрегация). */
public class Player {
    private int id;
    private String name;
    private int balance;
    private final List<BaseProperty> properties = new ArrayList<>();
    private final List<PropertyGroup> propertyGroups = new ArrayList<>();

    public Player(int id, String name, int balance) {
        if (id < 0 || balance < 0) {
            throw new GameRuleException("Идентификатор и баланс не могут быть отрицательными");
        }
        this.id = id;
        this.name = name;
        this.balance = balance;
        System.out.println("[Player] Создан игрок \"" + name + "\" (id " + id + "), баланс " + balance);
    }

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public int getBalance() {
        return balance;
    }

    public int getPropertyCount() {
        int count = 0;
        for (BaseProperty property : properties) {
            if (property.getOwnerId() == id) {
                count++;
            }
        }
        for (PropertyGroup group : propertyGroups) {
            count += group.getOwnedPropertyCount(id);
        }
        return count;
    }

    public void setId(int id) {
        if (id <= 0 || getPropertyCount() > 0) {
            throw new GameRuleException("Положительный id задаётся до покупки участков");
        }
        this.id = id;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void pay(int amount) {
        if (amount < 0) {
            throw new GameRuleException("Нельзя заплатить отрицательную сумму");
        }
        if (amount > balance) {
            throw new GameRuleException("У игрока " + name + " недостаточно денег: нужно "
                    + amount + ", есть " + balance);
        }
        balance -= amount;
    }

    public void receive(int amount) {
        if (amount < 0 || amount > Integer.MAX_VALUE - balance) {
            throw new GameRuleException("Недопустимая сумма пополнения баланса");
        }
        balance += amount;
    }

    public void buyProperty(BaseProperty property) {
        if (property == null) {
            throw new GameRuleException("Недвижимость не указана");
        }
        if (id <= 0) {
            throw new GameRuleException("Перед покупкой задайте положительный id игрока");
        }
        if (property.isOwned()) {
            throw new GameRuleException("Недвижимость " + property.getName()
                    + " уже принадлежит игроку " + property.getOwnerId());
        }
        pay(property.getPrice());
        property.assignOwner(id);
        // Учитываем именно объект недвижимости, а не равенство его данных
        boolean alreadyStored = false;
        for (BaseProperty stored : properties) {
            if (stored == property) {
                alreadyStored = true;
                break;
            }
        }
        if (!alreadyStored) {
            properties.add(property);
        }
        System.out.println("Игрок " + name + " купил недвижимость " + property.getName()
                + " за " + property.getPrice());
    }

    public void buyProperty(PropertyGroup group, int index) {
        if (group == null) {
            throw new GameRuleException("Группа не указана");
        }
        group.buyProperty(index, this);
        // Сохраняется группа целиком: ссылок на её внутренние участки снаружи нет
        if (!propertyGroups.contains(group)) {
            propertyGroups.add(group);
        }
    }

    public void mortgageProperty(BaseProperty property) {
        if (property.getOwnerId() != id) {
            throw new GameRuleException("Недвижимость не принадлежит игроку " + name);
        }
        int amount = property.getMortgageValue();
        if (amount > Integer.MAX_VALUE - balance) {
            throw new GameRuleException("Залог приведёт к переполнению баланса");
        }
        property.mortgage();
        receive(amount);
        System.out.println("Игрок " + name + " заложил недвижимость " + property.getName()
                + ", получено " + amount);
    }

    public void mortgageProperty(PropertyGroup group, int index) {
        group.mortgageProperty(index, this);
    }

    public void buildOnProperty(PropertyGroup group, int index) {
        group.buildOn(index, this);
    }

    public void sellBuildingOnProperty(Property property) {
        if (property.getOwnerId() != id) {
            throw new GameRuleException("Участок не принадлежит игроку " + name);
        }
        int amount = property.getBuildRefund();
        if (amount > Integer.MAX_VALUE - balance) {
            throw new GameRuleException("Продажа постройки приведёт к переполнению баланса");
        }
        property.sellBuilding();
        receive(amount);
        System.out.println("Игрок " + name + " продал постройку на участке " + property.getName()
                + ", получено " + amount);
    }

    public void sellBuildingOnProperty(PropertyGroup group, int index) {
        group.sellBuildingOnProperty(index, this);
    }

    public void reportProperty(BaseProperty property) {
        System.out.println("Игрок \"" + name + "\" осматривает: " + property.describe());
    }

    @Override
    public String toString() {
        StringBuilder info = new StringBuilder("Игрок \"" + name + "\" (id " + id
                + "): баланс " + balance + ", участков " + getPropertyCount());
        for (BaseProperty property : properties) {
            if (property.getOwnerId() == id) {
                info.append(System.lineSeparator()).append("  - ").append(property);
            }
        }
        for (PropertyGroup group : propertyGroups) {
            info.append(group.describeOwnedProperties(id));
        }
        return info.toString();
    }
}
