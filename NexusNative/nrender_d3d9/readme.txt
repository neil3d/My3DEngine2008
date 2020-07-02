使用D3D9, Shader Model 3实现的渲染系统<插件>

.关于Scene Depth
	由于Depth of Field、大气散射等效果都需要访问场景的z值，所以需要Depth Texture。但是D3D9访问depth texture返回的是与
	当前的z指比较的结果。虽然nvidia提供了FOURCC format--INTZ（geforce 8x）,RAWZ(geforce 7x)，ATI提供了DF16/DF24但是使用
	起来还是有很多不方便的地方。所以最后决定使用一个R32F的格式来保存linear depth--这也是CryEngine2的方案。
	float3 m=float3(0.996093809371817670572857294849, 0.0038909914428586627756752238080039, 1.5199185323666651467481343000015e-5);
	float d = dot(tex2D(depthSampler, depthUV).arg, m);