// ------------------------------------------------------------------------------------------------
#include "Misc/Model.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CSStr GetModelName(Int32 /*id*/)
{
    // @TODO Implement...
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
void SetModelName(Int32 /*id*/, StackStrF & /*name*/)
{
    // @TODO Implement...
}

// ------------------------------------------------------------------------------------------------
bool IsModelWeapon(Int32 id)
{
    switch (id)
    {
        case 258:
        case 259:
        case 260:
        case 261:
        case 262:
        case 263:
        case 264:
        case 265:
        case 266:
        case 267:
        case 268:
        case 269:
        case 270:
        case 271:
        case 272:
        case 273:
        case 274:
        case 275:
        case 276:
        case 277:
        case 278:
        case 279:
        case 280:
        case 281:
        case 282:
        case 283:
        case 284:
        case 285:
        case 286:
        case 287:
        case 288:
        case 289:
        case 290:
        case 291:
        case 292:
        case 293:
        case 294:   return true;
        default:    return false;
    }
}

// ------------------------------------------------------------------------------------------------
bool IsModelActuallyWeapon(Int32 id)
{
    switch (id)
    {
        case 259:
        case 260:
        case 261:
        case 262:
        case 263:
        case 264:
        case 265:
        case 266:
        case 267:
        case 268:
        case 269:
        case 270:
        case 271:
        case 272:
        case 274:
        case 275:
        case 276:
        case 277:
        case 278:
        case 279:
        case 280:
        case 281:
        case 282:
        case 283:
        case 284:
        case 285:
        case 286:
        case 287:
        case 288:
        case 289:
        case 290:
        case 291:   return true;
        default:    return false;
    }
}

} // Namespace:: SqMod
