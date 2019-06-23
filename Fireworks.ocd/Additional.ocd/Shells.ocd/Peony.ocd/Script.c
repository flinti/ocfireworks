/**
 * Firework Shell: Peony
 * 
 * 
 * @author Flinti
*/

#include FireworkBomb


private func Initialize()
{
	_inherited(...);
	var effclrA = HSL(Random(256), RandomX(200, 255), RandomX(140, 180));
	var trailclr = RGB(255, 140, 30);
	
	Set(effclrA, trailclr);
}

public func Set(int trailclr, int effectclr)
{
	fireworkData = new FireworkData {
		fuseTime = 0,
		effects = [
			new FW_Effect_Emit {
				delay = [21, 25],
				amount = 20,
				die = true,
				emitted = [
					{
						angle = FW_Distribution_Linear(0, 359, 10),
						speed = [5, 16],
						fireworkData = new FireworkData {
							duration = [20, 24],
							trails = [
								new FW_Trail_Move {
									float = true,
									gravity = 15,
								},
								new FW_Trail_Star {
									size = 3,
									color = effectclr,
									light = true,
								}
							],
						}
					}
				],
			}
		], 
		trails = [
			new FW_Trail_Glow {
				size = [1, 2],
				density = 1,
				timer = 3,
				color = trailclr,
				light = true,
				lightRange = 5,
				lightFadeoutRange = 20,
			}
		]
	};
}


local Name = "$Name$";
local Description = "$Description$";
local Components = { Coal = 1 };



