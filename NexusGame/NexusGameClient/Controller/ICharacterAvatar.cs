namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using NexusEngine;

    /// <summary>
    /// 角色Avatar接口
    /// </summary>
    public interface ICharacterAvatar
    {
        #region Methods

        /// <summary>
        /// 初始化，创建Avatar所需的对象
        /// </summary>
        void InitAvatar(string uniqueActorName);

        void ResetAvatarResource(NActor ownerActor);

        void Update(float deltaTime);

        void SetAnim(string AnimName);

        void PlayAnim(bool  loop, float rate, float startTime);

        #endregion Methods
    }
}