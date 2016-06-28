/**
 * Firework Bomb
 * Basic functionality for all firework bombs
 * 
 * @author Flinti
*/

#include FireworkBase



/* Implementation */

protected func OnFinished()
{
	RemoveObject();
}

protected func Initialize()
{
	SetGraphics("None");
}

/** 
 * Interactions with other objects
 * You can use this with the cannon
*/

public func Fuse(bool shallExplodeOnHit)
{
	SetFused();
}

public func OnCannonShot(object cannon)
{
	SetFused(cannon);
}

public func IsFireworkBomb() { return true; }


local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Components = { Coal = 1 };



