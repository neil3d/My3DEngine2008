using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using NexusEngine;

namespace Nexus.GameFramework
{
	class ActorFactory_PointLight : ActorFactory
	{
		public ActorFactory_PointLight()
        {
			this.nativeActorClassName = "nactor";
        }

		public override void PostCreateActor(NexusEngine.NActor actor)
		{
			base.PostCreateActor(actor);

			//创建光源部件
			actor.CreateComponent("PointLightComponent", "npoint_light_component");

			//创建显示用部件
			NResourceLoc res = new NResourceLoc("engine_data", "/editor_res/ball.nmdl");
			NActorComponent comp = actor.CreateComponent("LightSignBoard", "nstatic_editor_mesh_component");
			NStaticMeshComponent meshComp = comp as NStaticMeshComponent;
			meshComp.ResetResource(res);
		}
	}
}
