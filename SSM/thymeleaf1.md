**thymeleaf**

Thymeleaf是用来开发Web和独立环境项目的服务器端的Java模版引擎。



引入依赖

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-thymeleaf</artifactId>
</dependency>
```

Spring Boot默认存放模板页面的路径在`src/main/resources/templates`或者`src/main/view/templates`，默认的页面文件后缀是`.html`。



Thymeleaf的主要作用是把model中的数据渲染到html中，因此其语法主要是如何解析model中的数据。

```java
@Controller
public class ThymeleafController {

    @RequestMapping(value = "show", method = RequestMethod.GET)
    public String show(Model model){
        model.addAttribute("uid","123456789");
        model.addAttribute("name","Jerry");
        return "show";
    }
} 
```

show.html

```html
<!DOCTYPE HTML>
<html xmlns:th="http://www.thymeleaf.org">
<head>
    <title>SpringBoot模版渲染</title>
    <meta http-equiv="Content-Type" content="text/html;charset=UTF-8"/>
</head>
<body>
    <p th:text="'用户ID：' + ${uid}"/>
    <p th:text="'用户名称：' + ${name}"/>
</body>
</html> 
```

Thymeleaf通过`${}`来获取model中的变量，注意这不是el表达式，而是ognl表达式，但是语法非常像。关于“${属性}”和“{属性}”的区别:${}访问完整信息，而{}访问指定对象中的属性内容， 如果访问的只是普通的内容两者没有区别。

Thymeleaf中所有的表达式都需要写在"指令"中，指令是HTML5中的自定义属性，在Thymeleaf中所有指令都是以`th:`开头。

当数据量比较多的时候，频繁的写`user.`就会非常麻烦。

因此，Thymeleaf提供了自定义变量来解决：

```html
<h2 th:object="${user}">
    <p>Name: <span th:text="*{name}">Jack</span>.</p>
    <p>Age: <span th:text="*{age}">21</span>.</p>
    <p>friend: <span th:text="*{friend.name}">Rose</span>.</p>
</h2>
```

Thymeleaf中提供了一些内置对象，并且在这些对象中提供了一些方法，方便我们来调用。获取这些对象，需要使用`#对象名`来引用。

| 对象            | 作用                                          |
| --------------- | --------------------------------------------- |
| #ctx            | 获取Thymeleaf自己的Context对象                |
| #requset        | 如果是web程序，可以获取HttpServletRequest对象 |
| #response       | 如果是web程序，可以获取HttpServletReponse对象 |
| #session        | 如果是web程序，可以获取HttpSession对象        |
| #servletContext | 如果是web程序，可以获取HttpServletContext对象 |

| 对象       | 作用                             |
| ---------- | -------------------------------- |
| #dates     | 处理java.util.date的工具对象     |
| #calendars | 处理java.util.calendar的工具对象 |
| #numbers   | 用来对数字格式化的方法           |
| #strings   | 用来处理字符串的方法             |
| #bools     | 用来判断布尔值的方法             |
| #arrays    | 用来护理数组的方法               |
| #lists     | 用来处理List集合的方法           |
| #sets      | 用来处理set集合的方法            |
| #maps      | 用来处理map集合的方法            |

有的时候，我们需要在指令中填写基本类型如：字符串、数值、布尔等，并不希望被Thymeleaf解析为变量，这个时候称为字面值。数字默认是字面值，使用一对`'`引用的内容是字符串字面值。

`${}`内部的是通过OGNL表达式引擎解析的，外部的才是通过Thymeleaf的引擎解析，因此运算符尽量放在`${}`外进行。比较不用<>，用gt，lt。

循环指令：th:each 

条件指令：th:if,th:unless

分支：th:switch,th:case



