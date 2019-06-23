/**
 * Firework Particle Provider
 * 
 * 
 * @author Flinti
*/

public func GetParticle(string name, proplist config)
{
	if(!config)
		config = {};
	var particle = new starTypes[name] {};
	particle->~init(config);
	return particle;
}


local starTypes;

private func StarInit(proplist p)
{
	var size = FW_Distribution->GetParticleValueProvider(p.size);
	this.Size = PV_Linear(size, 0) ?? this.Size;
}

private func TrailingInit(proplist p)
{
	var size = FW_Distribution->GetParticleValueProvider(p.size);
	this.Size = PV_Linear(size, 0) ?? 1;
	
	var length = FW_Distribution->GetParticleValueProvider(p.length ?? 4000);
	this.Stretch = PV_Speed(length, 500);
}

private func FishInit(proplist p)
{
	if(p.fullsizeAfter)
	{
		var size = FW_Distribution->GetParticleValueProvider(p.size);
		this.Size = PV_KeyFrames(0, 0, 0, p.fullsizeAfter, size, 1000, size);
	}
	else
		this.Size = FW_Distribution->GetParticleValueProvider(p.size);
	if(p.maxSwarmForce)
	{
		var f = p.maxSwarmForce;
		this.ForceX = PV_Wind(40, PV_Random(PV_Linear(0, -f), PV_Linear(0, f), 10));
		this.ForceY = PV_Gravity(20, PV_Random(PV_Linear(0, -f), PV_Linear(0, f), 10));
	}
}

private func SparkleInit(proplist p)
{
	this.Size = PV_KeyFrames(0, 0, FW_Distribution->GetParticleValueProvider(p.size), 700, FW_Distribution->GetParticleValueProvider(p.size, 2), 1000, 0);
}


private func Definition(id def)
{
	starTypes = {
		star = {
			Size = 3,
			Rotation = PV_Direction(),
			ForceX = PV_Wind(40),
			ForceY = PV_Gravity(20),
			DampingX = PV_Linear(950, 800),
			DampingY = PV_Linear(950, 800),
			BlitMode = GFX_BLIT_Additive,
			CollisionVertex = 500,
			OnCollision = PC_Stop(),
			
			name = "FireworkStar",
			stdAmount = 1,
			
			init = def.StarInit,
		},
		trailing = {
			Rotation = PV_Direction(),
			OnCollision = PC_Die(),
			CollisionVertex = 500,
			BlitMode = GFX_BLIT_Additive,
			ForceX = PV_Wind(25),
			ForceY = PV_Gravity(20),
			DampingX = PV_Linear(1000, 900), 
			DampingY = PV_Linear(1000, 900),
			Stretch = PV_Speed(4000, 500),
			
			name = "LongSpark",
			stdAmount = 1,
		
			init = def.TrailingInit,
		},
		fish = {
			Rotation = PV_Direction(),
			OnCollision = PC_Die(),
			CollisionVertex = 500,
			BlitMode = GFX_BLIT_Additive,
			Alpha = PV_Random(100, 254),
			ForceX = PV_Wind(40, PV_Random(PV_Linear(0, -20), PV_Linear(0, 20), 10)),
			ForceY = PV_Gravity(20, PV_Random(PV_Linear(0, -20), PV_Linear(0, 20), 10)),
			DampingX = 950, 
			DampingY = 950,
			Stretch = PV_Speed(800, 200),
			
			name = "LongSpark",
			stdAmount = 2,
		
			init = def.FishInit,
		},
		sparkle = {
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
			
			name = "StarSpark",
			stdAmount = 3,
			
			init = def.SparkleInit,
		},
	};
}












