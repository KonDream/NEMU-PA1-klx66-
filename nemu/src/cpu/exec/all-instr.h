#include "prefix/prefix.h"

#include "data-mov/mov.h"
/* Add by klx*/
#include "commands/call.h"
#include "commands/push.h"
#include "commands/test.h"
#include "commands/je.h"

#include "commands/cmp.h"
#include "commands/add.h"

#include "commands/pop.h"
#include "commands/ret.h"
/*~~~~~~~~~~~~~*/
#include "data-mov/xchg.h"
#include "data-mov/movext.h"
#include "data-mov/cltd.h"
#include "data-mov/leave.h"

#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"

#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"

#include "string/rep.h"

#include "misc/misc.h"

#include "special/special.h"
