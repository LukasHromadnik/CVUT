package Database;

import Models.Trip;

import java.util.HashMap;

public class Database {

    private HashMap<Integer, Trip> data;

    public static Database instance = null;

    public static Database getInstance() {
        if (Database.instance == null) {
            Database.instance = new Database();
        }

        return Database.instance;
    }

    private Database() {
        this.data = new HashMap<Integer, Trip>();

        Trip t1 = new Trip(1, "Praha", 10);
        addTrip(t1);
        Trip t2 = new Trip(2, "Londýn", 20);
        addTrip(t2);
        Trip t3 = new Trip(3, "New York", 30);
        addTrip(t3);
        Trip t4 = new Trip(4, "Sydney", 40);
        addTrip(t4);
        Trip t5 = new Trip(5, "Bratislava", 50);
        addTrip(t5);
    }

    public void addTrip(Trip trip) {
        data.put(trip.getID(), trip);
    }

    public Trip getTrip(Integer id) {
        return data.get(id);
    }

    public void updateTrip(Trip trip) {
        data.put(trip.getID(), trip);
    }

    public HashMap<Integer, Trip> getTrips() {
        return data;
    }
}
