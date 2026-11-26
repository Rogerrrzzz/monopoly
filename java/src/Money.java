public record Money(long amount) {
    public Money plus(Money other) {
        return new Money(Math.addExact(amount, other.amount));
    }

    public Money times(int count) {
        return new Money(Math.multiplyExact(amount, count));
    }

    @Override
    public String toString() {
        return Long.toString(amount);
    }
}
