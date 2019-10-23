/**
 * Created by Lukáš Hromadník on 13.11.16.
 */

import java.io.IOException;
import java.io.PrintWriter;
import java.net.URLDecoder;
import java.nio.charset.StandardCharsets;
import java.sql.*;
import java.util.Date;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.swing.plaf.nimbus.State;

import java.util.HashMap;
import java.util.Map;

public class MainServlet extends HttpServlet {

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        PrintWriter out = response.getWriter();
        out.println("<html>");

        Connection connection = null;
        try {
            connection = prepareConnection();
        } catch (Exception e) {
            out.println(e.toString());
        }

        Map<String, String> query = parseQuery(request.getQueryString());

        String queryString = query.get("query");
        if (queryString == null) {
            out.println("Nothing to do");
            return;
        }

        try {
            Statement statement = connection.createStatement();
            ResultSet result = statement.executeQuery(queryString.replace("+", " "));
            ResultSetMetaData metaData = result.getMetaData();
            out.println("<table border=\"1\"><tr>");
            for (int i = 1; i <= metaData.getColumnCount(); i++) {
                out.println("<th>" + metaData.getColumnName(i) + "</th>");
            }
            out.println("</tr>");
            while (result.next()) {
                out.println("<tr>");
                for (int i = 1; i <= metaData.getColumnCount(); i++) {
                    out.println("<td>" + result.getString(i) + "</td>");
                }
                out.println("</tr>");
            }
            out.println("</table>");

            statement.close();
            connection.close();
        } catch (Exception e) {
            out.println(e.toString());
        }
        out.println("<br><br><a href=\"http://localhost:8080\">Back</a>");
        out.println("</html>");
    }

    private Connection prepareConnection() throws Exception {
        Class.forName("com.mysql.jdbc.Driver");
        Connection conn = null;
        String database = "jdbc:mysql://147.32.233.18:3306/mimdw";
        return DriverManager.getConnection(database, "mimdw", "mimdw");
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
