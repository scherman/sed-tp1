#include "pmodeladm.h"
#include "register.h"

#include "sun.h"
#include "controller.h"
#include "solar_cell.h"
#include "engine.h"


void register_atomics_on(ParallelModelAdmin &admin)
{
	admin.registerAtomic(NewAtomicFunction<Sun>(), SUN_MODEL_NAME);
	admin.registerAtomic(NewAtomicFunction<Controller>(), CONTROLLER_MODEL_NAME);
	admin.registerAtomic(NewAtomicFunction<SolarCell>(), SOLAR_CELL_MODEL_NAME);
	admin.registerAtomic(NewAtomicFunction<Engine>(), ENGINE_MODEL_NAME);
}
