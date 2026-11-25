// Общее состояние и поведение недвижимости
public abstract class BaseProperty implements Comparable<BaseProperty> {
    private static final int BANK_OWNER_ID = -1;
    private static int createdCount;

    private PropertyInfo info;
    private int ownerId = BANK_OWNER_ID;
    private boolean mortgaged;

    protected BaseProperty() {
        this("Без названия", 0, 0);
    }

    protected BaseProperty(String name, int price, int baseRent) {
        this.info = new PropertyInfo(name, price, baseRent);
        createdCount++;
        System.out.println("[BaseProperty] Конструктор с параметрами: " + info.getName());
    }

    // Для глубокой копии создаётся отдельная карточка
    protected BaseProperty(BaseProperty other) {
        this.info = new PropertyInfo(other.info);
        this.ownerId = other.ownerId;
        this.mortgaged = other.mortgaged;
        createdCount++;
        System.out.println("[BaseProperty] Конструктор копирования: " + info.getName());
    }

    protected String header() {
        String owner = isOwned() ? "игрок " + ownerId : "банк";
        return "\"" + getName() + "\": цена " + getPrice() + ", владелец - " + owner
                + (mortgaged ? ", в залоге" : "");
    }

    // Наследники используют этот метод только для поверхностной копии
    protected void copySharedStateTo(BaseProperty target) {
        target.info = info;
        target.ownerId = ownerId;
        target.mortgaged = mortgaged;
    }

    public String getName() {
        return info.getName();
    }

    public int getPrice() {
        return info.getPrice();
    }

    public int getBaseRent() {
        return info.getBaseRent();
    }

    public int getOwnerId() {
        return ownerId;
    }

    public boolean isOwned() {
        return ownerId != BANK_OWNER_ID;
    }

    public boolean isMortgaged() {
        return mortgaged;
    }

    public int getMortgageValue() {
        return getPrice() / 2;
    }

    public int getRedeemCost() {
        return (int) (getMortgageValue() * 11L / 10);
    }

    public void rename(String name) {
        info.setName(name);
    }

    public void setBaseRent(int baseRent) {
        info.setBaseRent(baseRent);
    }

    public void assignOwner(int playerId) {
        if (playerId <= 0) {
            throw new GameRuleException("Идентификатор владельца должен быть положительным");
        }
        if (isOwned()) {
            throw new GameRuleException("Недвижимость уже принадлежит игроку " + ownerId);
        }
        ownerId = playerId;
    }

    public void mortgage() {
        if (mortgaged) {
            throw new GameRuleException("Недвижимость уже находится в залоге");
        }
        mortgaged = true;
    }

    public void redeem() {
        if (!mortgaged) {
            throw new GameRuleException("Недвижимость не находится в залоге");
        }
        mortgaged = false;
    }

    public void returnToBank() {
        ownerId = BANK_OWNER_ID;
        mortgaged = false;
    }

    public abstract int calcRent();

    public String describe() {
        return header() + ", аренда " + calcRent();
    }

    public abstract BaseProperty shallowCopy();

    public abstract BaseProperty deepCopy();

    public void showTo(Player player) {
        player.reportProperty(this);
    }

    public static int getCreatedCount() {
        return createdCount;
    }

    // Естественный порядок недвижимости - по названию
    @Override
    public int compareTo(BaseProperty other) {
        return getName().compareTo(other.getName());
    }

    @Override
    public String toString() {
        return describe();
    }

    @Override
    public boolean equals(Object object) {
        if (this == object) {
            return true;
        }
        if (object == null || getClass() != object.getClass()) {
            return false;
        }
        BaseProperty other = (BaseProperty) object;
        return ownerId == other.ownerId && mortgaged == other.mortgaged && info.equals(other.info);
    }

    @Override
    public int hashCode() {
        int result = info.hashCode();
        result = 31 * result + ownerId;
        result = 31 * result + Boolean.hashCode(mortgaged);
        return result;
    }
}
