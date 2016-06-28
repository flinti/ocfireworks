/**
 * Firework Rocket
 * Shshshshshsh-Boom.
 * 
 * @author Flinti
*/

#include FireworkBomb

	


protected func Initialize()
{
	_inherited(...);
	
	//var trailclr = HSL(Random(256), 255, 160);
	var trailclr = RGB(255, 140, 30);
	var effclrA = HSL(Random(256), RandomX(200, 255), RandomX(140, 180));
	var effclrB = InvertColor(effclrA);
	
	var data = new FireworkData {
		fuseTime = 0,
		effects = [ 
			new FW_Effect_Emit {
							delay = 36,
							die = true,
							amount = 12,
							flash = effclrA,
							emitted = [
								{
									angle = FW_Distribution_Linear(0, 360, 20),
									speed = [15, 20],
									fireworkData = new FireworkData {
										duration = [16, 24],
										trails = [
											new FW_Trail_Move {
												float = true,
												gravity = 30,
											},
											new FW_Trail_Glow {
												lightRange = 10,
												lightFadeoutRange = 100,
												size = 2,
												//density = 2,
												color = effclrA,
												light = true,			
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
			new FW_Trail_Sparkle {
						size = [2, 6],
						color = trailclr,
						lifetime = [80, 120],
						light = true,
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
	CreateEffect(FxIntCorrectRotation, 1, 5);
	return true;
}

protected func OnFinished()
{
	RemoveObject();
}

local FxIntCorrectRotation = new Effect {
	Timer = func()
	{
		var angle = Angle(0, 0, Target->GetXDir(100), Target->GetYDir(100));

		if(angle < 180)
			angle -= 5 + Random(3);
		else
			angle += 5 + Random(3);
			
		Target->SetR(angle);
	}
};


local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Components = { Coal = 1, Wood = 1 };

public func IsChemicalProduct() { return true; }

