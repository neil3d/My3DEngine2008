using System;

namespace Nexus.GUI
{


    #region SizingLocation
    /// <summary>
    ///		The set of possible locations for the mouse on a frame windows sizing border.
    /// </summary>
    public enum SizingLocation
    {
        /// <summary>
        ///		Position is not a sizing location.
        /// </summary>
        None,
        /// <summary>
        ///		Position will size from the top-left.
        /// </summary>
        TopLeft,
        /// <summary>
        ///		Position will size from the top-right.
        /// </summary>
        TopRight,
        /// <summary>
        ///		Position will size from the bottom left.
        /// </summary>
        BottomLeft,
        /// <summary>
        ///		Position will size from the bottom right.
        /// </summary>
        BottomRight,
        /// <summary>
        ///		Position will size from the top.
        /// </summary>
        Top,
        /// <summary>
        ///		Position will size from the left.
        /// </summary>
        Left,
        /// <summary>
        ///		Position will size from the bottom.
        /// </summary>
        Bottom,
        /// <summary>
        ///		Position will size from the right.
        /// </summary>
        Right
    }
    #endregion SizingLocation

    /// <summary>
    /// Enumeration of possible values for sorting direction
    /// as used with ListHeaderSegment, ListHeader, and MultiColumnList classes.
    /// </summary>
    public enum SortDirection
    {
        /// <summary>
        /// Items should not be sorted.
        /// </summary>
        None,

        /// <summary>
        /// Items should be sorted in ascending order.
        /// </summary>
        Ascending,

        /// <summary>
        /// Items should be sorted in descending order.
        /// </summary>
        Descending
    }

}
