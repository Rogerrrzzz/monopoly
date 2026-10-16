import java.util.ArrayList;
import java.util.List;

/** Группа сама создаёт свои участки и не выдаёт ссылки на них (композиция). */
public class PropertyGroup {
    private final String name;
    private final int capacity;
    private final List<Property> properties;

    public PropertyGroup(String name, int capacity) {
        if (capacity < 0) {
            throw new IllegalArgumentException("Вместимость группы не может быть отрицательной");
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
            throw new IllegalArgumentException("Для каждого участка нужны название, цена и аренда");
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

    public boolean addProperty(String name, int price, int baseRent) {
        if (properties.size() >= capacity) {
            System.out.println("[Отказ] Группа \"" + this.name + "\" уже заполнена, участок \""
                    + name + "\" не добавлен");
            return false;
        }
        properties.add(new Property(name, price, baseRent));
        return true;
    }

    // Метод доступа к части закрыт: даже неизменяемый список ссылок нарушил бы композицию.
    private Property getProperty(int index) {
        if (index < 0 || index >= properties.size()) {
            System.out.println("[Отказ] В группе \"" + name + "\" нет участка с номером " + index);
            return null;
        }
        return properties.get(index);
    }

    public String getPropertyInfo(int index) {
        Property property = getProperty(index);
        return property == null ? "Участок не найден" : property.toString();
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

    boolean buyProperty(int index, Player player) {
        Property property = getProperty(index);
        if (property == null) {
            return false;
        }
        if (player.getId() <= 0) {
            System.out.println("[Отказ] Перед покупкой задайте положительный id игрока");
            return false;
        }
        if (property.isOwned()) {
            System.out.println("[Отказ] Участок \"" + property.getName()
                    + "\" уже принадлежит игроку " + property.getOwnerId());
            return false;
        }
        if (!player.pay(property.getPrice())) {
            return false;
        }
        property.assignOwner(player.getId());
        System.out.println("Игрок \"" + player.getName() + "\" купил участок \""
                + property.getName() + "\" за " + property.getPrice());
        return true;
    }

    public boolean buildOn(int index, Player player) {
        Property property = getProperty(index);
        if (property == null) {
            return false;
        }
        int cost = property.getBuildCost();
        if (player.getBalance() < cost) {
            System.out.println("[Отказ] У игрока \"" + player.getName()
                    + "\" не хватает денег на постройку: нужно " + cost + ", есть " + player.getBalance());
            return false;
        }
        if (!belongsTo(property, player)) {
            return false;
        }
        if (!isFullyOwnedBy(player.getId())) {
            System.out.println("[Отказ] Игрок " + player.getId() + " не владеет всей группой \""
                    + name + "\", застройка запрещена");
            return false;
        }
        if (!property.build()) {
            return false;
        }
        player.pay(cost);
        System.out.println("Игрок \"" + player.getName() + "\" построил на участке \""
                + property.getName() + "\" за " + cost);
        return true;
    }

    boolean mortgageProperty(int index, Player player) {
        Property property = getProperty(index);
        if (property == null || !belongsTo(property, player)) {
            return false;
        }
        if (property.getMortgageValue() > Integer.MAX_VALUE - player.getBalance()) {
            return false;
        }
        if (!property.mortgage()) {
            return false;
        }
        player.receive(property.getMortgageValue());
        System.out.println("Игрок \"" + player.getName() + "\" заложил участок \""
                + property.getName() + "\", получено " + property.getMortgageValue());
        return true;
    }

    boolean sellBuildingOnProperty(int index, Player player) {
        Property property = getProperty(index);
        if (property == null || !belongsTo(property, player)) {
            return false;
        }
        if (property.getBuildRefund() > Integer.MAX_VALUE - player.getBalance()) {
            return false;
        }
        if (!property.sellBuilding()) {
            return false;
        }
        player.receive(property.getBuildRefund());
        System.out.println("Игрок \"" + player.getName() + "\" продал постройку на участке \""
                + property.getName() + "\", получено " + property.getBuildRefund());
        return true;
    }

    private boolean belongsTo(Property property, Player player) {
        if (property.getOwnerId() != player.getId()) {
            System.out.println("[Отказ] Участок \"" + property.getName()
                    + "\" не принадлежит игроку \"" + player.getName() + "\"");
            return false;
        }
        return true;
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
