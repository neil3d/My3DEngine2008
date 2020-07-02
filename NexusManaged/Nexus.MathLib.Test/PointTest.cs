using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using Nexus.MathLib;

namespace Nexus.MathLib.Test
{
    [TestFixture]
    public class PointTest
    {
        private Point pt12;
        private Point pt12Ex;
        private Point pt31;

        [SetUp]
        public void SetUp()
        {
            pt12 = new Point(12.0f, 12.0f);
            pt12Ex = new Point(12.0f, 12.0f);
            pt31 = new Point(31.0f, 31.0f);
        }

        [Test]
        public void EqualTest()
        {
            Assert.AreEqual( pt12,pt12Ex);
            Assert.AreNotEqual(pt12, pt31);
            
            Point newPt31 = pt31;
            Assert.AreEqual( pt31, newPt31 );
            Assert.IsTrue( pt31 == newPt31 );
        }

        [Test]
        public void AddTest()
        {
            Point pt24 = pt12 + new Point(12, 12);
            Assert.AreEqual( pt24, new Point(24,24));
        }

        [Test]
        public void SubtractTest()
        {
            Assert.AreEqual( pt12, (new Point(24,24) - new Point(12,12)) );
        }

        [Test]
        public void ParseTest()
        {
            Point pt = Point.Parse("x:12 y:14");
            Assert.AreEqual( pt, new Point(12,14));

            Assert.AreEqual( Point.Parse("X=32 Y:42"),new Point(32,42));
            Assert.AreEqual(Point.Parse("Y=32 X:42"), new Point(42, 32));
        }
    }
}
