using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NexusEngine;

namespace NexusEditor
{
    public delegate void TimelineStreamCheckEvent(int streamIndex, bool oldValue, bool currentValue);

    public abstract class KeyPoint
    {
        protected float m_Time;
        public virtual float Time
        {
            get
            {
                return m_Time;
            }
            set
            {
                m_Time = value;
            }
        }
		public abstract uint GetStreamCount();
		public abstract float GetValue(uint index);
		public abstract void SetValue(uint index, float value);
    }

	public class FloatKeyPoint : KeyPoint
	{
		public float m_Value;
		public override uint GetStreamCount()
		{
			return 1;
		}
		public override float GetValue(uint index)
		{
			return m_Value;
		}
		public override void SetValue(uint index,float value)
		{
			m_Value = value;
		}
	}

	public class Vector2KeyPoint : KeyPoint
	{
		public Vector2 m_Value;
		public override uint GetStreamCount()
		{
			return 2;
		}
		public override float GetValue(uint index)
		{
			switch (index)
			{
			case 0:
					return m_Value.x;
			case 1:
					return m_Value.y;
			default:
					throw new System.ArgumentException("Vector2KeyPoint::GetValue()::索引越界");
			}
		}
		public override void SetValue(uint index, float value)
		{
			switch (index)
			{
				case 0:
					m_Value.x=value;
					break;
				case 1:
					m_Value.y=value;
					break;
				default:
					return;
			}
		}
	}

	public class Vector3KeyPoint : KeyPoint
	{
		public Vector3 m_Value;
		public override uint GetStreamCount()
		{
			return 3;
		}
		public override float GetValue(uint index)
		{
			switch (index)
			{
				case 0:
					return m_Value.x;
				case 1:
					return m_Value.y;
				case 2:
					return m_Value.z;
				default:
					throw new System.ArgumentException("Vector2KeyPoint::GetValue()::索引越界");
			}
		}
		public override void SetValue(uint index, float value)
		{
			switch (index)
			{
				case 0:
					m_Value.x = value;
					break;
				case 1:
					m_Value.y = value;
					break;
				case 2:
					m_Value.z = value;
					break;
				default:
					return;
			}
		}
	}

	public class ColorKeyPoint : KeyPoint
	{
		public Color4f m_Value;
		public override uint GetStreamCount()
		{
			return 4;
		}
		public override float GetValue(uint index)
		{
			switch (index)
			{
				case 0:
					return m_Value.R;
				case 1:
					return m_Value.G;
				case 2:
					return m_Value.B;
				case 3:
					return m_Value.A;
				default:
					throw new System.ArgumentException("Vector2KeyPoint::GetValue()::索引越界");
			}
		}
		public override void SetValue(uint index, float value)
		{
			switch (index)
			{
				case 0:
					m_Value.R = value;
					break;
				case 1:
					m_Value.G = value;
					break;
				case 2:
					m_Value.B = value;
					break;
				case 3:
					m_Value.A = value;
					break;
				default:
					return;
			}
		}
	}


    public class KeyPointComparer : IComparer<KeyPoint>
    {
        public int Compare(KeyPoint x, KeyPoint y)
        {
            if (x == null)
            {
                if (y == null)
                {
                    return 0;
                }
                else
                {
                    return -1;
                }
            }
            else
            {
                if (y == null)
                {
                    return 1;
                }
                else
                {
                    return x.Time.CompareTo(y.Time);
                }
            }
        }
    }


    //时间轴列表项目
    public abstract class TimelineStreams
    {
        public TimelineStreamCheckEvent StreamCheckEvent;

		private string m_Name;
        public string Name
        {
            get
            {
                return m_Name;
            }
            set
            {
                m_Name = value;
            }
        }
        private bool m_Enable;
        public bool Enable
        {
            get
            {
                return m_Enable;
            }
            set
            {
                m_Enable = value;
            }
        }
        //获取数据流的数量
        public abstract int GetStreamCount(); 

        //获取指定的数据流
        public abstract List<KeyPoint> GetStream();

        //获取指定数据流的代表色
        public abstract Color GetStreamColor(int index);

        //获取指定数据流的名称
        public abstract string GetStreamName(int index);

        //指定数据流是否激活
        public abstract bool GetStreamEnable(int index);

        //激活或者冻结指定的数据流
        public abstract void SetStreamEnable(int index, bool value);

        //将指定节点的数据更新到数据源
		public abstract void Update(KeyPoint kp);

        //为指定数据流添加指定时间的数据节点
        public abstract void AddKeyPoint(float time);

        //删除指定的数据点
        public abstract void RemoveKeyPoint(int index);

        //按照时间从小到大的排序
        public virtual void Sort()
        {
            KeyPointComparer kpc = new KeyPointComparer();
            GetStream().Sort(kpc);
        }
    }

    public class FloatCurve : TimelineStreams
    {
        private List<KeyPoint> m_Value;
        private bool m_StreamEnable;
        NDistributionFloatCurve m_DataSource;

        public FloatCurve()
        {
            m_Value = new List<KeyPoint>();
            m_StreamEnable = true;
        }

        public void Bind(NDistributionFloatCurve curve)
        {
            m_DataSource = curve;
            GetStream().Clear();
            for (int i = 0; i < m_DataSource.KeyCount; ++i)
            {
                KeyPoint kp = new FloatKeyPoint();
                kp.Time = m_DataSource.GetTime(i);
				kp.SetValue(0, m_DataSource.GetValue(i));
                m_Value.Add(kp);
            }
        }

        public override int GetStreamCount()
        {
            return 1;
        }

        public override List<KeyPoint> GetStream()
        {
            return m_Value;
        }

        public override Color GetStreamColor(int index)
        {
            return Color.LightSkyBlue;
        }

        public override string GetStreamName(int index)
        {
            return Name + " Value";
        }

        public override bool GetStreamEnable(int index)
        {
            return m_StreamEnable;
        }

        public override void SetStreamEnable(int index, bool value)
        {
            bool old = m_StreamEnable;
            m_StreamEnable = value;
            if (StreamCheckEvent != null)
            {
                StreamCheckEvent(0, old, value);
            }
        }

        public override void Update(KeyPoint kp)
        {
			int i = m_Value.IndexOf(kp);
			if (i != -1)
			{
				m_DataSource.SetValue(i, kp.Time, kp.GetValue(0));
			}
        }

        public override void AddKeyPoint(float time)
        {
            float v = m_DataSource.GetValue(time);
            m_DataSource.AddKey(v,time);
            FloatKeyPoint kp = new FloatKeyPoint();
			kp.m_Value = v;
            kp.Time = time;
            m_Value.Add(kp);
            KeyPointComparer kpc = new KeyPointComparer();
            m_Value.Sort(kpc);
        }

        public override void RemoveKeyPoint(int index)
        {
            m_DataSource.RemoveKey(index);
            m_Value.RemoveAt(index);
        }
    }

    public class VectorCurve2 : TimelineStreams
    {
        private List<KeyPoint> m_Value;
        private Color[] m_Color;
        private bool[] m_StreamEnable;
        NDistributionVector2Curve m_DataSource;

        public VectorCurve2()
        {
            m_Value = new List<KeyPoint>();
            m_Color = new Color[2];
            m_Color[0] = Color.Red;
            m_Color[1] = Color.Blue;
            m_StreamEnable = new bool[2];
            for (int i = 0; i < 2; ++i)
            {
                m_StreamEnable[i] = true;
            }
        }

        public void Bind(NDistributionVector2Curve curve)
        {
			m_Value.Clear();
            m_DataSource = curve;
			for (int j = 0; j < m_DataSource.KeyCount; ++j)
			{				
				Vector2 v = m_DataSource.GetValue(j);
				Vector2KeyPoint kp = new Vector2KeyPoint();
				kp.Time = m_DataSource.GetTime(j);
				kp.m_Value = v;				
				m_Value.Add(kp);
			}
        }

        public override int GetStreamCount()
        {
            return 2;
        }

        public override List<KeyPoint> GetStream()
        {
            return m_Value;
        }

        public override Color GetStreamColor(int index)
        {
            return m_Color[index];
        }

        public override string GetStreamName(int index)
        {
            switch (index)
            {
            case 0:
                    return Name + " X";
            case 1:
                    return Name + " Y";
            case 2:
                    return Name + " Z";
            case 3:
                    return Name + " W";
            default:
                    return Name + " Value";
            }
        }

        public override bool GetStreamEnable(int index)
        {
            return m_StreamEnable[index];
        }

        public override void SetStreamEnable(int index, bool value)
        {
            bool old = m_StreamEnable[index];
            m_StreamEnable[index] = value;
            if (StreamCheckEvent != null)
            {
                StreamCheckEvent(index, old, value);
            }
        }

        public override void Update(KeyPoint kp)
        {
			int i = m_Value.IndexOf(kp);
			if (i != -1)
			{
				Vector2KeyPoint vkp = kp as Vector2KeyPoint;
				m_DataSource.SetValue(i, kp.Time, vkp.m_Value);
			}
        }

        public override void AddKeyPoint(float time)
        {
            Vector2 v=m_DataSource.GetValue(time);
            m_DataSource.AddKey(v, time);
			Vector2KeyPoint kp = new Vector2KeyPoint();
            kp.m_Value = v;
            kp.Time = time;
            m_Value.Add(kp);
            KeyPointComparer kpc = new KeyPointComparer();
            m_Value.Sort(kpc);
        }

        public override void RemoveKeyPoint(int index)
        {
            m_DataSource.RemoveKey(index);
            m_Value.RemoveAt(index);
        }
    }

    public class VectorCurve3 : TimelineStreams
    {
        private List<KeyPoint> m_Value;
        private Color[] m_Color;
        private bool[] m_StreamEnable;
        NDistributionVector3Curve m_DataSource;

        public VectorCurve3()
        {
            m_Value = new List<KeyPoint>();
            m_Color = new Color[3];
            m_Color[0] = Color.Red;
            m_Color[1] = Color.Green;
            m_Color[2] = Color.Blue;
            m_StreamEnable = new bool[3];
            for (int i = 0; i < 3; ++i)
            {
                m_StreamEnable[i] = true;
            }
        }

        public void Bind(NDistributionVector3Curve curve)
        {
            m_DataSource = curve;
            GetStream().Clear();
			for (int j = 0; j < m_DataSource.KeyCount; ++j)
            {
				Vector3KeyPoint kp = new Vector3KeyPoint();
                kp.Time = m_DataSource.GetTime(j);
                kp.m_Value = m_DataSource.GetValue(j);
                m_Value.Add(kp);
            }
        }

        public override int GetStreamCount()
        {
            return 3;
        }

        public override List<KeyPoint> GetStream()
        {
            return m_Value;
        }

        public override Color GetStreamColor(int index)
        {
            return m_Color[index];
        }

        public override string GetStreamName(int index)
        {
            switch (index)
            {
                case 0:
                    return Name + " X";
                case 1:
                    return Name + " Y";
                case 2:
                    return Name + " Z";
                case 3:
                    return Name + " W";
                default:
                    return Name + " Value";
            }
        }

        public override bool GetStreamEnable(int index)
        {
            return m_StreamEnable[index];
        }

        public override void SetStreamEnable(int index, bool value)
        {
            bool old = m_StreamEnable[index];
            m_StreamEnable[index] = value;
            if (StreamCheckEvent != null)
            {
                StreamCheckEvent(index, old, value);
            }
        }

        public override void Update(KeyPoint kp)
        {
			int i = m_Value.IndexOf(kp);
			if (i != -1)
			{
				Vector3KeyPoint vkp = kp as Vector3KeyPoint;
				m_DataSource.SetValue(i, kp.Time, vkp.m_Value);
			}
        }

        public override void AddKeyPoint(float time)
        {
			Vector3 v = m_DataSource.GetValue(time);
			m_DataSource.AddKey(v, time);
			Vector3KeyPoint kp = new Vector3KeyPoint();
			kp.m_Value = v;
			kp.Time = time;
			m_Value.Add(kp);
			KeyPointComparer kpc = new KeyPointComparer();
			m_Value.Sort(kpc);
        }

        public override void RemoveKeyPoint(int index)
        {
            m_DataSource.RemoveKey(index);
            m_Value.RemoveAt(index);
        }
    }

    public class ColorCurve : TimelineStreams
    {
        //色彩的四个分量，顺序：argb，值域[0,1]
        private List<KeyPoint> m_Value;
        private Color[] m_Color;
        private bool[] m_StreamEnable;
        private NDistributionColorCurve m_DataSource;

        public ColorCurve()
        {
            m_Value = new List<KeyPoint>();
            m_Color = new Color[4];
            m_Color[3] = Color.LightSkyBlue;
            m_Color[0] = Color.Red;
            m_Color[1] = Color.Green;
            m_Color[2] = Color.Blue;
            m_StreamEnable = new bool[4];
            for (int i = 0; i < 4; ++i)
            {
                m_StreamEnable[i] = true;
            }
        }

        public void Bind(NDistributionColorCurve curve)
        {
			m_DataSource = curve;
			GetStream().Clear();
			for (int j = 0; j < m_DataSource.KeyCount; ++j)
			{
				ColorKeyPoint kp = new ColorKeyPoint();
				kp.Time = m_DataSource.GetTime(j);
				kp.m_Value = m_DataSource.GetValue(j);
				m_Value.Add(kp);
			}
        }

        public override int GetStreamCount()
        {
            return 4;
        }

        public override List<KeyPoint> GetStream()
        {
            return m_Value;
        }

        public override Color GetStreamColor(int index)
        {
            return m_Color[index];
        }

        public override string GetStreamName(int index)
        {
            switch (index)
            {
                case 0:
                    return Name + " Red";
                case 1:
                    return Name + " Green";
                case 2:
                    return Name + " Blue";
                case 3:
                    return Name + " Alpha";
                default:
                    return Name + " Value";
            }
        }

        public override bool GetStreamEnable(int index)
        {
            return m_StreamEnable[index];
        }

        public override void SetStreamEnable(int index, bool value)
        {
            bool old = m_StreamEnable[index];
            m_StreamEnable[index] = value;
            if (StreamCheckEvent != null)
            {
                StreamCheckEvent(index, old, value);
            }
        }

		public override void Update(KeyPoint kp)
		{
			int i = m_Value.IndexOf(kp);
			if (i != -1)
			{
				ColorKeyPoint vkp = kp as ColorKeyPoint;
				m_DataSource.SetValue(i, kp.Time, vkp.m_Value);
			}
		}

		public override void AddKeyPoint(float time)
		{
			Color4f v = m_DataSource.GetValueByTime(time);
			m_DataSource.AddKey(v, time);
			ColorKeyPoint kp = new ColorKeyPoint();
			kp.m_Value = v;
			kp.Time = time;
			m_Value.Add(kp);
			KeyPointComparer kpc = new KeyPointComparer();
			m_Value.Sort(kpc);
		}

		public override void RemoveKeyPoint(int index)
		{
			m_DataSource.RemoveKey(index);
			m_Value.RemoveAt(index);
		}
    }
}
