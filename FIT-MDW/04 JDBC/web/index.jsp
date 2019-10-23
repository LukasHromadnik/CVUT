<%--
  Created by IntelliJ IDEA.
  User: Luke
  Date: 13.11.16
  Time: 21:21
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>MDW-HW-04-JDBC</title>
</head>
<body>

<h1>Insert your SQL statement</h1>

<form action="./perform" method="GET">
    <label for="query">Query:</label><br>
    <textarea name="query" id="query" rows="3" cols="50">SELECT * FROM records</textarea><br>
    <input type="submit">
</form>

</body>
</html>
