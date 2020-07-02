using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NexusEngineExtension
{
    /// <summary>
    /// 命令选项接口
    /// </summary>
    public interface ICommandOption
    {
        /// <summary>
        /// 执行该命令选项
        /// </summary>
        bool Execute();

        /// <summary>
        /// 取消执行该命令选项
        /// </summary>
        bool UnExecute();

        /// <summary>
        /// 当前Command是否有效
        /// </summary>
        /// <returns></returns>
        bool IsValid();
    }
}
