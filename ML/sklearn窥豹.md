**sklearn窥豹**

Sklearn主要模块

- Classification 分类
- Regression 回归
- Clustering 聚类
- Dimensionality reduction 降维
- Model Selection 模型选择
- Preprocessing 数据预处理



sklearn中的核心API是**估计器**，任何可以基于数据集对一些参数进行估计的对象都被称为估计器。

fit(x,y):传入数据以及标签即可训练模型，训练的时间和参数设置，数据集大小以及数据本身的特点有关

score(x,y)用于对模型的正确率进行评分(范围0-1)。但由于对在不同的问题下，评判模型优劣的的标准不限于简单的正确率，可能还包括召回率或者是查准率等其他的指标，特别是对于类别失衡的样本，准确率并不能很好的评估模型的优劣，因此在对模型进行评估时，不要轻易的被score的得分蒙蔽。

predict(x)用于对数据的预测，它接受输入，并输出预测标签，输出的格式为numpy数组。我们通常使用这个方法返回测试的结果，再将这个结果用于评估模型。

创建估计器：需要设置一组超参数，在创建好的估计器 model 可以直接访问这些超参数，用 . 符号。

拟合估计器：拟合之后可以访问 model 里学到的参数，比如线性回归里的特征前的系数 coef，或 K 均值里聚类标签 labels

**转换器**转换器也是一种估计器，用于对数据的处理，例如标准化、降维以及特征选择等等。同与估计器的使用方法类似:

fit(x,y):该方法接受输入和标签，计算出数据变换的方式。

transform(x):根据已经计算出的变换方式，返回对输入数据x变换后的结果（不改变x）

fit_transform(x,y) :该方法在计算出数据变换方式之后对输入x就地转换。

两大类转换器：将分类型变量 (categorical) 编码成数值型变量 (numerical)，规范化 (normalize) 或标准化 (standardize) 数值型变量

Sklearn 里核心 API 接口是估计器，那高级 API 接口就是元估计器 (meta-estimator)，即由很多基估计器 (base estimator) 组合成的估计器，一些多分类估计器。





**Model Selection **

常见的 Model Selection 估计器有以下几个：

cross_validate: 评估交叉验证的表现。

learning_curve: 建立学习曲线。  

GridSearchCV: 用交叉验证从网格中一组超参数搜索出最佳超参数。

RandomizedSearchCV 用交叉验证从一组随机超参数搜索出最佳超参数。

交叉验证：

```python
lr = LinearRegression()
cvscore = cross_val_score(lr,x,y,cv=5,scoring="r2")
cvpre = cross_val_predict(lr,x,y,cv=5)
```

cv是我们使用的交叉验证的生成器或者迭代器，它决定了交叉验证的数据是如何划分的，当cv的取值为整数的时候，使用(Stratified)KFold方法。

也可也创建自己的cv，如下所示：

```python
from sklearn.model_selection import ShuffleSplit
my_cv = ShuffleSplit(n_splits=3, test_size=0.3, random_state=0)
```

cross_val_predict 和 cross_val_score的使用方法是一样的，但是它返回的是一个使用交叉验证以后的输出值，而不是评分标准。假如数据划分为[1,2,3,4,5]份，它先用[1,2,3,4]训练模型，计算出来第5份的目标值，然后用[1,2,3,5]训练模型计算出第4份的目标值，直到都结束为止。



```python
from sklearn.linear_model import LinearRegression
from sklearn.datasets import load_boston
from sklearn.model_selection import train_test_split,cross_val_score,cross_val_predict
from sklearn import preprocessing
from sklearn.metrics import r2_score
import joblib

dataset = load_boston()
x = dataset.data
y = dataset.target
x_train,x_test,y_train,y_test = train_test_split(x,y,train_size=0.7)

# 归一化
x_train = preprocessing.minmax_scale(x_train)
x_test = preprocessing.minmax_scale(x_test)
y_train = preprocessing.minmax_scale(y_train)
y_test = preprocessing.minmax_scale(y_test)
x = preprocessing.minmax_scale(x)
y = preprocessing.minmax_scale(y)

#训练模型并用留出法评估
lr = LinearRegression()
lr.fit(x_train,y_train)
y_pre = lr.predict(x_test)
score = r2_score(y_test,y_pre)
print(score)
print("coef",lr.coef_)
print("intercept:",lr.intercept_)

#交叉验证
cvscore = cross_val_score(lr,x,y,cv=5,scoring="r2")
cvpre = cross_val_predict(lr,x,y,cv=5)
score2 = r2_score(y,cvpre)

#保存模型
joblib.dump(lr,"C:/Users/Administrator/Desktop/model/lr.pkl")
# lr = joblib.load("C:/Users/Administrator/Desktop/model/lr.pkl")
```

