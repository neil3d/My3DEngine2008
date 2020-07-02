using System;
using System.ComponentModel;
using NexusEngine;

namespace NexusEditor.EditorMain
{
    class EditorOptionsBasic
    {
        [Category("Editor Engine"), Description("Max Undo command")]
        public int UndoStackSize
        {
            get
            {
                return NLevelEditorEngine.Instance.CommandMgr.StackSize;
            }
            set
            {
                NLevelEditorEngine.Instance.CommandMgr.StackSize = value;
            }
        }

        [Category("鼠标操作"), Description("鼠标移动与三维空间的系数,数字越大拖动等操作越快")]
        public float MouseMoveFactor
        {
            get
            {
                return NLevelEditorEngine.Instance.ActorEd.MouseMoveFactor;
            }
            set 
            {
                NLevelEditorEngine.Instance.ActorEd.MouseMoveFactor = value;
            }
        }

        [Category("鼠标操作"), Description("鼠标旋转速度系数")]
        public float MouseRotateFactor
        {
            get
            {
                return NLevelEditorEngine.Instance.ActorEd.MouseRotateFactor;
            }
            set
            {
                NLevelEditorEngine.Instance.ActorEd.MouseRotateFactor = value;
            }
        }
    }
}
