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
	var hue = 0;//Random(6)*60;
	for(var i = 0; i < 4; ++i)
	{
		colorarray[i] = HSL(hue, RandomX(200, 255), 127);
		hue += 60;
	}
	
	var shotData  = new FireworkData {
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
												//duration = [20, 25],
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
														timer = 3,
														density = 5,
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
	var subeffects = [new shotData.effects[0] { delay = [20, 25] }];
	subeffects[0].emitted = [ new shotData.effects[0].emitted[0] {} ];
	subeffects[0].emitted[0].fireworkData = new shotData.effects[0].emitted[0].fireworkData { duration = [20, 25], effects = nil };
	
	shotData.effects[0].emitted[0].fireworkData.effects = subeffects;
	
	
	var emitted = {
						sound = "Firework::MediumShot?",
						pitch = [-5, 20],
						angle = FW_Distribution_Random(-5, 5),
						xoffset = FW_Distribution_LinearModulo(-4, 4, 5, 0),
						speed = 58,
						height = 4,
					
						fireworkData = shotData,
					};
	
	fireworkData = new FireworkData {
		fuseTime = 38,
		shots = [
			{
				step = 40,
				amount = 20,
				emitted = [
					emitted
				],
			}
		],
	};
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







	


local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Components = { Coal = 2 };

public func IsChemicalProduct() { return true; }
