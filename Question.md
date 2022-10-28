## 实现过程中碰到的其他问题与解决办法

### 为什么使用齐次坐标

因为软光栅涉及到向量以及矩阵的几何变换，主要包括平移、旋转、缩放。以矩阵表达式来计算这些变换时，平移是矩阵相加，旋转和缩放则是矩阵相乘，综合起来可以表示为p'= m1*p + m2(m1旋转缩放矩阵， m2为平移矩阵， p为原向量，p'为变换后的向量)。引入齐次坐标的目的主要是合并矩阵运算中的乘法和加法，表示为p' =M*p的形式。即它提供了用矩阵运算把二维、三维甚至高维空间中的一个点集从一个坐标系变换到另一个坐标系的有效方法。　　它可以表示无穷远的点。n+1维的齐次坐标中如果h=0，实际上就表示了n维空间的一个无穷远点。对于齐次坐标[a,b,h]，保持a,b不变，|**V**|=（x1*x1，y1*y1,z1*z1)^1/2的过程就表示了标准坐标系中的一个点沿直线ax+by=0 逐渐走向无穷远处的过程。

### 为什么需要透视校正

在使用光栅化的图形学方法中，法线，颜色，纹理坐标这些属性通常是绑定在图元的顶点上的。在3D空间中，这些属性值在图元上应该是线性变化的。但是当3D顶点被透视投影到2D屏幕之后，如果在2D投影面上对属性值进行线性插值，其对应的属性在3D空间中却不是线性变化的。

### 为什么需要切线空间

实际上，法线本身存储在哪个坐标系中都是可以的，例如存储在World Space、或者Object Space、或者Tangent Space中。但问题是，我们并不是单纯的想要得到法线，后续的光照计算才是我们的目的。不管使用哪个坐标系，都面临着一个选择，就是**最后光照计算使用的坐标系究竟是哪个**。对于Tangent-Space Normal Map，我们一般就是在Tangent Space里计算的，也就是说，我们需要**把viewDir、lightDir在Vertex Shader中转换到Tangent Space中，然后在Fragment Shader对法线纹理采样后，直接进行光照计算**。 

​        **Tangent-Space还有如下一些优点：**

- - **自由度很高**。Tangent-Space Normal Map记录的是相对法线信息，这意味着，即便把该纹理应用到一个完全不同的网格上，也可以得到一个合理的结果。
  - **可进行UV动画**。比如，我们可以移动一个纹理的UV坐标来实现一个凹凸移动的效果，这种UV动画在水或者火山熔岩这种类型的物体会会用到。
  - **可以重用Normal Map**。比如，一个砖块，我们可以仅使用一张Normal Map就可以用到所有的六个面上。
  - **可压缩**。由于Tangent-Space Normal Map中法线的Z方向总是正方向的，因此我们可以仅存储XY方向，而推导得到Z方向。

### 使用MVP变换怎么实现背面剔除

首先MVP矩阵思路是采用摄像机始终在原点不变原则，朝向-Z，up为Y轴

所以要实现背面剔除，可以直接使用摄像机位置如（0,0,10)，只要在Z轴正向就没问题

然后将模型原始坐标点点乘model变换矩阵，就可以得出模型经过平移、旋转、缩放后的矩阵，然后通过计算三角形面的法向量与摄像机位置到三角形该平面任意一点的射线的点积，判断正负即可知道该面是否需要剔除。

```c++
bool Rasterizer::IsBackFaceCulling(const std::array<Maths::Vector3f, 3> &vecs) {
    Maths::Vector3f vec1 = (vecs[1] - vecs[0]);
    Maths::Vector3f vec2 = (vecs[2] - vecs[1]);
    Maths::Vector3f view_direction = vecs[1]; //摄像机默认设置在原点
    Maths::Vector3f n = vec1.Cross(vec2);
    if (view_direction.Dot(n) > 0.0f)
        return true;

    return false;
}
```

### 多边形裁剪

由于使用多边形裁剪需要考虑该种情况，图中蓝色区域

[Source of image-20220718110304528.png - SoftRender - 祖龙Bitbucket (zulong.com)](http://git.zulong.com/users/jiangdaming/repos/softrender/browse/QuestionImage/裁剪区域.png)

裁剪区域的边角点也需要考虑，最后划分出新的三角形，蓝色区域划分出两个新三角形

[Source of image-20220718110412528.png - SoftRender - 祖龙Bitbucket (zulong.com)](http://git.zulong.com/users/jiangdaming/repos/softrender/browse/QuestionImage/三角形划分.png)

由于考虑到点的插入顺序，会影响三角形的划分，所以在无序插入情况下，需要先对点集进行顺时针或者逆时针排序，然后划分三角形。

点集顺时针排序算法

可以使用该点与X轴正向夹角的大小对不同点排序，该算法需要对每一个点计算atan()，时间复杂度可能较高。简单测试结果是 atan()计算比加减乘除计算耗时多20倍。所以采用其他算法对点集顺时针排序，算法如下：

```c++
Maths::Vector3f MathUtil::center;

Maths::Vector3f MathUtil::GetCenterPoint(const std::vector<VertexData> &vecs) {
    float xSum = 0;
    float ySum = 0;
    for (int i = 0; i < vecs.size(); ++i) {
        xSum += vecs[i].vertex.x;
        ySum += vecs[i].vertex.y;
    }
    return Maths::Vector3f{xSum / ((float)vecs.size()), ySum / ((float)vecs.size()), 0};
}

bool MathUtil::LessFunction(const VertexData &lhs, const VertexData &rhs) {
    if (lhs.vertex.x - center.x >= 0 && rhs.vertex.x - center.x < 0)
        return true;
    if (lhs.vertex.x - center.x < 0 && rhs.vertex.x - center.x >= 0)
        return false;
    if (lhs.vertex.x - center.x == 0 && rhs.vertex.x - center.x == 0) {
        if (lhs.vertex.y - center.y >= 0 || rhs.vertex.y - center.y >= 0)
            return lhs.vertex.y > rhs.vertex.y;
        return rhs.vertex.y > lhs.vertex.y;
    }
    float det = (lhs.vertex.x - center.x) * (rhs.vertex.y - center.y) - (rhs.vertex.x - center.x) * (lhs.vertex.y - center.y);
    if (det < 0)
        return true;
    else if (det > 0)
        return false;
    float d1 = (lhs.vertex.x - center.x) * (lhs.vertex.x - center.x) + (lhs.vertex.y - center.y) * (lhs.vertex.y - center.y);
    float d2 = (rhs.vertex.x - center.x) * (rhs.vertex.x - center.x) + (rhs.vertex.y - center.y) * (rhs.vertex.y - center.y);
    return d1 > d2;
}
void MathUtil::SortPoint(std::vector<VertexData> &vecs) {
    center = GetCenterPoint(vecs);
    std::sort(vecs.begin(), vecs.end(), LessFunction);
}
```

