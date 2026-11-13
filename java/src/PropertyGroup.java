import java.util.ArrayList;
import java.util.List;

/** Группа сама создаёт свои участки и не выдаёт ссылки на них (композиция). */
public class PropertyGroup {
    private final String name;
    private final int capacity;
    private final List<Property> properties;

    public PropertyGroup(String name, int capacity) {
        if (capacity < 0) {
            throw new GameRuleException("Вместимость группы не может быть отрицательной");
        }
        this.name = name;
        this.capacity = capacity;
        properties = new ArrayList<>(capacity);
        System.out.println("[PropertyGroup] Создана группа \"" + name + "\", вместимость " + capacity);
    }

    /** Передаются только данные; сами части создаются внутри конструктора группы. */
    public PropertyGroup(String name, String[] names, int[] prices, int[] baseRents) {
        this(name, names.length);
        if (prices.length != names.length || baseRents.length != names.length) {
            throw new GameRuleException("Для каждого участка нужны название, цена и аренда");
        }
        for (int i = 0; i < names.length; i++) {
            properties.add(new Property(names[i], prices[i], baseRents[i]));
        }
    }

    public String getName() {
        return name;
    }

    public int getCount() {
        return properties.size();
    }

    public int getCapacity() {
        return capacity;
    }

    public void addProperty(String name, int price, int baseRent) {
        if (properties.size() >= capacity) {
            throw new GameRuleException("Группа " + this.name + " уже заполнена");
        }
        properties.add(new Property(name, price, baseRent));
    }

    // Метод доступа к части закрыт: даже неизменяемый список ссылок нарушил бы композицию.
    private Property getProperty(int index) {
        if (index < 0 || index >= properties.size()) {
            throw new GameRuleException("В группе " + name + " нет участка с номером " + index);
        }
        return properties.get(index);
    }

    public String getPropertyInfo(int index) {
        return getProperty(index).toString();
    }

    public int getOwnedPropertyCount(int playerId) {
        int count = 0;
        for (Property property : properties) {
            if (property.isOwned() && property.getOwnerId() == playerId) {
                count++;
            }
        }
        return count;
    }

    public boolean isFullyOwnedBy(int playerId) {
        return playerId > 0 && capacity > 0 && properties.size() == capacity
                && getOwnedPropertyCount(playerId) == capacity;
    }

    void buyProperty(int index, Player player) {
        Property property = getProperty(index);
        if (player.getId() <= 0) {
            throw new GameRuleException("Перед покупкой задайте положительный id игрока");
        }
        if (property.isOwned()) {
            throw new GameRuleException("Участок " + property.getName()
                    + " уже принадлежит игроку " + property.getOwnerId());
        }
        player.pay(property.getPrice());
        property.assignOwner(player.getId());
        System.out.println("Игрок " + player.getName() + " купил участок " + property.getName()
                + " за " + property.getPrice());
    }

    public void buildOn(int index, Player player) {
        Property property = getProperty(index);
        checkOwner(property, player);
        if (!isFullyOwnedBy(player.getId())) {
            throw new GameRuleException("Для строительства нужно владеть всей группой " + name);
        }
        int cost = property.getBuildCost();
        if (player.getBalance() < cost) {
            throw new GameRuleException("Недостаточно денег на постройку: нужно " + cost
                    + ", есть " + player.getBalance());
        }
        property.build();
        player.pay(cost);
        System.out.println("Игрок " + player.getName() + " построил на участке " + property.getName()
                + " за " + cost);
    }

    void mortgageProperty(int index, Player player) {
        player.mortgageProperty(getProperty(index));
    }

    void sellBuildingOnProperty(int index, Player player) {
        player.sellBuildingOnProperty(getProperty(index));
    }

    private void checkOwner(Property property, Player player) {
        if (property.getOwnerId() != player.getId()) {
            throw new GameRuleException("Участок не принадлежит игроку " + player.getName());
        }
    }

    String describeOwnedProperties(int playerId) {
        StringBuilder info = new StringBuilder();
        for (Property property : properties) {
            if (property.isOwned() && property.getOwnerId() == playerId) {
                info.append(System.lineSeparator()).append("  - ").append(property);
            }
        }
        return info.toString();
    }

    @Override
    public String toString() {
        StringBuilder info = new StringBuilder("Группа \"" + name + "\" ("
                + properties.size() + "/" + capacity + "):");
        for (Property property : properties) {
            info.append(System.lineSeparator()).append("  ").append(property);
        }
        return info.toString();
    }
}
