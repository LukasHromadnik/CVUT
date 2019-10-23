/**
 * Created by Lukáš Hromadník on 22.11.16.
 */
public enum RequestMethod {
    GET("GET"), POST("POST");

    private String method;

    RequestMethod(String method) {
        this.method = method;
    }

    public String getMethod() {
        return method;
    }
}
