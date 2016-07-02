/**
 * Firework Cake
 * Boom-boom, boom-boom, ...
 * 
 * @author Flinti
*/

#include FireworkBase

public func ShotDelay(int delay_) { return { delay = delay_ }; }

func Initialize()
{
	var trailclr = RGB(230, 255, 230);
	var buqclr = HSL(Random(256), RandomX(200, 255), 127);
	
	var colorarray = CreateArray(4);
	var hue = Random(6)*60;
	for(var i = 0; i < 4; ++i)
	{
		colorarray[i] = HSL(hue, RandomX(200, 255), 127);
		hue += 60;
	}
	
	var firstshotData  = new FireworkData {
							fuseTime = 0,
							effects = [
								new FW_Effect_Emit {
									delay = [21, 25],
									die = true,
									amount = 4,
									flash = buqclr,
									emitted = [
										{
											angle = FW_Distribution_Linear(0, 360, 2),
											speed = [12, 15],
											fireworkData = new FireworkData {
												duration = [20, 25],
												trails = [
													new FW_Trail_Move {
														float = true,
														gravity = 10,
													},
													new FW_Trail_Star {
														size = 8,
														//color = -1,
														color = FW_Distribution_Set(colorarray),
														light = true,
													},
													new FW_Trail_Glow {
														size = 2,
														color = trailclr,
													}
												],
											}
										}
									],
								}
							],
							trails = [
								new FW_Trail_Glow {
									size = 2,
									color = trailclr,
									lifetime = [40, 60],
									light = true,
								}
							],
						};
	
	
	var emittedBase = {
						sound = "Firework::MediumShot?",
						angle = FW_Distribution_Random(-5, 5),
						xoffset = FW_Distribution_LinearModulo(-4, 4, 5, 0),
						speed = 58,
						height = 4,
					
						fireworkData = firstshotData,
					};
	
	fireworkData = new FireworkData {
		fuseTime = 38,
		shots = [
			{
				step = 30,
				amount = 20,
				emitted = [
					emittedBase
				],
			}
		],
	};
	
	
	/*var hue = Random(6)*60;
	for(var i = 0; i < 4; ++i)
	{
		buqclr = HSL(hue, RandomX(200, 255), 127);
		
		var shot = new emittedBase {};
		shot.fireworkData = new emittedBase.fireworkData {};
		shot.fireworkData.effects = new emittedBase.fireworkData.effects[0] {};
		shot.fireworkData.effects.flash = buqclr;
		shot.fireworkData.effects.emitted = [new emittedBase.fireworkData.effects[0].emitted[0] {}];
		shot.fireworkData.effects.emitted[0].fireworkData = new emittedBase.fireworkData.effects[0].emitted[0].fireworkData {};
		shot.fireworkData.effects.emitted[0].fireworkData.trails = CreateArray(GetLength(emittedBase.fireworkData.effects[0].emitted[0].fireworkData.trails));
		for(var q = 0; q < GetLength(emittedBase.fireworkData.effects[0].emitted[0].fireworkData.trails); ++q)
			shot.fireworkData.effects.emitted[0].fireworkData.trails[q] = new emittedBase.fireworkData.effects[0].emitted[0].fireworkData.trails[q] {};
		shot.fireworkData.effects.emitted[0].fireworkData.trails[1].color = buqclr;
		
		fireworkData.shots[0].emitted[i] = shot;
		
		hue += 60;
	}*/
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







	


local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Components = { Coal = 2 };

public func IsChemicalProduct() { return true; }
