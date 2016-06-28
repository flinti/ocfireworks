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
	
	//var obj = CreateObject(Rock, LandscapeWidth()/2, LandscapeHeight()/2-20);
	//obj.Collectible = false;
	//AddEffect("IntCastFirework", obj, 1, 10);
}

func InitializeAmbience()
{
	Tree_Coniferous->Place(5);
	Tree_Coniferous2->Place(5);
	Tree_Coniferous3->Place(5);
	Tree_Coniferous4->Place(5);
	Trunk->Place(3);
	Grass->Place(10);
}

global func FxIntCastFireworkTimer(object target, proplist fx)
{
	var obj = (target->CreateContents(FireworkRocket));
	obj->ControlUse(target);
	obj->SetGraphics(nil, Dummy);
	var angle = RandomX(-10, 10);
	obj->SetVelocity(angle, 60);
	obj->SetR(angle);
}


protected func InitializePlayer(int player, int x, int y, object base, int team, id extra_data)
{
	var clonk = GetCrew(player);
	clonk->CreateContents(FireworkRocket);
	clonk->AddEffect("IntRefillFirework", clonk, 1, 1);
	clonk->SetLightRange(1);
}

global func FxIntRefillFireworkTimer(object target)
{
	if(!target->FindContents(FireworkRocket))
		target->CreateContents(FireworkRocket);
	if(!target->FindContents(FireworkFountain))
		target->CreateContents(FireworkFountain);
	if(!target->FindContents(FireworkCake))
		target->CreateContents(FireworkCake);
}


