using System;
using System.Windows.Forms;
using System.Drawing;
using NexusEngine;

namespace NexusEditor.EditorMain
{
    /// <summary>
    /// 编辑器主窗口的viewport
    /// </summary>
    public class NEMainViewport : NEViewport
    {
        public NEMainViewport()
        {
            NLevelEditorEngine.Instance.LevelChanged += new EventHandler(Engine_LevelChanged);

            m_focusLevel = NLevelEditorEngine.Instance.MainLevelName;

            this.AllowDrop = true;
            this.DragOver += new DragEventHandler(NEMainViewport_DragOver);
            this.DragDrop += new DragEventHandler(NEMainViewport_DragDrop);
            this.DragEnter += new DragEventHandler(NEMainViewport_DragEnter);

            m_view.CreateHitHash();
        }

        #region RayCheck

        /// <summary>
        /// 鼠标射线检测
        /// </summary>
        /// <param name="hit">检测的结果</param>
        /// <param name="mousePosition">鼠标位置的屏幕坐标</param>
        /// <param name="dist">射线检测的长度</param>
        /// <param name="checkType">检测碰撞类型</param>
        /// <returns>如何发生碰撞则返回true</returns>
        public void MouseRayCheck(out NCheckResult hit, Point mousePosition, float dist, LineCheckType checkType)
        {
            //Point clientPt = this.PointToClient(mousePosition);
			Point clientPt = mousePosition;
            Ray ray = m_view.Camera.GetMouseRay(clientPt.X, clientPt.Y);
            if(!RayCheck(out hit, ray, dist, checkType))
            {
                hit.dist = 500;
                hit.location = ray.Position + ray.Direction * hit.dist;
            }
        }

        /// <summary>
        /// 射线检测
        /// </summary>
        /// <param name="hit">检测的结果</param>
        /// <param name="ray">射线</param>
        /// <param name="dist">距离</param>
        /// <param name="checkType">类型</param>
        /// <returns>是否发生碰撞</returns>
        public bool RayCheck(out NCheckResult hit, Ray ray, float dist, LineCheckType checkType)
        {
            NLevel mainLv = NLevelEditorEngine.Instance.MainLevel;
            Vector3 lineEnd = ray.Position + ray.Direction * dist;
            return mainLv.LineCheck(out hit, ray.Position, lineEnd, checkType);
        }
        #endregion

        /// <summary>
        /// 响应拖放
        /// </summary>        
        void NEMainViewport_DragEnter(object sender, DragEventArgs e)
        {
            if (!e.Data.GetDataPresent(typeof(NexusEngine.NFileEntity)))
            {
                return;
            }
        }

        /// <summary>
        /// 拖放结束事件, Resource Quick List中拖动一个Mesh资源文件,创建响应的Actor到Level中
        /// </summary>        
        void NEMainViewport_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(typeof(NexusEngine.NFileEntity)))
            {
                NFileEntity resFile = (NFileEntity)e.Data.GetData(typeof(NFileEntity));
                NResourceLoc resLoc = new NResourceLoc(resFile.PackageName, resFile.FilePath);

                if (ResourceEditor.ResourceActorCreator.AcceptResoruceFile(resLoc))
                {
                    NLevel mainLv = NLevelEditorEngine.Instance.MainLevel;
                    NActor newActor = ResourceEditor.ResourceActorCreator.CreateActor(resLoc);
                    if (newActor == null)
                    {
                        MessageBox.Show("添加对象失败，可能的原因是读取对象数据失败，或者当前作业层被锁定,等等", "错误");
                        return;
                    }
                    Point scrPt = new Point(e.X, e.Y);
                    Point clientPt = this.PointToClient(scrPt);
                    Ray ray = m_view.Camera.GetMouseRay(clientPt.X, clientPt.Y);
                    NCheckResult chk;
                    if (RayCheck(out chk, ray, 512 * 1024, LineCheckType.Terrain))
                    {
                        newActor.Location = chk.location;
                    }
                    else
                    {

                        Vector3 pt = ray.Position + ray.Direction * 500;
                        newActor.Location = pt;
                    }
                }
            }
        }

        /// <summary>
        /// 拖放相关事件, 指示自己能够接受拖放
        /// </summary>        
        void NEMainViewport_DragOver(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(typeof(NexusEngine.NFileEntity)))
            {
                e.Effect = DragDropEffects.Copy;
            }
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }

        void Engine_LevelChanged(object sender, EventArgs e)
        {
            m_focusLevel = NLevelEditorEngine.Instance.MainLevelName;
        }

        /// <summary>
        /// 重载消息处理函数
        /// </summary>
        /// <remarks>
        /// 根据当前的操作模式，将消息派发给Camera Controller或者TerrainEd等对象
        /// </remarks>
        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);

            //-- 先由Editor处理，根据返回值，看是否还交由CameraCtrl处理
            int ret = NLevelEditorEngine.Instance.HandleViewportMessage(ref m, this.m_view);
            switch (ret)
            {
                case 1:
                    this.Refresh();
                    break;
                case 2:
                    if (m_cameraCtrl.InputMessgaeProc(ref m))
                        this.Refresh();
                    break;
            }//end of switch
        }

        protected override NEWidgetsRender CreateWidgetsRender()
        {
            return new NELevelEditorWidgetsRender();
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);

            switch (e.KeyCode)
            {
                case Keys.Home: // 按Home键,将镜头移动到选定的对象
                    {
                        NActorEditor actorEd = NLevelEditorEngine.Instance.ActorEd;
                        if (actorEd.SelectedActor != null)
                        {
                            if (m_cameraCtrl is NPerspectiveCameraController)
                            {
                                NPerspectiveCameraController pcam = m_cameraCtrl as NPerspectiveCameraController;
                                pcam.SetLookat(actorEd.SelectedActor.Location);
                                this.Refresh();
                            }
                        }
                    }
                    break;
            }
        }
    }// end of class NEMainViewport
}
