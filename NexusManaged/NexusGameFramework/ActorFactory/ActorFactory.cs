using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using NexusEngine;

namespace Nexus.GameFramework
{
    /// <summary>
    /// NActor Factory 常用于编辑器创建NActor
    /// </summary>
    public class ActorFactory
    {
        #region Fields
        /// <summary>
        /// Native Actor类名
        /// </summary>
        protected string nativeActorClassName;

        #endregion

        #region Constructor
        protected ActorFactory()
        {
        }
        #endregion

        #region Properties
        /// <summary>
        /// Native Actor类名
        /// </summary>
        public string NativeActorClassName
        {
            get
            {
                return nativeActorClassName;
            }
            set
            {
                nativeActorClassName = value;
            }
        }
        #endregion

        #region Methods
        /// <summary>
        /// 创建Actor对象
        /// </summary>
        public NActor CreateActor(NLevel level, Vector3 location, Vector3 rotation)
        {
            if (string.IsNullOrEmpty(nativeActorClassName))
            {
                return null;
            }

            string actorName = this.GetType().Name + "_" + DateTime.Now.ToFileTime();
            NActor newActor = level.CreateActor(actorName, nativeActorClassName);

            if (newActor != null)
            {
				PostCreateActor(newActor);
                newActor.Location = location;
                newActor.Rotation = rotation;
            }            

            return newActor;
        }

        /// <summary>
        /// 创建对象成功后调用
        /// </summary>
        /// <param name="actor">新创建的对象，可能为null</param>
        virtual public void PostCreateActor(NActor actor) { }

        #endregion

    }
}
