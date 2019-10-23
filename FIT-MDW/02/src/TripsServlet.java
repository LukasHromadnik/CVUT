/**
 * Created by Lukáš Hromadník on 15.11.16.
 */

import Database.Database;
import Models.Person;
import Models.Trip;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class TripsServlet extends HttpServlet {

    private Database database = Database.getInstance();

//    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
//        String uri = request.getRequestURI();
//
//        Pattern pattern = Pattern.compile("\\/trips\\/(\\d+)\\/(\\w+)\\/?");
//        Matcher match = pattern.matcher(uri);
//
//        if (match.find()) {
//            Integer id = Integer.parseInt(match.group(1));
//            String methodName = match.group(2);
//            switch (methodName) {
//                case "edit":
//                    editTrip(id, request, response);
//                    break;
//                case "book":
//                    showBookTrip(id, request, response);
//                    break;
//            }
//        } else {
//            showList(request, response);
//        }
//    }
//
//    @Override
//    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
//        String uri = request.getRequestURI();
//
//        Pattern pattern = Pattern.compile("\\/trips\\/(\\d+)\\/([a-z-]+)\\/?");
//        Matcher match = pattern.matcher(uri);
//
//        if (match.find()) {
//            Integer id = Integer.parseInt(match.group(1));
//            String methodName = match.group(2);
//            switch (methodName) {
//                case "save":
//                    saveTrip(id, request, response);
//                    break;
//                case "save-booking":
//                    saveBooking(id, request, response);
//                    break;
//            }
//        } else {
//            response.setContentType("text/html");
//            PrintWriter out = response.getWriter();
//
//            out.println("<html><body><h1>400 Bad Request</h1></body></html>");
//        }
//    }
//
//    private void showList(HttpServletRequest request, HttpServletResponse response) throws IOException {
//        response.setContentType("text/html");
//        PrintWriter out = response.getWriter();
//
//        out.println("<html>");
//        out.println("<body>");
//        out.println("<h1>Trips</h1>");
//
//        out.println("<table border=\"1\">");
//        out.println("<tr>");
//        out.println("<th>ID</th>");
//        out.println("<th>Name</th>");
//        out.println("<th>Capacity</th>");
//        out.println("<th>Remaining</th>");
//        out.println("<th>Actions</th>");
//        out.println("</tr>");
//
//        for (Integer key : database.getTrips().keySet()) {
//            Trip trip = database.getTrip(key);
//            out.println("<tr>");
//            out.println("<td>" + trip.getID() + "</td>");
//            out.println("<td>" + trip.getName() + "</td>");
//            out.println("<td>" + trip.getCapacity() + "</td>");
//            out.println("<td>" + trip.getRemaining() + "</td>");
//            out.println("<td>");
//            out.println("<a href=\"/trips/" + trip.getID() + "\">Detail</a>");
//            out.println("| <a href=\"/trips/" + trip.getID() + "/edit\">Edit</a>");
//            out.println("| <a href=\"/trips/" + trip.getID() + "/book\">Book now</a>");
//            out.println("</td>");
//            out.println("</tr>");
//        }
//
//        out.println("</table>");
//
//        out.println("<br><a href=\"/trips/0/edit\">Add new trip</a>");
//
//        out.println("</body>");
//        out.println("</html>");
//    }
//
//    private void editTrip(Integer id, HttpServletRequest request, HttpServletResponse response) throws IOException {
//        response.setContentType("text/html");
//        PrintWriter out = response.getWriter();
//
//        out.println("<html>");
//        out.println("<body>");
//
//        out.println("<h1>");
//        Trip trip = null;
//        if (id.equals(0)) {
//            out.println("Add new trip");
//        } else {
//            out.println("Edit trip");
//            trip = database.getTrip(id);
//        }
//        out.println("</h1>");
//
//        out.println("<form action=\"/trips/" + id + "/save\" method=\"POST\">");
//        out.println("<label for=\"name\">Name</label>");
//        out.println("<input type=\"text\" id=\"name\" name=\"name\" placeholder=\"Name\"");
//        if (trip != null) {
//            out.println(" value=\"" + trip.getName() + "\"");
//        }
//        out.println("><br>");
//        out.println("<label for=\"capacity\">Capacity</label>");
//        out.println("<input type=\"text\" id=\"capacity\" name=\"capacity\" placeholder=\"Capacity\"");
//        if (trip != null) {
//            out.println(" value=\"" + trip.getCapacity() + "\"");
//        }
//        out.println("><br>");
//        out.println("<input type=\"submit\">");
//        out.println("</form>");
//
//        out.println("</body>");
//        out.println("</html>");
//    }
//
//    private void saveTrip(Integer id, HttpServletRequest request, HttpServletResponse response) throws IOException {
//        if (id.equals(0)) {
//            Integer maxID = 0;
//            for (Trip trip : database.getTrips().values()) {
//                maxID = Math.max(trip.getID(), maxID);
//            }
//            database.addTrip(new Trip(maxID + 1, request.getParameter("name"), Integer.parseInt(request.getParameter("capacity"))));
//        } else {
//            Trip trip = database.getTrip(id);
//            trip.setName(request.getParameter("name"));
//            trip.setCapacity(Integer.parseInt(request.getParameter("capacity")));
//            database.updateTrip(trip);
//        }
//
//        response.sendRedirect("/trips");
//
//    }
//
//    private void showBookTrip(Integer id, HttpServletRequest request, HttpServletResponse response) throws IOException {
//        response.setContentType("text/html");
//        PrintWriter out = response.getWriter();
//
//        out.println("<html>");
//        out.println("<body>");
//
//        Trip trip = database.getTrip(id);
//        out.println("<h1>Book trip to " + trip.getName() + "</h1>");
//
//        out.println("<form action=\"/trips/" + id + "/save-booking\" method=\"POST\">");
//        out.println("<label for=\"name\">Full name</label>");
//        out.println("<input type=\"text\" id=\"name\" name=\"name\" placeholder=\"Full name\">");
//        out.println("<br>");
//        out.println("<input type=\"submit\">");
//        out.println("</form>");
//
//        out.println("</body>");
//        out.println("</html>");
//    }
//
//    private void saveBooking(Integer id, HttpServletRequest request, HttpServletResponse response) throws IOException {
//        Trip trip = database.getTrip(id);
//        trip.addPerson(new Person(request.getParameter("name")));
//        database.updateTrip(trip);
//
//        response.sendRedirect("/trips");
//    }
}
