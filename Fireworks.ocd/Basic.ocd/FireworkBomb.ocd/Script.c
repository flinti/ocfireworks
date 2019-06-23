/**
 * Firework Bomb
 * Basic functionality for all firework bombs
 * 
 * @author Flinti
*/

#include FireworkBase



/* Implementation */

public func OnFused(object by)
{
	SetGraphics("None");
}

private func OnFinished()
{
	RemoveObject();
}

private func Initialize()
{
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



