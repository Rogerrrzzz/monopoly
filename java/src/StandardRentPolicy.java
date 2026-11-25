public final class StandardRentPolicy implements RentPolicy {
    @Override
    public int apply(int rent) {
        return rent;
    }
}
