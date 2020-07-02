using System;
using System.Drawing;
using System.Windows.Forms;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// 资源的预览窗口基类
    /// </summary>
    internal class ResourcePreview : NEViewport
    {
        protected NLevel m_level;        
        protected NActor m_lgtActor;
        protected NActorComponent m_resActorComp;
        protected BoundingBox m_actorBox;
        private NGameTimer m_lgtTimer;
        
        private bool m_draging;
        private Point m_dragPoint; 

        public ResourcePreview(string levelName)
        {
            NEditorEngine eng = NexusEditor.Program.engine;
            m_level = eng.CreateLevel(levelName, "nlevel");
            m_level.Init();

            RenderSetting rs = m_level.GetRenderSetting();
            rs.SkyLightHightColor = new Color4f(0.4f, 0.4f, 0.4f);
            rs.SkyLightLowColor = new Color4f(0.3f, 0.3f, 0.3f);

            float f = 200;
            m_actorBox = new BoundingBox(
                new Vector3(-f, -f, -f), new Vector3(f, f, f)
                );

            m_lgtTimer = new NGameTimer();
            m_lgtTimer.Reset();

            m_focusLevel = levelName;
            
            m_draging = false;
            this.MouseDown += new MouseEventHandler(ResourcePreview_MouseDown);
            this.MouseUp += new MouseEventHandler(ResourcePreview_MouseUp);
            this.MouseMove += new MouseEventHandler(ResourcePreview_MouseMove);
        }

        void ResourcePreview_MouseMove(object sender, MouseEventArgs e)
        {
            if ((NWin32API.GetKeyState(NWin32API.VK_MENU) & 0x80) != 0)
            {
                if (e.Button == MouseButtons.Left)
                {
                    const float rotateFactor = 0.5f;
                    int deltaX = e.X - m_dragPoint.X;
                    int deltaY = e.Y - m_dragPoint.Y;
                    m_dragPoint = e.Location;

                    if (m_lgtActor != null)
                    {
                        Vector3 rot = m_lgtActor.Rotation;
                        rot.x += deltaX * rotateFactor;
                        rot.y += deltaY * rotateFactor;
                        m_lgtActor.Rotation = rot;

                        this.Refresh();
                    }
                }
            }
        }

        void ResourcePreview_MouseUp(object sender, MouseEventArgs e)
        {
            m_draging = false;
        }

        void ResourcePreview_MouseDown(object sender, MouseEventArgs e)
        {
            m_draging = true;
            m_dragPoint = e.Location;
        }

        public NLevel Level
        {
            get { return m_level; }
        }


        protected override void Dispose(bool disposing)
        {
            NEditorEngine eng = NexusEditor.Program.engine;
            if (m_lgtActor != null)
            {
                m_lgtActor.Dispose();
                m_lgtActor = null;
            }
            if (m_resActorComp != null)
            {
                m_resActorComp.Dispose();
                m_resActorComp = null;
            }
            eng.DestroyLevel(m_level);
            m_level.Dispose();
            base.Dispose(disposing);
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            if ( (NWin32API.GetKeyState(NWin32API.VK_MENU) & 0x80) == 0)
            {
                if (m_cameraCtrl.InputMessgaeProc(ref m))
                {
                    this.Refresh();
                }
            }
        }


        public void LoadSpeedTree(NResourceLoc loc)
        {
            m_level.DestroyAllActors();

            //--
            try
            {
                NActor sptActor = m_level.CreateActor("SptPreviewActor", "nactor");
                NSpeedTreeComponent sptComp = sptActor.CreateComponent("SPTComp", "nspeed_tree_component") as NSpeedTreeComponent;
                sptComp.Create(loc);

                sptActor.Scale = new Vector3(50, 50, 50);
                m_actorBox = sptComp.GetBoundingBox();

                //-- add default light
                AddDefaultLight();

                m_resActorComp = sptComp;
            }
            catch (System.Exception e)
            {
                NexusEditor.Program.ShowException(e, "Speed Tree Load FAILED!");
            }
        }

        public void LoadStaticMesh(NResourceLoc loc)
        {
            m_level.DestroyAllActors();

            try
            {
                NResourceStaticMesh res =
                    NResourceManager.Instance.LoadStaticMesh(loc, 
                    EResourceIOMode.Block, EResourceIOPriority.Normal);
                this.ShowStaticMesh(res);

            }
            catch (System.Exception e)
            {
                NexusEditor.Program.ShowException(e, "Static Mesh Preview Load FAILED!");
            }
        }

        public void ShowSpecialEffect(NResourceSpecialEffect sfx)
        { 
            try
            {
                m_level.DestroyAllActors();

                //-- create a actor for SFX
                NActor sfxActor = m_level.CreateActor("SFXActor", "nactor");
                NActorComponent comp = sfxActor.CreateComponent("SFXComp", "nspecial_effect_instance");
                NSpecialEffectInstance sfxComp = comp as NSpecialEffectInstance;                
                sfxComp.ResetResource(sfx);
                sfxComp.Play();

                m_resActorComp = sfxComp;
            }
            catch (System.Exception e)
            {
                NexusEditor.Program.ShowException(e, "SFX Preview Open FAILED!");
            }
        }

		public NShapeComponent ShowSphereShapeComponent(float radius, int numRings, int numSections)
		{
			try
			{
				m_level.DestroyAllActors();

				//-- create a actor for mesh
				NActor meshActor = m_level.CreateActor("StaticMeshActor", "nactor");
				NActorComponent comp = meshActor.CreateComponent("MeshComp", "nshape_component");
				NShapeComponent meshComp = comp as NShapeComponent;
				meshComp.CreateSphere(radius,numRings,numSections);
				m_actorBox = meshComp.GetBoundingBox();

				//-- add default light
                AddDefaultLight();
				
				m_resActorComp = comp;

				return meshComp;
			}
			catch (System.Exception e)
			{
				NexusEditor.Program.ShowException(e, "Static Mesh Preview Open FAILED!");
				return null;
			}
		}

        public void ShowStaticMesh(NResourceStaticMesh mesh)
        {
            try
            {
                m_level.DestroyAllActors();

                //-- create a actor for mesh
                NActor meshActor = m_level.CreateActor("StaticMeshActor", "nactor");
                NActorComponent comp = meshActor.CreateComponent("MeshComp", "nstatic_mesh_component");
                NStaticMeshComponent meshComp = comp as NStaticMeshComponent;
                meshComp.ResetResource(mesh);
                m_actorBox = meshComp.GetBoundingBox();

                //-- add default light
                AddDefaultLight();                
            }
            catch (System.Exception e)
            {
                NexusEditor.Program.ShowException(e, "Static Mesh Preview Open FAILED!");
            }            
        }

        //public void ShowAnimMesh(NResourceAnimMesh mesh, string sequanceName, bool loop)
        //{ 
        //    try
        //    {
        //        m_level.DestroyAllActors();

        //        //-- create a actor for mesh
        //        NActor meshActor = m_level.CreateActor("AnimMeshActor", "nactor");
        //        NActorComponent comp = meshActor.CreateComponent("MeshComp", "nanim_mesh_component");
        //        NAnimMeshComponent meshComp = comp as NAnimMeshComponent;
        //        meshComp.ResetResource(mesh);
        //        m_actorBox = meshComp.GetBoundingBox();

        //        if (sequanceName.Length > 0)
        //        {
        //            meshComp.SetAnim(sequanceName);
        //            meshComp.PlayAnim(loop, 1.0f, 0.0f);
        //        }

        //        //-- add default light
        //        AddPointLight();

        //        m_resActorComp = comp;
        //        BoundingBox box = meshComp.GetBoundingBox();
        //        m_lgtAnimRadius = Vector3.Distance(box.Maximum, box.Minimum);
        //    }
        //    catch (System.Exception ex)
        //    {
        //        NexusEditor.Program.ShowException(ex, "Anim Mesh Preview Open FAILED!");
        //    }
        //}

        public void AnimLight()
        { 
        }

        internal virtual void AddDefaultLight()
        { 
            try
            {
                NActor lgtActor = m_level.CreateActor("DefaultDirLgt", "nactor");
                NActorComponent comp = lgtActor.CreateComponent("DirLgtComp", "ndirectional_light_component");
                NDirectionalLightComponent lgtComp = comp as NDirectionalLightComponent;
                lgtComp.bProjectShadow = true;
                lgtActor.Rotation = new Vector3(45, 45, 0);

                m_lgtActor = lgtActor;
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Create Directional Light FAILED!");
            }
        }

        public void Reset()
        {
            m_level.DestroyAllActors();
            this.Refresh();
        }

        public void ZoomExtents()
        {
            m_cameraCtrl.ZoomExtents(m_actorBox, m_view.Camera);
            this.Refresh();
        }

        /// <summary>
        /// 覆盖基类,增加"非Lit渲染模式隐藏灯光actor"的操作
        /// </summary>
        public new ERenderMode ViewRenderMode
        {
            get { return base.ViewRenderMode; }
            set
            {
                if (m_lgtActor != null)
                    m_lgtActor.Visible = (value == ERenderMode.Lit);
                base.ViewRenderMode = value;                
            }
        }

        public void ShowResource(NResourceLoc resLoc)
        {
            switch (resLoc.FileExtension)
            {
                case "spt":
                    this.LoadSpeedTree(resLoc);
                    break;
                case "nmdl":
                    this.LoadStaticMesh(resLoc);
                    break;
                case "nam":                    
                    break;
            }
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);

            switch (e.KeyCode)
            {
                case Keys.Home:
                    this.ZoomExtents();
            	break;
            }
        }

        public NActorComponent ResourceActorComponent
        {
            get { return m_resActorComp; }
        }
    }
}
