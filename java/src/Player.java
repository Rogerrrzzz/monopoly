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
            throw new IllegalArgumentException("Идентификатор и баланс не могут быть отрицательными");
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
            throw new IllegalArgumentException("Положительный id задаётся до покупки участков");
        }
        this.id = id;
    }

    public void setName(String name) {
        this.name = name;
    }

    public boolean pay(int amount) {
        if (amount < 0) {
            System.out.println("[Отказ] Нельзя заплатить отрицательную сумму");
            return false;
        }
        if (amount > balance) {
            System.out.println("[Отказ] У игрока \"" + name + "\" недостаточно денег: нужно "
                    + amount + ", есть " + balance);
            return false;
        }
        balance -= amount;
        return true;
    }

    public boolean receive(int amount) {
        if (amount < 0 || amount > Integer.MAX_VALUE - balance) {
            System.out.println("[Отказ] Недопустимая сумма пополнения баланса");
            return false;
        }
        balance += amount;
        return true;
    }

    public boolean buyProperty(BaseProperty property) {
        if (property == null) {
            System.out.println("[Отказ] Участок не указан (null)");
            return false;
        }
        if (id <= 0) {
            System.out.println("[Отказ] Перед покупкой задайте положительный id игрока");
            return false;
        }
        if (property.isOwned()) {
            System.out.println("[Отказ] Участок \"" + property.getName()
                    + "\" уже принадлежит игроку " + property.getOwnerId());
            return false;
        }
        if (!pay(property.getPrice())) {
            return false;
        }
        property.assignOwner(id);
        // Разные объекты с одинаковыми данными всё равно являются разной недвижимостью
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
        System.out.println("Игрок \"" + name + "\" купил недвижимость \"" + property.getName()
                + "\" за " + property.getPrice());
        return true;
    }

    public boolean buyProperty(PropertyGroup group, int index) {
        if (!group.buyProperty(index, this)) {
            return false;
        }
        // Сохраняется группа целиком: ссылок на её внутренние участки снаружи нет.
        if (!propertyGroups.contains(group)) {
            propertyGroups.add(group);
        }
        return true;
    }

    public boolean mortgageProperty(BaseProperty property) {
        if (property.getOwnerId() != id) {
            System.out.println("[Отказ] Участок \"" + property.getName()
                    + "\" не принадлежит игроку \"" + name + "\"");
            return false;
        }
        if (property.getMortgageValue() > Integer.MAX_VALUE - balance) {
            return false;
        }
        if (!property.mortgage()) {
            return false;
        }
        receive(property.getMortgageValue());
        System.out.println("Игрок \"" + name + "\" заложил участок \"" + property.getName()
                + "\", получено " + property.getMortgageValue());
        return true;
    }

    public boolean mortgageProperty(PropertyGroup group, int index) {
        return group.mortgageProperty(index, this);
    }

    public boolean buildOnProperty(PropertyGroup group, int index) {
        return group.buildOn(index, this);
    }

    public boolean sellBuildingOnProperty(Property property) {
        if (property.getOwnerId() != id) {
            System.out.println("[Отказ] Участок \"" + property.getName()
                    + "\" не принадлежит игроку \"" + name + "\"");
            return false;
        }
        if (property.getBuildRefund() > Integer.MAX_VALUE - balance) {
            return false;
        }
        if (!property.sellBuilding()) {
            return false;
        }
        receive(property.getBuildRefund());
        System.out.println("Игрок \"" + name + "\" продал постройку на участке \""
                + property.getName() + "\", получено " + property.getBuildRefund());
        return true;
    }

    public boolean sellBuildingOnProperty(PropertyGroup group, int index) {
        return group.sellBuildingOnProperty(index, this);
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
