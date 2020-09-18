MySQL必知必会笔记



MySQL shell 命令

\sql    切换到sql，\js，切换到js

\connect root@localhost:3306    本地连接



```mysql
SHOW databases;  --查看数据库
USE databasename;
SHOW TABLES;	--查看表
```

建表及设计

```mysql
CREATE TABLE costum(
	id     int       NOT NULL AUTO_INCREMENT,
	name   char(50)  NOT NULL,
	age    int,
	male   boolean   DEFAULT 0;
	resume TEXT,
	PRIMARY KEY(id),           --主键可由多列组合，组合唯一即可     
	FULLTEXT(resume)
)ENGINE = MYIsAM;           --该引擎支持全文本搜索，不支持事务处理，InnoDB不支持全文搜索，支持事务处理

RENAME TABLE costom TO costomer;
ALTER TABLE costomer ADD province char(50);
ALTER TABLE costomer DROP COLUMN province;
DROP TABLE costom;
```



**查**

```mysql
SELECT username FROM user WHERE age>10;
SELECT * FROM user WHERE age>10;
SELECT username FROM user WHERE age BETWEEN 1 AND 8;
SELECT DISTINCT username FROM user;   --只查询不同的名字相当于取set
SELECT username FROM user LIMIT 2,5;  --从第2行开始查询5条记录
SELECT username FROM user ORDER BY age;  --排序，可按多列
SELECT username FROM user ORDER BY age DESC;  --降序排序

过滤
SELECT username FROM user WHERE age IS NULL;
SELECT username FROM user WHERE age  IN (1,2,3);
SELECT username FROM user WHERE age NOT IN (1,2,3);  --NOT只用于in，between，exists前

通配符过滤
SELECT username FROM user WHERE username LIKE 'wang%';
--%可匹配任意长字符，_匹配单个字符，通配符放在开头很慢
全文本搜索   性能更好
创建表单时用FULLTEXT(clolmn)指明需要进行全文本搜索的列，自动创建其中的每个词的索引
match(列) agains(搜索的字符串)

计算字段
SELECT Concat(username,'(',age,')') AS bieming FROM user;    --算数运算也可
SELECT RTrim(username) FROM user;
```

**常用的数据处理函数**

now(),date(),time(),year()

rtrim(),ltrim(),right()右边字符，lower转为小写，upper转为大写

**聚集函数和分组**

```
SELECT COUNT(*) FROM user GROUP BY age;  --分组计数
max,min,avg,sum
SELECT COUNT(*) FROM user GROUP BY age HAVING COUNT(*)>=2;
HAVING在分组过滤中相当于where
```

```
SELECT * FROM user WHERE username IN (SELECT name FROM othersheet);--查询可以嵌套

--联结
SELECT * FROM table1,table2 WHERE table1.id = table2.id;   --不用where联结会返回两个表的笛卡尔积  

--组合查询结果
SELECT username FROM table1 UNION SELECT username FROM table2    --两个结果集结构要相同

```

**增**

```mysql
INSERT INTO user(id,Username) VALUES((SELECT MAX(id) FROM user)+1,'xiaoming');
--若无null字段也可不写列名
--插入多条记录
INSERT INTO user(id,username) VALUES(1,'xiaoming'),(2,'xiaohong');
--插入查询结果集
INSERT INTO user(username) SELECT student_name FROM students ;

```

删

```mysql
DELETE FROM WHERE ...
```

改

```mysql
UPDATE user SET username = 'xiaowang', age = '20' WHERE id = 5;
```

在一次更改多条记录时若不要原子性可在update后加ignore

视图

虚拟的表，由查询得到的结果集而来，一般只用于查询，以重用SQL语句，不对视图进行增删改

CREATE VIEW XXX AS SELECT......

DROP VIEW XXX

触发器

增删改操作的之前或后所触发的语句

例如，在每插入一条记录之后查询新插入的用户名

CREATE TRIGGER trigger1 AFTER INSERT ON user FOR EACH ROW SELECT NEW.username INTO @newuser;

删除之前把待删除的记录转到另一个表

CREATE TRIGGER trigger2 BEFORE DELETE ON user FOR EACH ROW INSERT INTO olduser(username,age)

VALUES(OLD.username,OLD.age);

事务处理

保证成批的操作要么全部执行要么全部不执行

```mysql
--开始一个事务
START TRANSACTION;
方式一使用rollback
DELETE FROM user WHERE id=1;
ROLLBACK;  --回退全部操作
方式二使用commit
事务中要手动提交
DELETE FROM user WHERE id=1;
DELETE FROM user WHERE id=2;
COMMIT;   --如果前面两条操作没有全部成功则不会提交
方式三 使用savepoint
DELETE FROM user WHERE id=1;
SAVEPOINT delete1;
DELETE FROM user WHERE id=2;
ROLLBACK TO delete1;
```

