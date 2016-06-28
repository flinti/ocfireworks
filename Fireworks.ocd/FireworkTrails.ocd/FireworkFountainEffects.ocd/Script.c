/**
 * Firework Fountain Effects
 * 
 * Contains fountain effect prototypes
 * 
 * Contains fountain effect functions. These have to be called from object context.
 * These are internals, for public interfaces see /Basic.ocd/FireworkBase.ocd, /Basic.ocd/Fountain.ocd and /System.ocg/Firework.c
 * 
 * @author Flinti
*/

/* -- Effect prototypes -- */

static const FW_FountainEffect_None = { 
								Name = "FountainEffect_None", 
								initfn = nil,
								execfn = FireworkFountainEffects.None, 
							};
static const FW_FountainEffect_Sparkle = { 
								Name = "FountainEffect_Sparkle", 
								initfn = FireworkFountainEffects.SparkleInit,
								execfn = FireworkFountainEffects.Sparkle, 
								color = -1, 
								lightRange = 20, 
								lightFadeoutRange = 100,
							};





/* -- None -- */

public func None(effect fx)
{
}


/* -- Sparkle -- */

func SparkleParticle() 
{
	return {
		Rotation = PV_Direction(),
		OnCollision = PC_Stop(),
		CollisionVertex = 500,
		BlitMode = GFX_BLIT_Additive,
		Alpha = PV_Random(1, 254, 3),
		ForceX = PV_Wind(40, PV_Random(-1,1)),
		ForceY = PV_Gravity(20, PV_Random(-1,1)),
		DampingX = 950, 
		DampingY = 950,
		Size = PV_KeyFrames(0, 0, 0, 200, PV_Random(3, 10), 1000, PV_Random(8, 11)),
	};
}


public func SparkleInit(effect fx, proplist fweffect)
{
	fx.particleType = "StarSpark";
	fx.particle = Particles_Colored(SparkleParticle(), fweffect.color, fweffect.color2);
	
	fx.deviation = fweffect.deviation ?? 10;
	fx.speed = fweffect.speed ?? [0, 0];
	if(GetType(fx.speed) != C4V_Array)
		fx.speed = [fx.speed, fx.speed];
		
	var size = fweffect.size;
	if(GetType(size) == C4V_Array)
		fx.particle.Size = PV_KeyFrames(0, 0, 0, 200, PV_Random(size[0] / 2, size[1] / 2), 500, PV_Random(size[0], size[1]), 1000, 0);
	else
		fx.particle.Size = PV_KeyFrames(0, 0, 0, 200, size / 2, 500, size, 1000, 0);
	
	if(fweffect.lifetime)
	{
		if(GetType(fweffect.lifetime) == C4V_Array)
			fx.lifetime = PV_Random(fweffect.lifetime[0], fweffect.lifetime[1]);
		else
			fx.lifetime = fweffect.lifetime;
	}
	else
		fx.lifetime = PV_Random(20, 30);
		
	fx.posradius = PV_Random(1, 5, nil, 1);
	fx.speedradius = PV_Random(fx.speed[0], fx.speed[1], 0, 1);
}

public func Sparkle(effect fx)
{
	var angle = GetR();
	var angle2 = -90 - angle;
	CreateParticle(fx.particleType, 
				PV_Cos(angle2, fx.posradius), //x
				PV_Sin(angle2, fx.posradius), //y
				
				PV_Cos(PV_Random(angle2 - fx.deviation, angle2 + fx.deviation, 0, 2), fx.speedradius), //xdir
				PV_Sin(PV_Random(angle2 - fx.deviation, angle2 + fx.deviation, 0, 2), fx.speedradius), //ydir
		fx.lifetime, fx.particle, fx.amount);
}


