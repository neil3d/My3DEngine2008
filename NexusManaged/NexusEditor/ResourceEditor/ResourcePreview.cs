using System;
using System.Windows.Forms;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// 资源的预览窗口基类
    /// </summary>
    class ResourcePreview : NEViewport
    {
        protected NLevel m_level;
        protected float m_lgtAnimRadius;
        protected NActor m_lgtActor;
        protected BoundingBox m_actorBox;
        private NGameTimer m_lgtTimer;

        public ResourcePreview(string levelName)
        {
            NEditorEngine eng = NexusEditor.Program.engine;
            m_level = eng.CreateLevel(levelName, "nlevel");
            m_level.Init("nplain_scene");

            float f = 200;
            m_actorBox = new BoundingBox(
                new Vector3(-f, -f, -f), new Vector3(f, f, f)
                );

            m_lgtTimer = new NGameTimer();
            m_lgtTimer.Reset();

            m_focusLevel = levelName;
        }

        protected override void Dispose(bool disposing)
        {
            NEditorEngine eng = NexusEditor.Program.engine;
            if (m_lgtActor != null)
            {
                m_lgtActor.Dispose();
                m_lgtActor = null;
            }
            eng.DestroyLevel(m_level);
            m_level.Dispose();
            base.Dispose(disposing);
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            if (m_cameraCtrl.InputMessgaeProc(ref m))
            {
                this.Refresh();
            }
        }

        public void AddPointLight()
        {
            string lgtActorName = "PointLight";
            lgtActorName += DateTime.Now.ToFileTime();

            NActor lgtActor = m_level.CreateActor(lgtActorName, "nactor");
            NActorComponent comp = lgtActor.CreateComponent("PoinLgt", "nlight_component");
            NLightComponent lgtComp = comp as NLightComponent;
            lgtComp.CreatePointLight();

            NPointLightProxy lgt = lgtComp.GetPointLight();
            lgt.AmbientColor = new Color4f(0.1f, 0.1f, 0.1f);
            lgt.DiffuseColor = new Color4f(0.6f, 0.6f, 0.6f);


            comp = lgtActor.CreateComponent("Box", "nshape_component");
            NShapeComponent boxComp = comp as NShapeComponent;
            Vector3 boxSize = m_actorBox.Maximum - m_actorBox.Minimum;
            float f = Math.Max(boxSize.x, boxSize.y);
            f = Math.Max(f, boxSize.z);
            f *= 0.02f;
            boxComp.CreateBox(f, f, f);

            lgtActor.Location = new Vector3(500, 500, 500);
            m_lgtActor = lgtActor;

            m_lgtActor.Visible = (this.ViewRenderMode == ERenderMode.Lit);
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

                m_actorBox = sptComp.GetBoundingBox();

                //-- add default light
                AddPointLight();
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
                AddPointLight();

                BoundingBox box = meshComp.GetBoundingBox();
                m_lgtAnimRadius = Vector3.Distance(box.Maximum, box.Minimum) * 0.5f;
            }
            catch (System.Exception e)
            {
                NexusEditor.Program.ShowException(e, "Static Mesh Preview Open FAILED!");
            }            
        }

        public void ShowAnimMesh(NResourceAnimMesh mesh, string sequanceName, bool loop)
        { 
            try
            {
                m_level.DestroyAllActors();

                //-- create a actor for mesh
                NActor meshActor = m_level.CreateActor("AnimMeshActor", "nactor");
                NActorComponent comp = meshActor.CreateComponent("MeshComp", "nanim_mesh_component");
                NAnimMeshComponent meshComp = comp as NAnimMeshComponent;
                meshComp.ResetResource(mesh);
                m_actorBox = meshComp.GetBoundingBox();

                if (sequanceName.Length > 0)
                    meshComp.PlayAnim(sequanceName, loop, 0);

                //-- add default light
                AddPointLight();

                BoundingBox box = meshComp.GetBoundingBox();
                m_lgtAnimRadius = Vector3.Distance(box.Maximum, box.Minimum) * 0.5f;
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Anim Mesh Preview Open FAILED!");
            }
        }

        private void AddDirectLight()
        { 
            try
            {
                NActor lgtActor = m_level.CreateActor("DefaultDirLgt", "nactor");
                NActorComponent comp = lgtActor.CreateComponent("DirLgtComp", "nlight_component");

                // todo
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

        public void AnimLight()
        {
            if(m_lgtActor==null)
                return;

            m_lgtTimer.Tick();
            float deltaTime = m_lgtTimer.DeltaTime;

            float r = this.m_actorBox.Maximum.Length();
            r = Math.Max(100, r*0.6f);

            float s = (float)Math.Sin(deltaTime);
            float c = (float)Math.Cos(deltaTime);

            m_lgtActor.Location = new Vector3(s * r, c * r, s * c * r);

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
    }
}
