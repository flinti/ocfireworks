/**
	Firework Trails
	
	Contains trail prototypes.
	
	Contains trail functions. These have to be called from object context.
	These are internals, for public interfaces see /Basic.ocd/FireworkBomb.ocd and /System.ocg/Firework.c

	@author Flinti
*/


/* ---------------------------------- Trail prototypes ---------------------------------- */

static const FW_Trail_None = { 
								execfn = FireworkTrails.None, 
								type = FWT_None,
								};
static const FW_Trail_Glow = { 
								initfn = FireworkTrails.GlowInit, 
								execfn = FireworkTrails.Glow, 
								type = FWT_Glow, 
								size = 3, 
								color = -1, 
								lightRange = 4, 
								lightFadeoutRange = 100,
							};
static const FW_Trail_Sparkle = { 
								initfn = FireworkTrails.SparkleInit, 
								execfn = FireworkTrails.Sparkle, 
								type = FWT_Sparkle, 
								size = 3, 
								color = -1, 
								lightRange = 4, 
								lightFadeoutRange = 100,
							};
							
static const FWT_None = 0;
static const FWT_Glow = 1;
static const FWT_Sparkle = 2;



/* ----------------------------------------- None ----------------------------------------- */

public func None(effect fx)
{
}

/* ----------------------------------------- Glow ----------------------------------------- */

public func GlowInit(effect fx, proplist trail)
{
	fx.trailParticleType = "Magic";
	fx.trailParticle = Particles_Colored(Particles_Glimmer(), trail.color, trail.color2);
	fx.trailParticle.Size = PV_Linear(trail.size, 0);
	fx.trailParticle.DampingY = 600;
	fx.trailParticle.BlitMode = GFX_BLIT_Additive;
}

public func Glow(effect fx)
{
	var angle = 90 - Angle(0, 0, GetXDir(100), GetYDir(100)); //clonk angle -> math angle
	CreateParticle(fx.trailParticleType, PV_Cos(angle, PV_Random(1, 5, nil, 1)), PV_Sin(-angle, PV_Random(1, 5, nil, 1)), 0, 0, PV_Random(40, 50), fx.trailParticle, 10);
}


/* --------------------------------------- Sparkle --------------------------------------- */

public func SparkleInit(effect fx, proplist trail)
{
	fx.trailParticleType = "StarSpark";
	fx.trailParticle = Particles_Colored(Particles_SmokeTrail(), trail.color, trail.color2);
	fx.trailParticle.BlitMode = GFX_BLIT_Additive;
	
	var size = trail.size;
	if(GetType(size) == C4V_Array)
		fx.trailParticle.Size = PV_KeyFrames(0, 0, PV_Random(size[0], size[1]), 700, PV_Random(size[0] / 2, size[1] / 2), 1000, 0);
	else
		fx.trailParticle.Size = PV_KeyFrames(0, 0, size, 700, size / 2, 1000, 0);
}

public func Sparkle(effect fx)
{
	var rawangle = Angle(0, 0, GetXDir(100), GetYDir(100));
	var angle = 90 - rawangle; //clonk angle -> math angle
	var angle2 = -rawangle;
	
	CreateParticle(fx.trailParticleType, 
				PV_Cos(angle, PV_Random(1, 5, nil, 1)), 
				PV_Sin(-angle, PV_Random(1, 5, nil, 1)), 
				
				PV_Cos(angle2, PV_Random(-4, 4, nil, 2)), 
				PV_Sin(-angle2, PV_Random(-4, 4, nil, 2)), 
		PV_Random(70, 100), fx.trailParticle, 7);
}

