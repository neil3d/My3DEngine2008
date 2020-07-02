#pragma once

namespace NexusEngine
{
	public value struct MathConst
	{
		literal float HalfPI		=	float(3.14159265358979323846264338327950288419716939937510582 * 0.5);
		literal float QuarterPI	=	float(3.14159265358979323846264338327950288419716939937510582 * 0.25);
		literal float PI			=	float(3.14159265358979323846264338327950288419716939937510582);
		literal float TwoPI		=	float(3.14159265358979323846264338327950288419716939937510582 * 2.0);

		literal float EPS			=	float(10e-6);
		literal float DoubleEPS	=	float(10e-6) * 2;
		literal float BigEPS		=	float(10e-2);
		literal float SmallEPS		=	float(10e-6);

		// Maximum size of the world
		literal float WorldMax		=	524288.0f;
		// Half the maximum size of the world
		literal float HalfWorldMax	=	262144.0f;
		// Half the maximum size of the world - 1
		literal float HalfWorldMax1	=	262143.0f;
	};
}//namespace NexusCore