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

        [Category("Generate Noise"), Description("Generate target")]
        public ETrnBurshTarget Target
        {
            get { return target; }
            set { target = value; }
        }

        [Category("Generate Noise"), Description("Generate target index")]
        public uint TargetIndex
        {
            get { return targetIndex; }
            set { targetIndex = value; }
        }

        [Category("Generate Noise"), Description("Noise rect")]
        public Int32Rect AreaRect
        {
            get { return rect; }
            set { rect = value; }
        }

        [Category("Generate Noise"), Description("Num octaves of Perlin noise")]
        public int NumOctaves
        {
            get { return octaves; }
            set { octaves = value;}
        }

        [Category("Generate Noise"), Description("Amplitude of Perlin noise")]
        public float Amplitude
        {
            get { return amplitude; }
            set { amplitude = value; }
        }

        [Category("Generate Noise"), Description("Frequency of Perlin noise")]
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
            name = "DefaultTerrainName";
            width = 129;
            height = 129;
            chunkSize = 17;
            scale = new Vector3(32, 32, 32);
            pos = new Vector3(0, 0, 0);
        }

        [Category("Heightmap"), Description("Terrain Actor Name")]
        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        [Category("Heightmap"), Description("Heightmap width, in pixel.")]
        public uint Width
        {
            get { return width; }
            set { width = value; }
        }

        [Category("Heightmap"), Description("Heightmap height, in pixel.")]
        public uint Height
        {
            get { return height; }
            set { height = value; }
        }

        [Category("Heightmap"), Description("Heightmap initial unscaled height value.")]
        public ushort Init
        {
            get { return initH; }
            set { initH = value; }
        }

        [Category("Heightmap"), Description("Chunk size of quad tree node.")]
        public uint ChunkSize
        {
            get { return chunkSize; }
            set { chunkSize = value; }
        }

        [Category("Property"), Description("Scale of heightmap.")]
        public Vector3 Scale
        {
            get { return scale; }
            set { scale = value; }
        }

        [Category("Property"), Description("Position of terrain actor.")]
        public Vector3 Position
        {
            get { return pos; }
            set { pos = value; }
        }
    }

    class NETerrainBasicMtlProp
    {
        private NResourceLoc textureLoc;

        [Category("Terrain Basic Material"), Description("Texture resource location.")]
        public NResourceLoc DiffuseTextureRes
        {
            get { return textureLoc; }
            set { textureLoc = value; }
        }
    }

    /// <summary>
    /// 地形多层贴图材质的属性
    /// </summary>
    public class NETerrainTextureLayerProp
    {
        private NResourceLoc textureLoc;
        private Vector2 uvScale;
        private float uvRotate;

        public NETerrainTextureLayerProp()
        {
            textureLoc = new NResourceLoc();
            uvScale = new Vector2(10, 10);            
        }

        [Category("TextureLayer"), Description("Texture resource location.")]
        public NResourceLoc TextureRes
        {
            get { return textureLoc; }
            set { textureLoc = value;}
        }

        [Category("TextureLayer"), Description("Texture uv scale.")]
        public Vector2 UVScale
        {
            get { return uvScale; }
            set { uvScale = value; }
        }

        [Category("TextureLayer"), Description("Texture uv rotate.")]
        public float UVRotate
        {
            get { return uvRotate; }
            set { uvRotate = value; }
        }
    }
}
