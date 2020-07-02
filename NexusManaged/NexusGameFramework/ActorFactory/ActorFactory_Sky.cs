using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngine;

namespace Nexus.GameFramework
{
	class ActorFactory_Sky : ActorFactory
	{
		public ActorFactory_Sky()
		{
			this.nativeActorClassName = "nactor";
		}

		public override void PostCreateActor(NexusEngine.NActor actor)
		{
			base.PostCreateActor(actor);

			//创建显示用部件
			NResourceLoc res = new NResourceLoc("engine_data", "/resource_default/skydome.nmdl");
			NResourceStaticMesh mesh = NResourceManager.Instance.LoadStaticMesh(res,EResourceIOMode.Auto,EResourceIOPriority.Normal);
			if(mesh==null) return;
			actor.Frozen = true;
			NActorComponent comp = actor.CreateComponent("SkyComponent", "nsky_component");
			NSkyComponent meshComp = comp as NSkyComponent;
			meshComp.ResetResource(res);
		}
	}
}
