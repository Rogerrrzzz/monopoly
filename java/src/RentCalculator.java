public final class RentCalculator {
    private RentPolicy policy;

    public RentCalculator(RentPolicy policy) {
        setPolicy(policy);
    }

    public void setPolicy(RentPolicy policy) {
        if (policy == null) {
            throw new IllegalArgumentException("Правило расчёта аренды не задано");
        }
        this.policy = policy;
    }

    public int calculate(Rentable property) {
        return policy.apply(property.calcRent());
    }
}
