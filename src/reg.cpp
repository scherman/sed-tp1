#include "pmodeladm.h"
#include "register.h"

#include "sun.h"


void register_atomics_on(ParallelModelAdmin &admin)
{
	admin.registerAtomic(NewAtomicFunction<Sun>(), ATOMIC_MODEL_NAME);
}
