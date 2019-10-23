package mas.agents;

import java.io.*;
import java.util.Base64;

public class Serde {

    public static String serialize(Serializable obj) throws IOException {
        ByteArrayOutputStream bo = new ByteArrayOutputStream();
        ObjectOutputStream so = new ObjectOutputStream(bo);
        so.writeObject(obj);
        so.flush();
        return new String(Base64.getEncoder().encode(bo.toByteArray()));
    }

    public static Object deserialize(String str) throws IOException, ClassNotFoundException {
        byte b[] = Base64.getDecoder().decode(str.getBytes());
        ByteArrayInputStream bi = new ByteArrayInputStream(b);
        ObjectInputStream si = new ObjectInputStream(bi);
        return si.readObject();
    }

}
