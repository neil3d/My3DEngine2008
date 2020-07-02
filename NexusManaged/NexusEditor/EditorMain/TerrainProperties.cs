using System;
using System.ComponentModel;
using System.Windows;
using NexusEngine;

namespace NexusEditor.EditorMain
{
    /// <summary>
    /// 地形产生Noise时的参数设置
    /// </summary>
    public class NETerrainNoise
    {
        private ETrnBurshTarget target;
        private uint targetIndex;
        private Int32Rect rect;
        private int octaves;
        private float amplitude;
        private float frequency;        

        public NETerrainNoise()
        {
            rect = new Int32Rect(0, 0, -1, -1);
            octaves = 8;
            amplitude = 32;
            frequency = 2;
        }

        [Category("Generate Noise"), Description("目标图层类型")]
        public ETrnBurshTarget Target
        {
            get { return target; }
            set { target = value; }
        }

        [Category("Generate Noise"), Description("目标图层索引(Alpha图层,Decot图层)")]
        public uint TargetIndex
        {
            get { return targetIndex; }
            set { targetIndex = value; }
        }

        [Category("Generate Noise"), Description("作用的矩形区域")]
        public Int32Rect AreaRect
        {
            get { return rect; }
            set { rect = value; }
        }

        [Category("Generate Noise"), Description("迭代次数，数字越大变化越连续")]
        public int NumOctaves
        {
            get { return octaves; }
            set { octaves = value;}
        }

        [Category("Generate Noise"), Description("振幅")]
        public float Amplitude
        {
            get { return amplitude; }
            set { amplitude = value; }
        }

        [Category("Generate Noise"), Description("频率")]
        public float Frequency
        {
            get { return frequency; }
            set { frequency = value; }
        }
    }

    /// <summary>
    /// 地形创建时的参数集合
    /// </summary>
    public class NETerrainProp
    {
        private string name;
        private uint width;
        private uint height;
        private ushort initH;
        private uint chunkSize;
        private Vector3 scale;
        private Vector3 pos;

        public NETerrainProp()
        {
            name = "Terrain_" + DateTime.Now.ToFileTime();
            width = 257;
            height = 257;
            chunkSize = 65;
            scale = new Vector3(128, 2, 128);
            pos = new Vector3(0, 0, 0);
        }

        [Category("Heightmap"), Description("请为地形对象启一个便于辨认的名称")]
        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        [Category("Heightmap"), Description("高度图的宽度")]
        public uint Width
        {
            get { return width; }
            set { width = value; }
        }

        [Category("Heightmap"), Description("高度图的高度")]
        public uint Height
        {
            get { return height; }
            set { height = value; }
        }

        [Category("Heightmap"), Description("高度图的初始值(0到65535)")]
        public ushort Init
        {
            get { return initH; }
            set { initH = value; }
        }

        [Category("Heightmap"), Description("地形分块的大小")]
        public uint ChunkSize
        {
            get { return chunkSize; }
            set { chunkSize = value; }
        }

        [Category("Property"), Description("高度缩放系数")]
        public Vector3 Scale
        {
            get { return scale; }
            set { scale = value; }
        }

        [Category("Property"), Description("地形的起始位置")]
        public Vector3 Position
        {
            get { return pos; }
            set { pos = value; }
        }
    }

    public class NETerrainMaterialProp
    {
        private NResourceLoc basicTextureLoc;
        private int m_alphaWidth;
        private int m_alphaHeight;
        private Vector2 detailUVScale;

        public NETerrainMaterialProp()
        {
            m_alphaWidth = -1;
            m_alphaHeight = -1;
            detailUVScale = new Vector2(16, 16);
        }

        [Category("TerrainMaterial"), Description("简单材质模式的贴图文件路径")]
        public NResourceLoc BasicTexture
        {
            get { return basicTextureLoc; }
            set { basicTextureLoc = value; }
        }

        [Category("TerrainMaterial"), Description("Alpha图层宽度")]
        public int AlphaMapWidth
        {
            get { return m_alphaWidth; }
            set { m_alphaWidth = value; }
        }

        [Category("TerrainMaterial"), Description("Alpha图层高度")]
        public int AlphaMapHeight
        {
            get { return m_alphaHeight; }
            set { m_alphaHeight = value; }
        }
    }

    /// <summary>
    /// 地形多层贴图材质的属性
    /// </summary>
    public class NETerrainTextureLayerProp
    {
        public LayerData data;

        public NETerrainTextureLayerProp()
        {
            data.DiffuseTexture = new NResourceLoc();
            data.NormalTexture = new NResourceLoc();
            data.UVScale = new Vector2(10, 10);
            data.UVRotate = 0;
            data.Specular = 0;
            data.SpecularPower = 32;
        }

        [Category("TextureLayer"), Description("贴图文件路径")]
        public NResourceLoc TextureRes
        {
            get { return data.DiffuseTexture; }
            set { data.DiffuseTexture = value; }
        }

        [Category("TextureLayer"), Description("法线贴图文件路径")]
        public NResourceLoc NormalRes
        {
            get { return data.NormalTexture; }
            set { data.NormalTexture = value; }
        }

        [Category("TextureLayer"), Description("贴图UV缩放,数字越大重复次数越多")]
        public Vector2 UVScale
        {
            get { return data.UVScale; }
            set { data.UVScale = value; }
        }

        [Category("TextureLayer"), Description("贴图旋转系数")]
        public float UVRotate
        {
            get { return data.UVRotate; }
            set { data.UVRotate = value; }
        }

        [Category("TextureLayer"), Description("高光强度，数字越大高光强度越大")]
        public float Specular
        {
            get { return data.Specular; }
            set { data.Specular = value; }
        }

        [Category("TextureLayer"), Description("高光系数，数字越大光斑越小")]
        public float SpecularPower
        {
            get { return data.SpecularPower; }
            set { data.SpecularPower = value; }
        }
    }
}
