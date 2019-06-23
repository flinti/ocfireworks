/**
 * Firework Mortar
 * Shoots a firework bomb into the sky.
 * 
 * @author Flinti
*/



func Initialize()
{
	
}

public func ControlUse(object clonk, int x, int y)
{	
	if(Touchable)
	{
		SetFused(clonk);
		return;
	}
	
	if(Distance(x, y, 0, 0) > 40)
		x = y = 0;
	
	Exit(x, y);
	
	MakeCollectible(false);
	return true;
}

public func MakeCollectible(bool yes)
{
	if(yes)
	{
		if(Contents())
			Contents()->Exit();
		Touchable = 0;
		Collectible = true;
	}
	else
	{
		Collectible = false;
		Touchable = 2;
	}
}

public func SetUp(object bomb, int r)
{
	MakeCollectible(false);
	if(Contents())
		Contents()->Exit();
	bomb->Enter(this);
	SetR(r);
}

public func SetFused(object by)
{
	//var obj = Contents();
	var obj = CreateContents(FireworkShellChrysanthemum);
	if(obj)
	{
		var r = GetR(), speed = obj->~GetStdExitSpeed() ?? 58;
		var x = Sin(r, 4), y = -Cos(r, 4);
		obj->Exit(x, y);
		obj->SetObjectLayer(obj);
		obj->SetR(r);
		obj->SetXDir(Sin(r, speed));
		obj->SetYDir(-Cos(r, speed));
		if(!obj->~ShotSound())
			Sound("Firework::HeavyShot?", nil, nil, nil, nil, nil, RandomX(-10, 20));
		obj->SetFused(by);
		CreateMuzzleFlash(x, y, r, 6);
	}
}

// Can contain exactly one firework bomb
private func RejectCollect(id def, object obj)
{		
	if(!obj->~IsFireworkBomb() || Contents())
		return true;
}

private func Incineration()
{
	SetFused();
}



public func GetInteractionMenus(object clonk)
{
	var menus = _inherited() ?? [];
	
	var menu =
	{
		title = "$Actions$",
		entries_callback = this.GetMenuEntries,
		callback = "OnMenuAction",
		callback_hover = "OnMenuHover",
		callback_target = this,
		BackgroundColor = RGB(0, 50, 50),
		Priority = 30
	};
	PushBack(menus, menu);
	return menus;
}

// The interactions depend on weather I am empty or not
private func Collection2()
{
	UpdateInteractionMenus(this.GetMenuEntries);
}

private func Ejection()
{
	UpdateInteractionMenus(this.GetMenuEntries);
}

public func GetMenuEntries(object clonk)
{
	var menu_entries = [];
	var custom_entry = 
		{
			Right = "100%", Bottom = "2em",
			BackgroundColor = {Std = 0, OnHover = 0x50ff0000},
			image = {Right = "2em"},
			text = {Left = "2em"}
		};
			
	PushBack(menu_entries, {symbol = FireworkMortar, extra_data = "mkcollectible", 
		custom = new custom_entry
		{
			Priority = 20,
			text = new custom_entry.text { Text = "$MkCollectible$"},
			image = new custom_entry.image { Symbol = GetID() }
		}});
	
	if(CanFire())
	{
		PushBack(menu_entries, {symbol = FireworkSymbol_Ignite, extra_data = "fire", 
			custom = new custom_entry
			{
				Priority = 30,
				text = new custom_entry.text { Text = "$Fire$"},
				image = new custom_entry.image { Symbol = FireworkSymbol_Ignite }
			}});
	}

	return menu_entries;
}


public func OnMenuHover(id symbol, string action, desc_menu_target, menu_id)
{
	var text = "";
	if (action == "fire")
		text = "$FireDesc$";
	else if(action == "mkcollectible")
		;

	GuiUpdateText(text, menu_id, 1, desc_menu_target);
}


public func OnMenuAction(symbol_or_object, string action, bool alt)
{
	if (action == "fire")
		this->SetFused();//symbol_or_object);
	else if(action == "mkcollectible")
		MakeCollectible(true);

	UpdateInteractionMenus(this.GetMenuEntries);	
}


local Collectible = true;
local Touchable = 0;
local Name = "$Name$";
local Description = "$Description$";
local Components = { Metal = 1 };

public func IsToolProduct() { return true; }
public func IsContainer() { return Touchable; }
public func IsElectricalIgnitable() { return Touchable; }
public func CanFire() { return Touchable && Contents(); }




