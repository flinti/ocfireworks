/**
 * Firework Fountain
 * Shshshshshshshshshshshshshsh.
 * 
 * @author Flinti
*/

#include FireworkBase


func Initialize()
{
	var angle = Random(256);
	var clra = HSL(angle, 255, 127);
	var clrb = HSL(angle + 60 + Random(240), 255, 127);
	fireworkData = new FireworkData {
		fuseTime = 3,
		duration = 38*10,
		fountaineffects = [
					new FW_FountainEffect_Sparkle {
						speed = [20, 30],
						deviation = 30,
						size = [7, 10],
						lifetime = [25, 35],
						color = clra,
						light = true,
					},
					new FW_FountainEffect_Sparkle {
						speed = [20, 35],
						deviation = 30,
						size = [5, 8],
						lifetime = [25, 35],
						color = clrb, 
						light = true,
					}
				],
	};
}

public func ControlUse(object clonk, int x, int y)
{	
	if(x > 0)
		Exit(10);
	else
		Exit(-10);
		
	SetFused(clonk);
	SetObjectLayer(this);
	return true;
}

protected func OnFused()
{
}

protected func OnFinished()
{
	inherited(...);
	//CreateEffect(FxBurnedOutSmoke, 1, 1);
}

protected func DoStartFirework()
{
	inherited(...);
	ScheduleTrails(fireworkData.fountaineffects);
}

/*local FxBurnedOutSmoke = new Effect {
	Timer = func()
	{
		if(!Random(3))
			Target->Smoke(RandomX(-1, 1), RandomX(-4, -2), RandomX(3, 6));
		
		if(Time > 250 + Random(200))
			return FX_Execute_Kill;
	}
};*/
	


local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Components = { Coal = 1 };

public func IsChemicalProduct() { return true; }
