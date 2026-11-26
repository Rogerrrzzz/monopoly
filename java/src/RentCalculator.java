public final class RentCalculator<T extends Rentable> {
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

    public int calculate(T property) {
        return calculate(property, policy);
    }

    // Разовое правило доступно коду из того же пакета
    int calculate(T property, RentPolicy oneTimePolicy) {
        return oneTimePolicy.apply(property.calcRent());
    }

    public <U extends Rentable> long calculateTotal(T first, U second) {
        return (long) calculate(first) + policy.apply(second.calcRent());
    }
}
