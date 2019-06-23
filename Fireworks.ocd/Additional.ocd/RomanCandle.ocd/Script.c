/**
 * Roman Candle
 * Pft-pft-pft-pft.
 * 
 * @author Flinti
*/

#include FireworkBase


func Initialize()
{	
	var clrs = [HSL(0, 255, 127), HSL(60, 255, 127), HSL(120, 255, 127), HSL(180, 255, 127), HSL(280, 255, 127)];
	var interval = 6;
	var duration = 38*20;
	
	Set(-1, clrs, interval, duration);
}

public func Set(int trailclr, array starclrs, int interval, int duration)
{
	var effproto = new FW_FountainEffect_Emit {
						timer = GetLength(starclrs) * interval,
						speed = [35, 41],
						deviation = 10,
						fireworkData = new FireworkData {
							duration = [34, 40],
							trails = [
								new FW_Trail_Star {
									size = 6,
									color = nil,
									light = true,
								},
								new FW_Trail_Move {
									float = true,
									gravity = 40,
								},
								new FW_Trail_Glow {
									size = [2, 3],
									color = trailclr,
									density = 1,
									lifetime = [10, 14],
								}
							],
						},
					};
	
	fireworkData = new FireworkData {
		fuseTime = 3,
		duration = duration,
		fountaineffects = CreateArray(GetLength(starclrs)),
	};
	
	for(var i = 0; i < GetLength(starclrs); ++i)
	{
		fireworkData.fountaineffects[i] = new effproto { delay = i * interval };
		fireworkData.fountaineffects[i].fireworkData = new effproto.fireworkData {};
		fireworkData.fountaineffects[i].fireworkData.trails = CreateArray(GetLength(effproto.fireworkData.trails));
		for(var q = 0; q < GetLength(effproto.fireworkData.trails); ++q)
			fireworkData.fountaineffects[i].fireworkData.trails[q] = new effproto.fireworkData.trails[q] {};
		fireworkData.fountaineffects[i].fireworkData.trails[0].color = starclrs[i];
	}
}

public func ControlUse(object clonk, int x, int y)
{	
	if(Distance(x, y, 0, 0) > 40)
		x = y = 0;
	
	Exit(x, y);
		
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
