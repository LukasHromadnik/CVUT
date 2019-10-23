package Models;

import java.util.HashMap;

public class Trip {
    private String name;
    private Integer id;
    private Integer capacity;
    private HashMap<Integer, Person> persons;

    public Trip(Integer id, String name, Integer capacity) {
        this.id = id;
        this.name = name;
        this.capacity = capacity;
        this.persons = new HashMap<Integer, Person>();
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getID() {
        return id;
    }

    public Integer getCapacity() {
        return capacity;
    }

    public void setCapacity(Integer capacity) {
        this.capacity = capacity;
    }

    public Integer getRemaining() {
        return capacity - persons.size();
    }

    public void addPerson(Person person) {
        persons.put(person.getID(), person);
    }

    public HashMap<Integer, Person> getPersons() {
        return persons;
    }

    public Person getPerson(Integer index) {
        return persons.get(index);
    }

    public void updatePerson(Person person) {
        persons.put(person.getID(), person);
    }

    public Person removePerson(Integer index) {
        return persons.remove(index);
    }
}