/**
	Firework Functions
	Firework effect functions

	@author Flinti
*/



/**
 * Create the firework effect defined by effect at x, y.
 * @param effect Firework effect to create
 * @param x x coordinate the effect will be created at. Object local coordinate if called from object context.
 * @param y y coordinate the effect will be created at. Object local coordinate if called from object context.
*/
global func CreateFireworkEffect(proplist effect, int x, int y)
{
	var bomb = CreateObject(FireworkBomb, x, y);
	var fwData = { effects = new effect { die = true } };
	bomb->SetFireworkData(fwData);
	bomb->SetFused(this);
}


