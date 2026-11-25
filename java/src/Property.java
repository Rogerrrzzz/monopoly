// Участок с возможностью строительства
public class Property extends BaseProperty implements Rentable {
    private static final int MAX_BUILD_LEVEL = 4;
    private static final int[] RENT_MULTIPLIERS = {1, 2, 3, 5, 8};

    private int buildLevel;

    public Property() {
        super();
        System.out.println("[Property] Конструктор по умолчанию");
    }

    public Property(String name, int price, int baseRent) {
        super(name, price, baseRent);
        System.out.println("[Property] Конструктор с параметрами: " + name);
    }

    public Property(Property other) {
        super(other);
        this.buildLevel = other.buildLevel;
        System.out.println("[Property] Конструктор копирования: " + other.getName());
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
        if (!isOwned() || isMortgaged()) {
            return 0;
        }
        int multiplier = RENT_MULTIPLIERS[buildLevel];
        if (getBaseRent() > Integer.MAX_VALUE / multiplier) {
            throw new GameRuleException("Сумма аренды участка слишком велика");
        }
        return getBaseRent() * multiplier;
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

    public void build() {
        if (isMortgaged()) {
            throw new GameRuleException("Нельзя строить на заложенном участке");
        }
        if (buildLevel >= MAX_BUILD_LEVEL) {
            throw new GameRuleException("Достигнут предел застройки участка");
        }
        buildLevel++;
    }

    public void sellBuilding() {
        if (buildLevel == 0) {
            throw new GameRuleException("На участке нет построек для продажи");
        }
        buildLevel--;
    }

    @Override
    public void mortgage() {
        if (buildLevel > 0) {
            throw new GameRuleException("Нельзя заложить участок с постройкой");
        }
        super.mortgage();
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
