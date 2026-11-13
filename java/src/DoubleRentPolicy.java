public final class DoubleRentPolicy implements RentPolicy {
    @Override
    public int apply(int rent) {
        if (rent < 0) {
            throw new GameRuleException("Недопустимая сумма двойной аренды");
        }
        return rent * 2;
    }
}
