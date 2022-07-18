## 实现过程中碰到的其他问题与解决办法

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

![image-20220718110304528](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20220718110304528.png)

裁剪区域的边角点也需要考虑，最后划分出新的三角形，蓝色区域划分出两个新三角形

![image-20220718110412528](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20220718110412528.png)

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

