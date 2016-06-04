/**
	Test
*/

protected func Initialize()
{
	SetSkyParallax(0, 10, 10);
	CreateEnvironmentObjects("Temperate");
	
	var time = CreateObject(Time);
	time->SetTime(0);
	time->SetCycleSpeed(0);
	
	var obj = CreateObject(Rock, LandscapeWidth()/2, LandscapeHeight()/2-20);
	obj.Collectible = false;
	AddEffect("IntCastFirework", obj, 1, 10);
}

global func FxIntCastFireworkTimer(object target, proplist fx)
{
	var obj = (target->CreateContents(FireworkRocket));
	obj->ControlUse(target);
	var angle = RandomX(-10, 10);
	obj->SetVelocity(angle, 60);
}


protected func InitializePlayer(int player, int x, int y, object base, int team, id extra_data)
{
	var clonk = GetCrew(player);
	clonk->CreateContents(FireworkRocket);
	clonk->AddEffect("IntRefillFirework", clonk, 1, 1);
	clonk->SetLightRange(10);
	
	var nx = LandscapeWidth()/2+20, ny = LandscapeHeight()/2;
	while(GBackSolid(nx, ny))
		ny-=10;
	clonk->SetPosition(nx, ny);
}

global func FxIntRefillFireworkTimer(object target)
{
	if(!target->FindContents(FireworkRocket))
		target->CreateContents(FireworkRocket);
}
