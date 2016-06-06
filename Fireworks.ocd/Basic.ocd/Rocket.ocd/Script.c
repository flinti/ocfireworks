/**
	Firework Rocket
	Shshshshshsh-Boom.

	@author Flinti
*/

#include FireworkBomb

	


protected func Initialize()
{
	_inherited(...);
	
	var data = {
		fuseTime = 0,
		effects = new FW_Effect_Emit {
							delay = 36,
							die = true,
							amount = 12,
							emitted = [
							{
								angle = [0, 360, -20, 20],
								speed = [15, 20],
								fireworkData = {
									duration = [16, 24],
									trails = new FW_Trail_Glow {
										lightRange = 10,
										lightFadeoutRange = 100,
										size = 3,
										float = true,
										gravity = 30,
										color = HSL(Random(256), 230, 128),			
									}
								}
							}
							],
						},
		trails = [ /*new FW_Trail_Glow {
						duration = 22,
						lightRange = 4,
						lightFadeoutRange = 100,
						size = 3,
						soundLoop = "Fire::FuseLoop",
						accel = 360,
						color = HSL(Random(256), 255, 128),
					},*/
					new FW_Trail_Sparkle {
						duration = 20,
						lightRange = 4,
						lightFadeoutRange = 100,
						size = [2, 6],
						soundLoop = "Fire::FuseLoop",
						accel = 360,
						color = HSL(Random(256), 255, 128),
					},
					new FW_Trail_None {
						delay = 20,
					}
				]
					
	};
	SetFireworkData(data);
}

public func ControlUse(object clonk, int x, int y)
{
	var angle = Angle(0, 0, x, y);
	Exit();
	SetR(angle);
	SetSpeed(Sin(angle, 10), -Cos(angle, 10));
	SetFused(clonk);
	SetObjectLayer(this);
	Sound("Firework::RocketStart?");
	return true;
}



local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Components = { Coal = 1, Wood = 1 };

public func IsChemicalProduct() { return true; }

