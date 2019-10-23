package Controllers;

import Main.Router;
import Models.Person;
import Models.Trip;
import Database.Database;

import java.io.IOException;

public class BookingsController extends Controller {

    private Trip trip;

    public BookingsController(Integer tripID) {
        this.trip = Database.getInstance().getTrip(tripID);
    }

    @Override
    public void listAll() {
        out.println("<html>");
        out.println("<body>");

        out.println("<h1>" + trip.getName() + "</h1>");

        out.println("<h2>Attendants</h2>");

        out.println("<ul>");
        for (Integer key : trip.getPersons().keySet()) {
            Person person = trip.getPerson(key);
            out.println("<li>" + person.getName() + " <a href=\"" + Router.buildURL("trips", trip.getID(), "bookings", person.getID(), "edit") +"\">Edit</a></li>");
        }
        out.println("</ul>");

        out.println("</body>");
        out.println("</html>");
    }

    public void edit(Integer id) {
        out.println("<html>");
        out.println("<body>");

        out.println("<h1>Book trip to " + trip.getName() + "</h1>");

        out.println("<form action=\"" + Router.buildURL("trips", trip.getID(), "bookings", id, "save") + "\" method=\"POST\">");
        out.println("<label for=\"name\">Full name</label>");
        out.println("<input type=\"text\" id=\"name\" name=\"name\" placeholder=\"Full name\"");
        if (!id.equals(0)) {
            out.println(" value=\"" + trip.getPerson(id).getName() + "\"");
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
            for (Integer key : trip.getPersons().keySet()) {
                maxID = Math.max(maxID, trip.getPerson(key).getID());
            }

            trip.addPerson(new Person(maxID + 1, Router.request.getParameter("name")));
        } else {
            Person person = trip.getPerson(id);
            person.setName(Router.request.getParameter("name"));
            trip.updatePerson(person);
        }

        Database.getInstance().updateTrip(trip);

        Router.response.sendRedirect("/trips/" + trip.getID());
    }

    public void remove(Integer id) throws IOException {
        trip.removePerson(id);
        Database.getInstance().updateTrip(trip);
        Router.response.sendRedirect("/trips/" + trip.getID());
    }
}
