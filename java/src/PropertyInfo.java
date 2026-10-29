// Изменяемая карточка недвижимости
public class PropertyInfo {
    private String name;
    private final int price;
    private int baseRent;

    public PropertyInfo(String name, int price, int baseRent) {
        if (price < 0 || baseRent < 0) {
            throw new IllegalArgumentException("Цена и базовая аренда не могут быть отрицательными");
        }
        this.name = name;
        this.price = price;
        this.baseRent = baseRent;
    }

    public PropertyInfo(PropertyInfo other) {
        this(other.name, other.price, other.baseRent);
    }

    public String getName() {
        return name;
    }

    public int getPrice() {
        return price;
    }

    public int getBaseRent() {
        return baseRent;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setBaseRent(int baseRent) {
        if (baseRent < 0) {
            throw new IllegalArgumentException("Базовая аренда не может быть отрицательной");
        }
        this.baseRent = baseRent;
    }

    @Override
    public boolean equals(Object object) {
        if (this == object) {
            return true;
        }
        if (object == null || getClass() != object.getClass()) {
            return false;
        }
        PropertyInfo other = (PropertyInfo) object;
        return price == other.price && baseRent == other.baseRent && name.equals(other.name);
    }

    @Override
    public int hashCode() {
        int result = name.hashCode();
        result = 31 * result + price;
        result = 31 * result + baseRent;
        return result;
    }
}
