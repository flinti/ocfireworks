/**
 * Fish Rocket
 * Shshshshshsh-Boom.
 * 
 * @author Flinti
*/

#include FireworkRocket




protected func Initialize()
{
	var trailclr = RGB(140, 140, 140);
	var effclr = HSL(Random(256), RandomX(200, 255), RandomX(140, 180));
	
	Set(trailclr, effclr);
}

public func Set(int trailclr, int effectclr)
{
	var data = new FireworkData {
		fuseTime = 0,
		effects = [ 
			new FW_Effect_EmitStarsRandom {
							delay = 36,
							die = true,
							amount = 30,
							flash = effectclr,
							emitted = [
								{
									type = "fish",
									color = effectclr,
									speed = [0, 10],
									size = 5,
									lifetime = [50, 70],
									fullsizeAfter = 150,
									maxSwarmForce = 30,
								}
							],
			},
			new FW_Effect_Glow {
				delay = 36,
				color = effectclr,
				duration = 65,
			},
			new FW_Effect_Emit {
							delay = 36,
							amount = 6,
							emitted = [
								{
									angle = FW_Distribution_Random(0, 359),
									speed = [11, 16],
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
											}
										],
									}
								}
							],
			}
		], 
		trails = [ 
			new FW_Trail_Move {
				duration = 20,
				accel = 360,
			},
			new FW_Trail_Glow {
						size = [2, 3],
						color = trailclr,
						light = true,
			}
		]
	};
	
	
	SetFireworkData(data);
}




local Name = "$Name$";
local Description = "$Description$";
local Components = { Coal = 1, Wood = 1 };

