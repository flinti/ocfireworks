
global func BlendColorChannels(int ch1, int ch2)
{
	return Sqrt( (ch1**2 + ch2**2) / 2 );
}

global func BlendAlphaChannels(int ch1, int ch2)
{
	return (ch1 + ch2) / 2;
}

global func BlendColors(int clr1, int clr2)
{
	var rgba = SplitRGBaValue(clr1);
	var rgba2 = SplitRGBaValue(clr2);

	rgba[0] = BlendColorChannels(rgba[0], rgba2[0]);
	rgba[1] = BlendColorChannels(rgba[1], rgba2[1]);
	rgba[2] = BlendColorChannels(rgba[2], rgba2[2]);
	rgba[3] = BlendAlphaChannels(rgba[3], rgba2[3]);

	return RGBa(rgba[0], rgba[1], rgba[2], rgba[3]);
}

global func InvertColor(int clr)
{
	var a = GetRGBaValue(clr, 0);
	clr = RGB2HSL(clr);
	var hue = GetRGBaValue(clr, 1), sat = GetRGBaValue(clr, 2), light = GetRGBaValue(clr, 3);
	return HSLa(hue + 180, sat, light, a);
}
