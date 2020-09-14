# Spring概览

## 两大特性

（1）**IOC(控制反转)或DI（依赖注入）:**明确定义组件的接口，独立开发各个组件，然后根据组件的依赖关系组装运行；即将创建及管理对象的权利交给Spring容器。Spring是一个轻型容器(light-weight Container)，其核心Bean工厂(Bean Factory)，用以构造我们所需要的M(Model）。能够让相互协作的软件组件保持松散耦合。降低了业务对象替换的复杂性，提高了组件之间的解耦。

（2）**AOP(面向切面编程)**：通过预编译方式和运行期动态代理实现在不修改源代码的情况下给程序动态统一添加功能的一种技术。即系统级的服务从代码中解耦出来。例如：将日志记录，性能统计，安全控制，事务处理，异常处理等代码从业务逻辑代码中划分出来。允许你把遍布应用各处的功能分离出来形成可重用组件。

## 核心组件

### 1.bean

Bean组件主要解决：Bean 的定义、Bean 的创建以及对 Bean 的解析。开发者关心Bean创建，其他由Spring内部帮你完成。Bean 的创建时典型的工厂模式，他的顶级接口是 BeanFactory，下图是这个工厂的继承层次关系。

![img](https://timgsa.baidu.com/timg?image&quality=80&size=b9999_10000&sec=1599888592891&di=40764695f46a32fb101a8cda0e409310&imgtype=0&src=http%3A%2F%2Fimage.codes51.com%2FArticle%2Fimage%2F20171008%2F20171008000739_1458.png)

Bean 的定义就是完整的描述了在 Spring 的配置文件中定义的 <bean/> 节点中所有的信息，包括各种子节点。

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="
        http://www.springframework.org/schema/beans 
        http://www.springframework.org/schema/beans/spring-beans.xsd">
<!-- 基础配置 -->
<bean id="userDao" class="com.example.demo....."></bean>
</beans>
```

当 Spring 成功解析你定义的一个 <bean/> 节点后，在 Spring 的内部他就被转化成 BeanDefinition 对象。以后所有的操作都是对这个对象完成的。

bean 的解析过程非常复杂，功能被分的很细，因为这里需要被扩展的地方很多，必须保证有足够的灵活性，以应对可能的变化。Bean 的解析主要就是对 Spring 配置文件的解析。

### 2.Context 

 Context 在 Spring 的 org.springframework.context 包下，给 Spring 提供一个运行时的环境，用以保存各个对象的状态。ApplicationContext 是 Context 的顶级父类，他除了能标识一个应用环境的基本信息外，他还继承了五个接口，这五个接口主要是扩展了 Context 的功能。ApplicationContext 继承了 BeanFactory，这也说明了 Spring 容器中运行的主体对象是 Bean，另外 ApplicationContext 继承了 ResourceLoader 接口，使得 ApplicationContext 可以访问到任何外部资源。下面是 Context 的类结构图：![image-20200912105054264](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20200912105054264.png)

应用上下文（Application Context）负责装载bean的定义，并把它们组装起来，即装载配置文件。Spring应用上下文全权负责对象的组装。spring自带多种应用上下文的实现，它们之间的区别仅仅在于如何加载配置。
a、AnnotationConfigApplicationContext：从一个或多个基于Java配置类中加载Spring应用上下文。
b、AnnotationConfigWebApplicationContext：从一个或多个基于java的配置类中加载Spring Web应用上下文。
c、ClassPathXmlApplicationContext：从类路径下的一个或多个xml配置文件中加载上下文定义，把应用上下文定义文件作为类资源。
d、FileSystemXMLApplicationContext：从文件系统下的一个或多个xml配置文件中加载上下文定义。
e、xmlWebApplicationContext：从Web应用的一个或多个xml配置文件中加载上下文定义。

总体来说 ApplicationContext 必须要完成以下几件事：

  （1）标识一个应用环境。

  （2）利用 BeanFactory 创建 Bean 对象。

  （3）保存对象关系表。

  （4）能够捕获各种事件。