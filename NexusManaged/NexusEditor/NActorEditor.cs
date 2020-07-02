using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using NexusEngine;

namespace NexusEditor
{
    public class ActorSelectedArgs : EventArgs
    {
        private NActor m_selected;

        public ActorSelectedArgs(NActor selectedActor)
        {
            m_selected = selectedActor;
        }

        public NActor SelectedActor
        {
            get { return m_selected; }
        }
    }

    public class NActorEditor : NexusEngine.NActorEditorBase
    {
        public event EventHandler ActorSelected;

        public override void BindActor(NActor ac)
        {
            base.BindActor(ac);
            if (ActorSelected != null)
                ActorSelected(this, new ActorSelectedArgs(ac));

            if (ac is NTerrainActor)
            {
                NLevelEditorEngine.Instance.TerrainEd.BindTerrain(ac as NTerrainActor);
            }
        }
        public NActor SelectedActor
        {
            get { return this.GetActor(); }
            set { this.BindActor(value); }
        }
    }
}
