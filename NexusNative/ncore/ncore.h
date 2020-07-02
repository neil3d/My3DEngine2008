
#ifndef _NCORE_H_
#define _NCORE_H_

#include "memory/nmemory.h"
#include "base_depend.h"
#include "base_define.h"

//-- object model
#include "object/nexception.h"
#include "object/singleton.h"
#include "object/nclass.h"
#include "object/nobject.h"
#include "object/narchive.h"
#include "object/serialize.h"
#include "object/nname.h"

//-- math
#include "math/math_const.h"
#include "math/color.h"
#include "math/vector.h"
#include "math/matrix.h"
#include "math/quat.h"
#include "math/ncamera.h"
#include "math/geom_def.h"
#include "math/nfrustum.h"
#include "math/math_serialize.h"
#include "math/rect.h"
#include "math/object_space.h"

//-- platform
#include "platform/nshared_lib.h"

//-- file
#include "file/nfile_system.h"

//--
#include "nlog.h"
#include "util/string_util.h"

nCoreAPI void nexus_init_core();

#endif //_NCORE_H_