package Main;

import Controllers.Controller;
import com.sun.deploy.net.HttpRequest;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.util.ArrayList;

public class Router extends HttpServlet {

    private PrintWriter out;

    public static HttpServletRequest request = null;
    public static HttpServletResponse response = null;

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        handleRequest(req, resp);
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        handleRequest(req, resp);
    }

    private void handleRequest(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        Router.request = request;
        Router.response = response;

        response.setContentType("text/html");
        out = response.getWriter();

        String[] uriParts = request.getRequestURI().split("\\/");
        ArrayList<String> url = new ArrayList<String>();
        for (int i = 1; i < uriParts.length; i++) {
            url.add(uriParts[i]);
        }

        // Controller

        Controller controller = null;
        try {
            Class controllerClass = loadControllerClass(url.get(0));
            controller = (Controller) controllerClass.newInstance();
        } catch (Exception e) {
            handleException(e);
            return;
        }

        controller.out = response.getWriter();

        if (url.size() == 1) {
            controller.listAll();
            return;
        }

        if (url.size() == 2) {
            url.add("detail");
        }

        try {
            Method method = controller.getClass().getMethod(url.get(2), Integer.class);
            method.invoke(controller, Integer.parseInt(url.get(1)));
            return;
        } catch (NoSuchMethodException e) {
            try {
                Class subControllerClass = loadControllerClass(url.get(2));
                Constructor subControllerConstructor = subControllerClass.getConstructor(Integer.class);
                controller = (Controller) subControllerConstructor.newInstance(Integer.parseInt(url.get(1)));
                controller.out = this.out;
            } catch (Exception ex) {
                handleException(ex);
                return;
            }
        } catch (Exception e) {
            handleException(e);
            return;
        }

        if (url.size() == 3) {
            controller.listAll();
            return;
        }

        if (url.size() == 4) {
            url.add("detail");
        }

        try {
            Method method = controller.getClass().getMethod(url.get(4), Integer.class);
            method.invoke(controller, Integer.parseInt(url.get(3)));
            return;
        } catch (Exception e) {
            handleException(e);
            return;
        }
    }

    private void handleException(Exception e) {
        out.println(e);
        e.printStackTrace();
    }

    private Class loadControllerClass(String name) throws Exception {
        String controllerName = name.toLowerCase();
        String controllerClassName = "Controllers." + controllerName.substring(0, 1).toUpperCase() + controllerName.substring(1) + "Controller";

        return Class.forName(controllerClassName);
    }

    public static String buildURL(Object... args) {
        String result = "";
        for (Object arg : args) {
            result += "/" + arg.toString();
        }
        return result;
    }
}
