/**
	Firework Rocket
	Shshshshshsh-Boom.

	@author Flinti
*/

#include FireworkBomb

	
	//local f = func() { local g = func() {Log("g");}; Log("f");};


local a = { qwertz = 1 };
local b = new a {  };
local c = new b{ qwertz = 3 };

protected func Initialize()
{
	_inherited(...);
	
	var data = {
		fuseTime = 0,
		effects = [	new FW_Effect_Emit {
							delay = 30,
							die = true,
							amount = 10,
							emitted = [
							{
								speedSet = [10, 20],
								fireworkData = {
									duration = 20,
									trail = new FW_Trail_Glow {
										float = true,
										lightRange = 10,
										lightFadeoutRange = 100,
										size = 3,
										color = HSL(Random(256), 255, 128),			
									}
								}
							}
							/*{
								speedSet = [30, 40],
								fireworkData = {
									duration = 10,
									trail = new FW_Trail_Glow {
										lightRange = 10,
										lightFadeoutRange = 300,
										size = 3,
										color = RGB(128, 255, 128),			
									}
								}
							}*/
							],
						}
					],
		trail = new FW_Trail_Glow {
						lightRange = 4,
						lightFadeoutRange = 100,
						size = 3,
						soundLoop = "Fire::FuseLoop",
						accel = 20,
						color = RGB(128, 255, 128),
					}
	};
	SetFireworkData(data);
}

protected func ControlUse(object clonk, int iX, int iY)
{
	SetController(clonk->GetController());
	Exit(0, 0, 0, 0, -6);
	SetFused(clonk);
	SetObjectLayer(this);
	Sound("Firework::SoftShot?");
	SetGraphics(nil, Dummy);
	return true;
}



local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Components = { Wood = 1, Firestone = 1 };


