// Участок с возможностью строительства
public class Property extends BaseProperty {
    private static final int MAX_BUILD_LEVEL = 4;
    private static final int[] RENT_MULTIPLIERS = {1, 2, 3, 5, 8};

    private int buildLevel;

    public Property() {
        super();
        System.out.println("[Property] Конструктор по умолчанию");
    }

    public Property(String name, int price, int baseRent) {
        super(name, price, baseRent);
        System.out.println("[Property] Конструктор с параметрами: " + getName());
    }

    public Property(Property other) {
        super(other);
        this.buildLevel = other.buildLevel;
        System.out.println("[Property] Конструктор копирования: " + getName());
    }

    public int getBuildLevel() {
        return buildLevel;
    }

    public int getBuildCost() {
        return getPrice() / 2;
    }

    public int getBuildRefund() {
        return getBuildCost() / 2;
    }

    @Override
    public int calcRent() {
        return !isOwned() || isMortgaged() ? 0 : getBaseRent() * RENT_MULTIPLIERS[buildLevel];
    }

    @Override
    public String describe() {
        return "Участок " + super.describe() + ", уровень застройки " + buildLevel;
    }

    @Override
    public Property shallowCopy() {
        Property copy = new Property(getName(), getPrice(), getBaseRent());
        copySharedStateTo(copy);
        copy.buildLevel = buildLevel;
        return copy;
    }

    @Override
    public Property deepCopy() {
        return new Property(this);
    }

    public boolean build() {
        if (isMortgaged() || buildLevel >= MAX_BUILD_LEVEL) {
            System.out.println("[Отказ] Участок заложен или достигнут предел застройки: " + getName());
            return false;
        }
        buildLevel++;
        return true;
    }

    public boolean sellBuilding() {
        if (buildLevel == 0) {
            System.out.println("[Отказ] Нет построек: " + getName());
            return false;
        }
        buildLevel--;
        return true;
    }

    @Override
    public boolean mortgage() {
        if (buildLevel > 0) {
            System.out.println("[Отказ] Нельзя заложить участок с постройкой: " + getName());
            return false;
        }
        return super.mortgage();
    }

    @Override
    public void returnToBank() {
        super.returnToBank();
        buildLevel = 0;
    }

    @Override
    public boolean equals(Object object) {
        if (!super.equals(object)) {
            return false;
        }
        Property other = (Property) object;
        return buildLevel == other.buildLevel;
    }

    @Override
    public int hashCode() {
        return 31 * super.hashCode() + buildLevel;
    }
}
