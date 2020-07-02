using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{  
    /// <summary>
    /// 使用static mesh等基本资源快速向level editor中创建actor
    /// </summary>
    class ResourceActorCreator
    {
        static public NActor CreateActor(List<NResourceLoc> resList)
        {
            string actorName = "ResourceActor_";
            actorName += DateTime.Now.ToFileTime();

            NLevel mainLv = NLevelEditorEngine.Instance.MainLevel;
            NActor newActor = mainLv.CreateActor(actorName, "nactor");
            foreach (NResourceLoc res in resList)
            {
                CreateActorComponent(newActor, res);
            }            

            return newActor;
        }

        static public NActor CreateActor(NResourceLoc resLoc)
        {
            string actorName = "ResourceActor_";
            actorName += DateTime.Now.ToFileTime();

            NLevel mainLv = NLevelEditorEngine.Instance.MainLevel;
            NActor newActor = mainLv.CreateActor(actorName, "nactor");
            CreateActorComponent(newActor, resLoc);                       

            return newActor;
        }

        static public bool AcceptResoruceFile(NResourceLoc res)
        {
            if (!res.IsValid())
                return false;
            
            bool accept = false;
            switch (res.FileExtension)
            { 
                case "spt":
                case "nmdl":
                case "nam":
                    accept = true;
                    break;
            }

            return accept;
        }

        static private void CreateActorComponent(NActor newActor, NResourceLoc res)
        {
            try
            {
                switch (res.FileExtension)
                {
                    case "nmdl":
                        {
                            NActorComponent comp = newActor.CreateComponent(
                                MakeComponentName("StaticMesh_", res),
                                "nstatic_mesh_component");
                            NStaticMeshComponent meshComp = comp as NStaticMeshComponent;
                            meshComp.ResetResource(res);
                        }
                        break;
                    case "spt":
                        {
                            NActorComponent comp = newActor.CreateComponent(
                                MakeComponentName("SPTComp_", res),
                                "nspeed_tree_component");
                            NSpeedTreeComponent sptComp = comp as NSpeedTreeComponent;
                            sptComp.Create(res);
                        }
                        break;
                    case "nam":
                        {
                            NActorComponent comp = newActor.CreateComponent(
                                MakeComponentName("AnimMeshComp_", res),
                                "nanim_mesh_component");
                            NAnimMeshComponent meshComp = comp as NAnimMeshComponent;
                            meshComp.ResetResource(res);
                        }
                        break;
                }// end of switch
            }
            catch (System.Exception e)
            {
                NexusEditor.Program.ShowException(e, "Create Resource Actor Component FAILED!");
            }            
        }

        static private string MakeComponentName(string prefix, NResourceLoc res)
        {
            string ret = prefix + res.ToString();
            ret = ret.Replace(':', '_');
            return ret.Replace('/', '_');            
        }
    }
}
