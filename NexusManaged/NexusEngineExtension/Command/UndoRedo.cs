using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NexusEngineExtension
{
    public class UndoRedo
    {
        private Stack<ICommandOption> undoCommands = new Stack<ICommandOption>();
        private Stack<ICommandOption> redoCommands = new Stack<ICommandOption>();

        public Stack<ICommandOption> UndoCommands
        {
            get { return undoCommands; }
        }

        public Stack<ICommandOption> RedoCommands
        {
            get { return redoCommands; }
        }

        public void Redo(int levels)
        {
            for (int i = 1; i <= levels; i++)
            {
                if (redoCommands.Count != 0)
                {
                    ICommandOption command = redoCommands.Pop();
                    if(command.Execute())
                    {
                        undoCommands.Push(command);
                    }
                }
            }
        }

        public void Undo(int levels)
        {
            for (int i = 1; i <= levels; i++)
            {
                if (undoCommands.Count != 0)
                {
                    ICommandOption command = undoCommands.Pop();
                    if(command.UnExecute())
                    {
                        redoCommands.Push(command);
                    }
                }
            }
        }

        public bool IsUndoPossible()
        {
            if (undoCommands.Count != 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        public bool IsRedoPossible()
        {

            if (redoCommands.Count != 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
