using System;
using System.Text;

namespace NexusEngineExtension
{
    /// <summary>
    /// General exception that gets thrown upon an error with the excel.
    /// </summary>
    [global::System.Serializable]
    public class ExcelException : Exception
    {
        /// <summary>
        /// Initializes a new instance of the ExcelException class
        /// </summary>
        public ExcelException() { }
        /// <summary>
        /// Initializes a new instance of the ExcelException class with a specified error message
        /// </summary>
        /// <param name="message">A message that describes the error.</param>
        public ExcelException(string message) : base(message) { }
        /// <summary>
        /// Initializes a new instance of the ExcelException class with a specified error message and 
        /// a reference to the inner exception that is the cause of this exception.
        /// </summary>
        /// <param name="message">A message that describes the error.</param>
        /// <param name="inner">The exception that is the cause of the current exception.</param>
        public ExcelException(string message, Exception inner) : base(message, inner) { }
        /// <summary>
        /// Initializes a new instance of the ExcelException class with serialized data.
        /// </summary>
        /// <param name="info">The object that holds the serialized object data.</param>
        /// <param name="context">The contextual information about the source or destination.</param>
        protected ExcelException(
          System.Runtime.Serialization.SerializationInfo info,
          System.Runtime.Serialization.StreamingContext context)
            : base(info, context) { }
    }
}
