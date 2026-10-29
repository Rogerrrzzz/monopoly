// Конечный тип недвижимости без строительства
public final class RailwayStation extends BaseProperty {
    public RailwayStation() {
        super();
        System.out.println("[RailwayStation] Конструктор по умолчанию");
    }

    public RailwayStation(String name, int price, int baseRent) {
        super(name, price, baseRent);
        System.out.println("[RailwayStation] Конструктор с параметрами: " + getName());
    }

    public RailwayStation(RailwayStation other) {
        super(other);
        System.out.println("[RailwayStation] Конструктор копирования: " + getName());
    }

    @Override
    public int calcRent() {
        return !isOwned() || isMortgaged() ? 0 : getBaseRent();
    }

    @Override
    public String describe() {
        // Описание формируется без вызова super.describe()
        return "Вокзал " + header() + ", фиксированная аренда " + calcRent();
    }

    @Override
    public RailwayStation shallowCopy() {
        RailwayStation copy = new RailwayStation(getName(), getPrice(), getBaseRent());
        copySharedStateTo(copy);
        return copy;
    }

    @Override
    public RailwayStation deepCopy() {
        return new RailwayStation(this);
    }
}
