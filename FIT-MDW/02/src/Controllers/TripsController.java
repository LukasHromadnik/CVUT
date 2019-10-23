package Controllers;

import Database.Database;
import Main.Router;
import Models.Person;
import Models.Trip;

import java.io.IOException;

public class TripsController extends Controller {

    private Database database = Database.getInstance();

    @Override
    public void listAll() {
        out.println("<html>");
        out.println("<body>");
        out.println("<h1>Trips</h1>");

        out.println("<table border=\"1\">");
        out.println("<tr>");
        out.println("<th>ID</th>");
        out.println("<th>Name</th>");
        out.println("<th>Capacity</th>");
        out.println("<th>Remaining</th>");
        out.println("<th>Actions</th>");
        out.println("</tr>");

        for (Integer key : database.getTrips().keySet()) {
            Trip trip = database.getTrip(key);
            out.println("<tr>");
            out.println("<td>" + trip.getID() + "</td>");
            out.println("<td>" + trip.getName() + "</td>");
            out.println("<td>" + trip.getCapacity() + "</td>");
            out.println("<td>" + trip.getRemaining() + "</td>");
            out.println("<td>");
            out.println("<a href=\"" + Router.buildURL("trips", trip.getID())  + "\">Detail</a>");
            out.println("| <a href=\"" + Router.buildURL("trips", trip.getID(), "edit") + "\">Edit</a>");
            out.println("| <a href=\"" + Router.buildURL("trips", trip.getID(), "bookings", 0, "edit") + "\">Book now</a>");
            out.println("</td>");
            out.println("</tr>");
        }

        out.println("</table>");

        out.println("<br><a href=\"" + Router.buildURL("trips", 0, "edit") + "\">Add new trip</a>");

        out.println("</body>");
        out.println("</html>");
    }

    public void edit(Integer id) {
        out.println("<html>");
        out.println("<body>");

        out.println("<h1>");
        Trip trip = null;
        if (id.equals(0)) {
            out.println("Add new trip");
        } else {
            out.println("Edit trip");
            trip = database.getTrip(id);
        }
        out.println("</h1>");

        out.println("<form action=\"" + Router.buildURL("trips", id, "save") + "\" method=\"POST\">");
        out.println("<label for=\"name\">Name</label>");
        out.println("<input type=\"text\" id=\"name\" name=\"name\" placeholder=\"Name\"");
        if (trip != null) {
            out.println(" value=\"" + trip.getName() + "\"");
        }
        out.println("><br>");
        out.println("<label for=\"capacity\">Capacity</label>");
        out.println("<input type=\"text\" id=\"capacity\" name=\"capacity\" placeholder=\"Capacity\"");
        if (trip != null) {
            out.println(" value=\"" + trip.getCapacity() + "\"");
        }
        out.println("><br>");
        out.println("<input type=\"submit\">");
        out.println("</form>");

        out.println("</body>");
        out.println("</html>");
    }

    public void save(Integer id) throws IOException {
        if (id.equals(0)) {
            Integer maxID = 0;
            for (Trip trip : database.getTrips().values()) {
                maxID = Math.max(trip.getID(), maxID);
            }
            database.addTrip(new Trip(maxID + 1, Router.request.getParameter("name"), Integer.parseInt(Router.request.getParameter("capacity"))));
        } else {
            Trip trip = database.getTrip(id);
            trip.setName(Router.request.getParameter("name"));
            trip.setCapacity(Integer.parseInt(Router.request.getParameter("capacity")));
            database.updateTrip(trip);
        }

        Router.response.sendRedirect("/trips");
    }

    public void detail(Integer id) {
        Trip trip = database.getTrip(id);

        out.println("<html>");
        out.println("<body>");

        out.println("<h1>" + trip.getName() + "</h1>");

        out.println("<h2>Info</h2>");

        out.println("<ul>");
        out.println("<li>Capacity: " + trip.getCapacity() + "</li>");
        out.println("<li>Remaining: " + trip.getRemaining() + "</li>");
        out.println("</ul>");

        out.println("<h2>Attendants</h2>");

        out.println("<ul>");
        for (Integer key : trip.getPersons().keySet()) {
            Person person = trip.getPerson(key);
            out.println("<li>" + person.getName() + " <a href=\"" + Router.buildURL("trips", trip.getID(), "bookings", person.getID(), "edit") + "\">Edit</a> | <a href=\"" + Router.buildURL("trips", trip.getID(), "bookings", person.getID(), "remove") + "\">Remove</a></li>");
        }
        out.println("<li><a href=\"" + Router.buildURL("trips", trip.getID(), "bookings", 0, "edit") + "\">Add new</a></li>");
        out.println("</ul>");

        out.println("</body>");
        out.println("</html>");
    }
}
