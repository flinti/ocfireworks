/**
 * Firework Trails
 * 
 * Contains trail prototypes.
 * 
 * Contains trail functions. These have to be called from object context.
 * These are internals, for public interfaces see /Basic.ocd/FireworkBase.ocd, /Basic.ocd/FireworkBomb.ocd and /System.ocg/Firework.c
 * 
 * @author Flinti
*/


/* ---------------------------------- Trail prototypes ---------------------------------- */

static const FW_Trail_Move = {
								Name = "Trail_Move", 
								initfn = FireworkTrails.MoveInit,
								execfn = FireworkTrails.Move,
							};
static const FW_Trail_Glow = { 
								Name = "Trail_Glow",
								initfn = FireworkTrails.GlowInit, 
								execfn = FireworkTrails.Glow, 
								size = 3, 
								color = -1, 
								lightRange = 10, 
								lightFadeoutRange = 100,
							};
static const FW_Trail_Sparkle = { 
								Name = "Trail_Sparkle",
								initfn = FireworkTrails.SparkleInit, 
								execfn = FireworkTrails.Sparkle, 
								size = 3, 
								color = -1, 
								lightRange = 10, 
								lightFadeoutRange = 100,
							};
static const FW_Trail_Fish = { 
								Name = "Trail_Fish",
								initfn = FireworkTrails.FishInit, 
								execfn = FireworkTrails.Fish, 
								size = 2, 
								color = -1, 
								lightRange = 10, 
								lightFadeoutRange = 100,
							};
static const FW_Trail_Star = { 
								Name = "Trail_Star",
								initfn = FireworkTrails.StarInit, 
								execfn = FireworkTrails.Star, 
								size = [6, 8], 
								color = -1, 
								lightRange = 15, 
								lightFadeoutRange = 150,
							};

/* ----------------------------------------- Move ----------------------------------------- */

public func MoveInit(effect fx, proplist trail)
{
}

public func Move(effect fx)
{
	if(fx.trail.float)
		SetYDir(GetYDir(10000) - (100 - fx.trail.gravity) * GetGravity(), 10000);
		
	if(fx.trail.accel)
	{
		var newSpeed = GetSpeed(1000) + fx.trail.accel /* * dt (= 1)*/;
		if(newSpeed < 0)
			;
		else
		{
			var angle = GetR();
			SetXDir(Sin(angle, newSpeed), 1000);
			SetYDir(-Cos(angle, newSpeed), 1000);
		}
	}
}

/* ----------------------------------------- Glow ----------------------------------------- */

func GlowParticle()
{
	return {
		Stretch = PV_Speed(1000, 500),
		Rotation = PV_Direction(),
		OnCollision = PC_Die(),
		CollisionVertex = 500,
		BlitMode = GFX_BLIT_Additive,
		Alpha = 255,
		ForceX = PV_Wind(30),
		ForceY = PV_Gravity(20),
		DampingX = PV_Linear(1000,700),
		DampingY = PV_Linear(1000,700),
	};
}

public func GlowInit(effect fx, proplist trail)
{
	fx.trailParticleType = "FireworkStar";//"Magic";
	fx.trailParticle = Particles_Colored(GlowParticle(), trail.color, trail.color2);
	fx.trailParticle.Alpha = GetRGBaValue(trail.color, 0) ?? 255;
	fx.trailParticle.Size = PV_Linear(FW_Distribution->GetParticleValueProvider(trail.size), 0);
	fx.amount = FW_Distribution->GetParticleValueProvider(trail.density) ?? 3;
	fx.lifetime = FW_Distribution->GetParticleValueProvider(trail.lifetime ?? [40, 55]);
	
	fx.posradius = PV_Random(0, 5, nil, 1);
}

public func Glow(effect fx)
{
	var angle = -90 - Angle(0, 0, GetXDir(100), GetYDir(100)); //clonk angle + 180° -> math angle
	CreateParticle(fx.trailParticleType, 
				PV_Cos(angle, fx.posradius), //x
				PV_Sin(-angle, fx.posradius), //y
				
				0, 0, //xdir, ydir
		fx.lifetime, fx.trailParticle, fx.amount);
}


/* --------------------------------------- Sparkle --------------------------------------- */

/*func SparkleParticle() 
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
	//	Alpha = PV_Linear(128, 0),
		Size = PV_KeyFrames(0, 0, 0, 200, PV_Random(3, 10), 1000, PV_Random(8, 11)),
	};
}*/

public func SparkleInit(effect fx, proplist trail)
{
	var p = FireworkParticles->GetParticle("sparkle", trail);
	fx.trailParticleType = p.name;
	fx.trailParticle = Particles_Colored(p, trail.color, trail.color2);
	fx.amount = trail.density ?? 4;
	
	//var size = trail.size;
	//fx.trailParticle.Size = PV_KeyFrames(0, 0, FW_Distribution->GetParticleValueProvider(size), 700, FW_Distribution->GetParticleValueProvider(size, 2), 1000, 0);
		
	fx.lifetime = FW_Distribution->GetParticleValueProvider(trail.lifetime ?? [50, 80]);
	fx.speed = FW_Distribution->GetParticleValueProvider(trail.speed ?? 4);
}

public func Sparkle(effect fx)
{
	var rawangle = Angle(0, 0, GetXDir(100), GetYDir(100));
	var angle = -90 - rawangle; //clonk angle + 180° -> math angle
	var angle2 = -rawangle;
	
	CreateParticle(fx.trailParticleType, 
				PV_Cos(angle, PV_Random(1, 5, nil, 1)), //x
				PV_Sin(-angle, PV_Random(1, 5, nil, 1)), //y
				
				PV_Cos(angle2, PV_Random(-fx.speed, fx.speed, nil, 2)), //xdir
				PV_Sin(-angle2, PV_Random(-fx.speed, fx.speed, nil, 2)), //ydir
		fx.lifetime, fx.trailParticle, fx.amount);
}

/* --------------------------------------- Fish --------------------------------------- */


public func FishInit(effect fx, proplist trail)
{
	var p = FireworkParticles->GetParticle("fish", trail);
	fx.trailParticleType = p.name;
	fx.trailParticle = Particles_Colored(p, trail.color, trail.color2);
	fx.amount = trail.density ?? 4;
		
	fx.lifetime = FW_Distribution->GetParticleValueProvider(trail.lifetime ?? [50, 80]);
	fx.speed = trail.speed ?? 4;
}

public func Fish(effect fx)
{
	var rawangle = Angle(0, 0, GetXDir(100), GetYDir(100));
	var angle = -90 - rawangle; //clonk angle + 180° -> math angle
	
	CreateParticle(fx.trailParticleType, 
				PV_Cos(angle, PV_Random(1, 5, nil, 1)), //x
				PV_Sin(-angle, PV_Random(1, 5, nil, 1)), //y
				
				0, //xdir
				0, //ydir
		fx.lifetime, fx.trailParticle, fx.amount);
}


/* --------------------------------------- Star --------------------------------------- */

protected func StarParticle()
{
	return
	{
		Rotation = PV_Random(0, 359),
		OnCollision = PC_Stop(),
		CollisionVertex = 500,
		//Attach = ATTACH_MoveRelative | ATTACH_Front,
		BlitMode = GFX_BLIT_Additive,
	};
}

protected func StarParticle2()
{
	return new StarParticle()
	{
		Alpha = PV_Random(70, 100),
	};
}


public func StarInit(effect fx, proplist trail)
{
	fx.trailParticleType = "FireworkStarLight";
	fx.trailParticle = Particles_Colored(StarParticle(), trail.color, trail.color2);
	fx.trailParticle.Size = FW_Distribution->GetParticleValueProvider(trail.size ?? [5, 8]);
	
	fx.trailParticle2 = Particles_Colored(StarParticle2(), trail.color, trail.color2);
	fx.trailParticle2.Size = FW_Distribution->GetParticleValueProvider(trail.size ?? [5, 8], nil, 4);
}

public func Star(effect fx)
{
	CreateParticle(fx.trailParticleType, 0, 0, 0, 0, 1, fx.trailParticle);
	CreateParticle(fx.trailParticleType, 0, 0, 0, 0, 1, fx.trailParticle2);
}












