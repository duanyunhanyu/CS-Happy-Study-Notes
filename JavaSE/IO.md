## IO流四大家族

---

- java.io.InputStream		字节输入流
- java.io.OutputStream	字节输出流
- java.io.Reader 		字符输入流
- java.io.Writer 		字符输出流

四大家族首领都是抽象类

所有流都实现了java.io.Closeable接口，都是可关闭的，都有close()方法

所有输出流都实现了java.io.Flushable接口，都是可刷新的，都有flush()方法，将管道中的数据全部写到硬盘里



## FileInputStream

---

文件字节输入流，万能的，任何类型的文件都可以采用这个流来读

一次读取一个字节，内存和硬盘交互太频繁，耗费时间

```java
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

public class FileTest {
    public static void main(String[] args) {
        //创建文件字节输入流对象
        FileInputStream fis = null;
        try {
            fis = new FileInputStream("D:\\Java Source\\test.txt");     //文件中："abcde"
            int readData = 0;
            while((readData = fis.read()) != -1){	//巧妙的while循环判断
                System.out.println(readData);       //97 98 99 100 101
            }
        } catch (FileNotFoundException e) {			//自动生成的异常处理
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            //在finall语句块中确保流已经关闭
            if (fis != null) {      //避免空指针异常
                try {
                    fis.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
```



#### read方法

read方法不加参数时一次读取一个字节，还可以使用byte数组接受读取的字节

一次最多读取b.length 个字节，减少IO次数提高执行效率，往byte[]数组当中读，返回值为读取到的字节数，未读到返回-1

注意byte数组读取为覆盖，例如，当文件为"abcde"，byte数组大小为4，读取一次为"abcd"，第二次为"ebcd"。

```java
//依葫芦画瓢
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

public class FileTest {
    public static void main(String[] args) {
        //创建文件字节输入流对象
        FileInputStream fis = null;
        try {
            fis = new FileInputStream("D:/Java Source/test.txt");     //文件中："abcde"
           byte[] bytes = new byte[4];
           int readCount = 0;
           while((readCount = fis.read(bytes)) != -1) {
               //把byte数组转化为字符串，读到多少个转换多少个
               System.out.print(new String(bytes,0,readCount));
           }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            //在finall语句块中确保流已经关闭
            if (fis != null) {      //避免空指针异常
                try {
                    fis.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
```

#### 其它方法

```java
FileInputStream fis = new FileInputStream(path);
int fileLength = fis.avaliable();		//返回该文件剩余可读取的长度

//用法，不适用于大文件
byte[] bytes = new byte(fis.avaliable());
int readCount = fis.read(bytes);
System.out.println(new String(bytes));

fis.skip(n);		//跳过n个字节
```



## FileOutputStream

---

文件字节输出流，负责写

```java
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public class FileTest {
    public static void main(String[] args) {
        FileOutputStream fos = null;
        try {
            fos = new FileOutputStream("D:/Java Source/test.txt",true);
            //文件不存在会新建，末尾true表示在文件末尾以追加的方式写入，不会先清空文件
            //开始写
            String s = "啦啦啦";
            byte[] bytes = s.getBytes();        //将字符串转换为byte数组
            fos.write(bytes);
            //写完之后一定要刷新
            fos.flush();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally{
            if (fos != null) {
                try {
                    fos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
```



#### 实例，文件拷贝

使用字节流输入输出可以拷贝所有类型的文件

```java
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public class FileTest {
    public static void main(String[] args) {
        FileInputStream fis = null;
        FileOutputStream fos = null;
        try {
            //创建输入流对象
            fis = new FileInputStream("D:/相册/毕业照/12346454.JPG");
            //创建输出流对象
            fos = new FileOutputStream("D:/相册/12346454.JPG");
            //核心：一边读一边写
            byte[] bytes = new byte[1024*100];      //一次拷贝100kB
            int readCount = 0;
            while((readCount = fis.read(bytes)) != -1){
                fos.write(bytes,0,readCount);       //读了多少写多少
            }
            //输出流最后要刷新
            fos.flush();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (fos != null) {
                try {
                    fos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (fis == null) {
                try {
                    fis.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
```



## BufferedReader和BufferedWriter

---

#### BufferedReader用法

```java
import java.io.BufferedReader;
import java.io.FileReader;

public class Test {
    public static void main(String[] args) throws Exception {
        FileReader reader = new FileReader("D:\\Java Source\\studynote\\src\\Test.java");
        //当一个流的构造方法中需要一个流的时候，这个被传进来的流叫做节点流，外部负责包装的流叫包装流（处理流）
        BufferedReader br = new BufferedReader(reader);     //该方法只能传字符流
        //读一行
        String s = null;
        while((s = br.readLine()) != null)
            System.out.println(s);      //readLine方法读取一个文本行，但不带换行符
        br.close();
    }
}
```

#### 关于流的转换：

```java
//字节流
FileInputStream in = new FileInputStream(path);

//通过转换流转换 字节流->字符流
InputStreamReader reader = new InputStreamReader(in);

//这个构造方法只能传一个字符流，不能传字节流
BufferedReader br = new BufferedReader(reader);

//合并书写
BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(path)));
```

#### BufferedWriter用法

```java
import java.io.BufferedWriter;
import java.io.FileWriter;

public class Test {
    public static void main(String[] args) throws Exception {
        BufferedWriter bw = new BufferedWriter(new FileWriter("output",true));
        bw.write("hello world \n");
        bw.flush();			//记得刷新
        bw.close();
    }
}
```



## 标准输出流

---

java.io.PrintStream:标准字节输出流，默认输出到控制台

```java
System.out.println("hello world");
//System.out 返回一个PrintStream

PrintStream ps = System.out;
ps.println("hello world");
//标准输出流不需要关闭
```

改变输出方向

```java
import java.io.FileOutputStream;
import java.io.PrintStream;

public class Test {
    public static void main(String[] args) throws Exception {
        //标准输出流不再指向控制台，指向log文件
        PrintStream printStream = new PrintStream(new FileOutputStream("log",true));
        //修改输出方向
        System.setOut(printStream);
        //再输出
        System.out.println("hello world");
    }
}
```



## java.io.File类

---

- File类和IO四大家族没有关系，不能完成文件读和写，File代表文件和路径名的抽象表示形式
- 一个File对象有可能对应的是目录，有可能是文件

#### File类常用方法

```java
import java.io.File;

public class Test {
    public static void main(String[] args) throws Exception{
        //创建File对象
        File f1 = new File("D:\\Java Source\\test");
        //判断是否存在
        System.out.println(f1.exists());
        //以文件形式创建
        if(!f1.exists()){
            //以文件形式新建
            f1.createNewFile();
            //以目录形式新建
            //f1.mkdirs();
        }
        //获取文件父路径
        String parentPath = f1.getParent();
        File parentFile = f1.getParentFile();
        //获取绝对路径
        String absolutePath = parentFile.getAbsolutePath();
        //获取文件名
        String fileName = f1.getName();
        //判断是否是文件或目录
        f1.isFile();
        f1.isDirectory();
        //获取最后一次修改的时间戳
        long time = f1.lastModified();
        //获取文件大小，单位字节
        f1.length();
        //获取当前目录下的所有子文件
        File[] files = f1.listFiles();
    }
}
```



## 序列化和反序列化

- 序列化Serialize:  java对象存储到文件中，将java对象的状态保存下来的过程
- 反序列化Deserialize: 将硬盘上的数据重新恢复到内存中，恢复成java对象
- 参与序列化和反序列化的对象必须实现Serializeable接口
- Serializeable接口只是一个标志接口，接口中什么代码都没有，起到一个标识的作用，java虚拟机看到了这个类实现了这个接口，会对该类自动生成一个序列化版本号
- 类的属性中加入**transient**关键字，则该属性不会参与序列化。反序列化出来为null

```java
public class Students implements Serializable{...}
```

```java
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

public class Test {
    public static void main(String[] args) throws Exception{
        //序列化
        Students s = new Students();
        ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("path"));
        oos.writeObject(s);		//也可以传入一个对象的集合
        oos.flush();
        oos.close();

        //反序列化
        ObjectInputStream ois = new ObjectInputStream(new FileInputStream("path"));
        Object obj = ois.readObject();		//也可以返回一个list
        ois.close();
    }
}
```



#### 序列化版本号

- java中首先通过类名来确定一个类，如果类名不一样，肯定不是一个类，如果类名一样，则使用序列化版本号进行区分。
- 不同的人编写了同一个类，就可以用序列化版本号来区分
- 当序列化版本号是自动生成的时候，如果类的代码进行过修改，系统会重新编译生成全新的序列化版本号，认为类发生了改变。
- 凡是类实现了Serializable接口，建议提供固定的序列化版本号

```java
public class Students implements Serializable{
    private ststic final long serialVersionUID = 777777777777L;
}
```

