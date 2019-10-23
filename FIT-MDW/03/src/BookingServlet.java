/**
 * Created by Lukáš Hromadník on 13.11.16.
 */

import java.io.IOException;
import java.io.PrintWriter;
import java.net.URLDecoder;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.*;

public class BookingServlet extends HttpServlet {

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        HttpSession session = req.getSession();
        String id = session.getId();
        resp.addCookie(new Cookie("lorem", "ipsum"));

        switch (req.getRequestURI()) {
            case "/new":
                doNew(req, resp);
                break;
            case "/payment":
                doPayment(req, resp);
                break;
            default:
                PrintWriter out = resp.getWriter();
                out.println("Unauthorized access!");
                break;
        }
    }

    private void doNew(HttpServletRequest request, HttpServletResponse response) throws IOException {
        HttpSession session = request.getSession();
        PrintWriter out = response.getWriter();

        String stateName = (String) session.getAttribute("bookingState");
        BookingState bookingState = BookingState.createFromString(stateName);

        Map<String, String> map;
        try {
            map = parseQuery(request.getQueryString());
        } catch (IOException e) {
            out.println(e.toString());
            return;
        }

        boolean nameFound = false;
        boolean destinationFound = false;

        for (String key : map.keySet()) {
            if (key.equals("name")) {
                nameFound = true;
            }
            if (key.equals("destination")) {
                destinationFound = true;
            }
        }

        if (!nameFound) {
            out.println("Unable to find the attendant's name");
            return;
        }

        if (!destinationFound) {
            out.println("Unable to find the destination");
            return;
        }

        out.println("Successfully created the order");
        out.println("Name: " + map.get("name"));
        out.println("Destination: " + map.get("destination"));
        out.println("---");
        out.println("Waiting for the payment");

        session.setAttribute("bookingState", BookingState.PAYMENT.identifier);
    }

    private void doPayment(HttpServletRequest request, HttpServletResponse response) throws IOException {
        HttpSession session = request.getSession();
        PrintWriter out = response.getWriter();

        String stateName = (String) session.getAttribute("bookingState");
        BookingState bookingState = BookingState.createFromString(stateName);

        if (bookingState != BookingState.PAYMENT) {
            out.println("You must create the order at first");
            return;
        }

        Map<String, String> query;
        try {
            query = parseQuery(request.getQueryString());
        } catch (IOException e) {
            out.println("Unable to complete the order. You must pay something");
            return;
        }

        boolean cashFound = false;

        for (String key : query.keySet()) {
            if (key.equals("cash")) {
                cashFound = true;
            }
        }

        if (!cashFound) {
            out.println("Unable to complete the order. You must pay something");
            return;
        }

        int cash = 0;
        try {
            cash = Integer.parseInt(query.get("cash"));
        } catch (NumberFormatException e) {
            out.println("You must specify amount");
            return;
        }

        if (cash <= 0) {
            out.println("Nothing is for free");
            return;
        }

        out.println("Order successfully completed");
        out.println("You pay: " + cash + "$");

        session.setAttribute("bookingState", BookingState.COMPLETED.identifier);
    }

    private Map<String, String> parseQuery(String query) throws IOException {
        if (query == null) {
            throw new IOException("Unable to parse input data");
        }

        String[] params = query.split("&");
        Map<String, String> map = new HashMap<String, String>();
        for (String param : params) {
            String name = param.split("=")[0];
            String value = param.split("=")[1];
            value = URLDecoder.decode(value, StandardCharsets.UTF_8.name());
            map.put(name, value);
        }

        return map;
    }
}
