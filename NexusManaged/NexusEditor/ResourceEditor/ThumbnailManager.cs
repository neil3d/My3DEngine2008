using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using System.Drawing;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{    
    /// <summary>
    /// 在内存中缓存所有使用过的缩略图
    /// </summary>
    class ThumbnailManager
    {
        //-- Singleton Pattern - http://msdn.microsoft.com/en-us/library/ms998558.aspx
        private static readonly ThumbnailManager s_instance = new ThumbnailManager();        
        public static ThumbnailManager Instance
        {
            get { return s_instance; }
        }

        //--
        private ImageList m_bigThumbCache;
        private ImageList m_smallThumbCache;

        private ThumbnailManager() 
        {
            m_bigThumbCache = new ImageList();
            m_bigThumbCache.ImageSize = new Size(256, 256);
            m_bigThumbCache.ColorDepth = ColorDepth.Depth24Bit;

            m_smallThumbCache = new ImageList();
            m_smallThumbCache.ImageSize = new Size(48, 48);
            m_smallThumbCache.ColorDepth = ColorDepth.Depth24Bit;
        }

        public string LoadThumbnail(string pkgName, string fileName, ImageList bigIL, ImageList smallIL)
        {
            string thumbName = string.Format("{0}:{1}", pkgName, fileName);
            //-- find in cache
            { 
                int i = m_bigThumbCache.Images.IndexOfKey(thumbName);
                if (i != -1)
                {
                    if (bigIL != null)
                        bigIL.Images.Add(thumbName, m_bigThumbCache.Images[i]);

                    i = m_smallThumbCache.Images.IndexOfKey(thumbName);
                    if (smallIL != null)
                        smallIL.Images.Add(thumbName, m_smallThumbCache.Images[i]);

                    return thumbName;
                }                
            }

            //--
            Size bigSize = m_bigThumbCache.ImageSize;
            Size smallSize = m_smallThumbCache.ImageSize;

            IntPtr hbmp = NLevelEditorEngine.Instance.CreateThumbnail(
                    new NResourceLoc(pkgName, fileName), bigSize.Width, bigSize.Height
                    );


            Image bigImg = Image.FromHbitmap(hbmp);
            m_bigThumbCache.Images.Add(thumbName, bigImg);
            m_smallThumbCache.Images.Add(thumbName, bigImg);

            if (bigIL!=null)
                bigIL.Images.Add(thumbName, bigImg);
            if(smallIL!=null)
                smallIL.Images.Add(thumbName, bigImg);

            return thumbName;
        }
    }
}
