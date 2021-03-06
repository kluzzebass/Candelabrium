
#include <Effect.h>


/*
 * Cyclic color sequencer with or without white
 *
 * Red		255,	0,		0
 * Yellow	255,	255,	0
 * Green	0,		255,	0
 * Cyan     0,		255,	255
 * Blue		0,		0,		255
 * Magenta	255,	0,		255
 * White	255,	255,	255
 */

Color Effect::colorSequence(int seqPos, bool includeWhite)
{
	int totalSegments = includeWhite ? 7 : 6;
	seqPos %= colorsPerSegment * totalSegments;
	int segment = seqPos / colorsPerSegment;
	int segmentColor = seqPos % colorsPerSegment;

	// Red -> Yellow
	if (segment == 0) return Color(255, segmentColor, 0);

	// Yellow -> Green
	if (segment == 1) return Color(255 - segmentColor, 255, 0);

	// Green -> Cyan
	if (segment == 2) return Color(0, 255, segmentColor);

	// Cyan -> Blue
	if (segment == 3) return Color(0, 255 - segmentColor, 255);
	
	// Blue -> Magenta
	if (segment == 4) return Color(segmentColor, 0, 255);

	if (includeWhite)
	{
		// Magenta -> White
		if (segment == 5) return Color(255, segmentColor, 255);

		// White -> Red
		return Color(255, 255 - segmentColor, 255 - segmentColor);
	}

	// Magenta -> Red
	return Color(255, 0, 255 - segmentColor);
}

uint16_t Effect::sequenceLength(bool includeWhite)
{
	return colorsPerSegment * (includeWhite ? 7 : 6);
}

bool Effect::timeForUpdate()
{
	unsigned long now = millis();

	if (now < (lastTime + effectDelay))
		return false;

	lastTime = now;

	return true;
}

void Effect::clear()
{
	for (int i; i < LED_COUNT; i++) buffer[i].clear();
}

void Effect::initShimmer()
{
	shimmerInitialized = true;

	for (int i = 0; i < LED_COUNT; i++)
	{
		shimmerAngle[i] = M_PI * (float)random(0, 100) / (float)100;
		shimmerSpeed[i] = (float)random(1, 10) / (float)100;
	}
}

void Effect::shimmerUpdate()
{
	unsigned long now = millis();

	if (now < (lastShimmer + SHIMMER_DELAY)) return;

	lastShimmer = now;

	for (int i = 0; i < LED_COUNT; i++)
	{
		shimmerAngle[i] = (shimmerAngle[i] + shimmerSpeed[i]);
		if (shimmerAngle[i] > (M_PI * 2)) shimmerAngle[i] -= (M_PI * 2);
	}
}

Color Effect::shimmer(uint8_t led, int seqPos, bool includeWhite)
{
	Color black;
	seqPos += (int)((sin(shimmerAngle[led]) + 1) * SHIMMER_AMPLITUDE);
	seqPos %= sequenceLength(includeWhite);

	return colorSequence(seqPos, includeWhite).fadeTo(black, SHIMMER_POWER, 100);
}