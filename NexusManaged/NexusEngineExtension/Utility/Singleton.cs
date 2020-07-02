using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

namespace NexusEngineExtension
{
    /// <summary>
    /// A generic singleton
    /// </summary>
    /// <remarks>
    /// Although this class will allow it, don't try to do this: Singleton&lt; interface &gt;
    /// </remarks>
    /// <typeparam name="T">a class</typeparam>
    public abstract class Singleton<T> : IDisposable where T : class
    {
        public Singleton()
        {
            if (SingletonFactory.instance != null && !IntPtr.ReferenceEquals(this, SingletonFactory.instance))
                throw new Exception(String.Format("Cannot create instances of the {0} class. Use the static Instance property instead.", this.GetType().Name));
        }

        ~Singleton()
        {
            Dispose(false);
        }

        public virtual bool Initialize(params object[] args)
        {
            return true;
        }

        public static T Instance
        {
            get
            {
                try
                {
                    return SingletonFactory.instance;
                }
                catch (TypeInitializationException)
                {
                    throw new Exception(string.Format("Type {0} must implement a private parameterless constructor.", typeof(T)));
                }
            }
        }

        class SingletonFactory
        {
            static SingletonFactory()
            {

            }

            internal static T instance = (T)typeof(T).InvokeMember(typeof(T).Name,
                                                                      BindingFlags.CreateInstance | BindingFlags.Instance | BindingFlags.NonPublic,
                                                                      null, null, null);
        }

        public static void Destroy()
        {
            SingletonFactory.instance = null;
        }

        #region IDisposable Implementation


        #region isDisposed Property

        private bool _disposed = false;
        /// <summary>
        /// Determines if this instance has been disposed of already.
        /// </summary>
        protected bool isDisposed
        {
            get
            {
                return _disposed;
            }
            set
            {
                _disposed = value;
            }
        }

        #endregion isDisposed Property

        /// <summary>
        /// Class level dispose method
        /// </summary>
        /// <remarks>
        /// When implementing this method in an inherited class the following template should be used;
        /// protected override void dispose( bool disposeManagedResources )
        /// {
        /// 	if ( !isDisposed )
        /// 	{
        /// 		if ( disposeManagedResources )
        /// 		{
        /// 			// Dispose managed resources.
        /// 		}
        /// 
        /// 		// There are no unmanaged resources to release, but
        /// 		// if we add them, they need to be released here.
        /// 	}
        ///
        /// 	// If it is available, make the call to the
        /// 	// base class's Dispose(Boolean) method
        /// 	base.dispose( disposeManagedResources );
        /// }
        /// </remarks>
        /// <param name="disposeManagedResources">True if Unmanaged resources should be released.</param>
        protected virtual void Dispose(bool disposeManagedResources)
        {
            if (!isDisposed)
            {
                if (disposeManagedResources)
                {
                    Singleton<T>.Destroy();
                }

                // There are no unmanaged resources to release, but
                // if we add them, they need to be released here.
            }
            isDisposed = true;
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        #endregion IDisposable Implementation
    }
}
