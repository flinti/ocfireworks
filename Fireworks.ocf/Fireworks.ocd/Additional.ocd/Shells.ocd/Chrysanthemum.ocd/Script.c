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
	var trailclr = RGB(230, 255, 230);
	var buqclr = HSL(Random(256), RandomX(200, 255), RandomX(140, 180));
	
	Set(trailclr, buqclr);
}

public func Set(int trailclr, int effectclr)
{
	fireworkData = new FireworkData {
		fuseTime = 0,
		effects = [
			new FW_Effect_Emit {
				delay = [21, 25],
				die = true,
				amount = 14,
				flash = effectclr,
				emitted = [
					{
						angle = FW_Distribution_Linear(0, 360, 13),
						speed = [20, 25],
						fireworkData = new FireworkData {
							duration = [17, 21],
							trails = [
								new FW_Trail_Glow {
									size = 2,
									density = 3,
									color = effectclr,
									light = true,
								},
								new FW_Trail_Move {
									float = true,
									gravity = 20,
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
			}
		],
	};
}


local Name = "$Name$";
local Description = "$Description$";
local Components = { Coal = 1 };



