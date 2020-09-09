## Java学习笔记 : JDBC

---

#### 第一个JDBC程序

```java
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;

public class Demo01 {
    public static void main(String[] args) throws Exception {
        //加载驱动
        Class.forName("com.mysql.jdbc.Driver");

        //建立连接对象
        String url = "jdbc:mysql://localhost:3306/jdbcstudy?useUnicode=true&characterEncoding=utf8&useSSL=false";
        String username = "root";
        String password = "123456";
        Connection connection = DriverManager.getConnection(url,username,password);

        //使用连接对象建立执行sql的对象
        Statement statement = connection.createStatement();

        //执行sql，查询结果保存在结果集对象里面
        String sql = "SELECT * FROM users";
        ResultSet resultSet = statement.executeQuery(sql);

        //遍历结果集对象取到值
        while(resultSet.next()){
            System.out.println("id=" + resultSet.getObject("id"));
            System.out.println("name=" + resultSet.getObject("NAME"));
            System.out.println("pwd=" + resultSet.getObject("PASSWORD"));
            System.out.println("email=" + resultSet.getObject("email"));
            System.out.println("birth=" + resultSet.getObject("birthday"));
        }
        
        //记得关闭所有对象
        resultSet.close();;
        statement.close();
        connection.close();
    }
}
```



#### JDBC中的对象

- DriverManager

  ```java
  Class.forName("com.mysql.jdbc.Driver");	//固定写法，加载驱动
  ```

- URL

  ```java
  String url = "jdbc:mysql://localhost:3306/jdbcstudy?useUnicode=true&characterEncoding=utf8&useSSL=false";
  //协议://主机地址:端口号/数据库名?参数1&参数2&参数3
  ```

- Connection：代表数据库，数据库设置自动提交，事务提交与回滚

- Statement：执行SQL的对象

  ```java
  statement.executeQuery();	//查询操作返回 ResultSet
  statement.execute();		//执行任何SQL
  statement.executeUpdate();	//更新、插入、删除，返回受影响行数
  ```

- ResultSet：查询的结果集，封装了所有查询结果

  ```java
  //使用方式类似迭代器
  resultSet.next();
  resultSet.beforeFirst();
  resultSet.afterLast();
  resultSet.absolute(row);
  ```



#### 使用工具类封装

```properties
#配置文件
driver=com.mysql.jdbc.Driver
url=jdbc:mysql://localhost:3306/jdbcstudyuseUnicode=true&characterEncoding=utf8&useSSL=false
username=root
password=123456
```

```java
//工具类
public class JdbcUtils {

    private static String driver =null;
    private static String url =null;
    private static String username =null;
    private static String password =null;


    static{
        try{
            InputStream in = JdbcUtils.class.getClassLoader().getResourceAsStream("db.properties");
            Properties properties = new Properties();
            properties.load(in);

            driver = properties.getProperty("driver");
            url = properties.getProperty("url");
            username = properties.getProperty("username");
            password = properties.getProperty("password");

            //1.驱动只用加载一次
            Class.forName(driver);
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    //获取连接
    public static Connection getConnection() throws SQLException{
        return DriverManager.getConnection(url,username,password);
    }

    //释放连接
    public static void release(Connection conn, Statement st, ResultSet rs){
        if(rs!=null){
            try {
                rs.close();
            } catch (SQLException throwables) {
                throwables.printStackTrace();
            }
        }
        if(st!=null){
            try {
                st.close();
            } catch (SQLException throwables) {
                throwables.printStackTrace();
            }
        }
        if(conn!=null){
            try {
                conn.close();
            } catch (SQLException throwables) {
                throwables.printStackTrace();
            }
        }
    }
}
```

```java
//main函数

public class Demo01 {
    public static void main(String[] args){
        Connection conn = null;
        Statement st = null;
        ResultSet rs = null;

        try {
            conn = JdbcUtils.getConnection();
            st = conn.createStatement();
            String sql = "INSERT INTO users(id,`NAME`,`PASSWORD`,`email`,`birthday`)" +
                    "VALUES(4,'kinomi','123456','kinomi@stu.pku.edu.cn','2020-01-01')";
            int i = st.executeUpdate(sql);
            if(i > 0){
                System.out.println("插入成功");
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }finally {
            JdbcUtils.release(conn,st,rs);
        }

    }
}
```



#### SQL注入问题

SQL存在漏洞，可能被攻击，本质是因为SQL会被拼接。PreparedStatement可以防止SQL注入

```java
public class Demo01 {
    public static void main(String[] args){
        Connection conn = null;
        PreparedStatement pst = null;

        try {
            conn = JdbcUtils.getConnection();
            //使用?占位符代替参数
            String sql = "INSERT INTO users(id,`NAME`,`PASSWORD`,`email`,`birthday`) VALUES(?,?,?,?,?)";
            pst = conn.prepareStatement(sql);      //预编译SQL，先写sql，不执行

            //手动给参数赋值(下标,值)
            pst.setInt(1,4);
            pst.setString(2,"kinomi");
            pst.setString(3,"123456");
            pst.setString(4,"kinomi@stu.pku.edu.cn");
            pst.setDate(5,new java.sql.Date(new java.util.Date().getTime()));

            //执行
            int i = pst.executeUpdate();
            if(i>0){
                System.out.println("插入成功");
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }finally {
            JdbcUtils.release(conn,pst,null);
        }

    }
}
```



#### JDBC操作事务