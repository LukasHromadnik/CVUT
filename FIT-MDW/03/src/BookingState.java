/**
 * Created by ${MyName} on 13.11.16.
 */
public enum BookingState {
    NEW("new"), PAYMENT("payment"), COMPLETED("completed");

    public String identifier;

    private BookingState(String identifier) {
        this.identifier = identifier;
    }

    public static BookingState createFromString(String name) {
        for (BookingState value : BookingState.values()) {
            if (value.identifier.equals(name)) {
                return value;
            }
        }

        return BookingState.NEW;
    }

    public BookingState next() {
        if (this.equals(BookingState.NEW)) {
            return BookingState.PAYMENT;
        } else if (this.equals(BookingState.PAYMENT)) {
            return BookingState.COMPLETED;
        }

        return null;
    }
}

