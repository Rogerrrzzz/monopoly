/** Участок с ценой, владельцем, уровнем застройки и возможным залогом. */
public class Property {
    private static final int BANK_OWNER_ID = -1;
    private static final int MAX_BUILD_LEVEL = 4;
    private static final int[] RENT_MULTIPLIERS = {1, 2, 3, 5, 8};

    private final String name;
    private final int price;
    private final int baseRent;
    private int buildLevel;
    private int ownerId = BANK_OWNER_ID;
    private boolean mortgaged;

    public Property(String name, int price, int baseRent) {
        if (price < 0 || baseRent < 0) {
            throw new IllegalArgumentException("Цена и базовая аренда не могут быть отрицательными");
        }
        this.name = name;
        this.price = price;
        this.baseRent = baseRent;
        System.out.println("[Property] Создан участок \"" + name + "\", цена " + price);
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

    public int getBuildLevel() {
        return buildLevel;
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

    public int getBuildCost() {
        return price / 2;
    }

    public int getBuildRefund() {
        return getBuildCost() / 2;
    }

    public int getMortgageValue() {
        return price / 2;
    }

    public int getRedeemCost() {
        return (int) (getMortgageValue() * 11L / 10);
    }

    public int calcRent() {
        return !isOwned() || mortgaged ? 0 : baseRent * RENT_MULTIPLIERS[buildLevel];
    }

    public boolean assignOwner(int playerId) {
        if (isOwned()) {
            System.out.println("[Отказ] Участок \"" + name + "\" уже принадлежит игроку " + ownerId);
            return false;
        }
        if (playerId <= 0) {
            System.out.println("[Отказ] Идентификатор владельца должен быть положительным");
            return false;
        }
        ownerId = playerId;
        return true;
    }

    public boolean build() {
        if (mortgaged) {
            System.out.println("[Отказ] Нельзя строить на заложенном участке \"" + name + "\"");
            return false;
        }
        if (buildLevel >= MAX_BUILD_LEVEL) {
            System.out.println("[Отказ] На участке \"" + name + "\" уже максимальный уровень застройки");
            return false;
        }
        buildLevel++;
        return true;
    }

    public boolean sellBuilding() {
        if (buildLevel == 0) {
            System.out.println("[Отказ] На участке \"" + name + "\" нет построек");
            return false;
        }
        buildLevel--;
        return true;
    }

    public boolean mortgage() {
        if (buildLevel > 0) {
            System.out.println("[Отказ] Нельзя заложить участок \"" + name + "\", пока на нём есть постройки");
            return false;
        }
        if (mortgaged) {
            System.out.println("[Отказ] Участок \"" + name + "\" уже заложен");
            return false;
        }
        mortgaged = true;
        return true;
    }

    public boolean redeem() {
        if (!mortgaged) {
            System.out.println("[Отказ] Участок \"" + name + "\" не находится в залоге");
            return false;
        }
        mortgaged = false;
        return true;
    }

    public void returnToBank() {
        ownerId = BANK_OWNER_ID;
        buildLevel = 0;
        mortgaged = false;
    }

    @Override
    public String toString() {
        String owner = isOwned() ? "игрок " + ownerId : "банк";
        return "Участок \"" + name + "\": цена " + price + ", владелец - " + owner
                + ", уровень застройки " + buildLevel + ", аренда " + calcRent()
                + (mortgaged ? ", в залоге" : "");
    }
}
