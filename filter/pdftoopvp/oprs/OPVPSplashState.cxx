//========================================================================
//
// SplashState.cc
//
//========================================================================

#include <config.h>

#ifdef USE_GCC_PRAGMAS
#pragma implementation
#endif

#include <string.h>
#ifdef HAVE_CPP_POPPLER_VERSION_H
#include "cpp/poppler-version.h"
#endif
#include "goo/gmem.h"
#include "splash/SplashPattern.h"
#include "splash/SplashScreen.h"
#include "splash/SplashBitmap.h"
#include "splash/SplashState.h"
#include "OPVPSplashState.h"
#include "OPVPSplashClip.h"

//------------------------------------------------------------------------
// SplashState
//------------------------------------------------------------------------

OPVPSplashState::OPVPSplashState(int width, int height, bool vectorAntialias,
			 SplashScreenParams *screenParams) {
  SplashColor color;

  matrix[0] = 1;  matrix[1] = 0;
  matrix[2] = 0;  matrix[3] = 1;
  matrix[4] = 0;  matrix[5] = 0;
  memset(&color, 0, sizeof(SplashColor));
  strokePattern = new SplashSolidColor(color);
  fillPattern = new SplashSolidColor(color);
  screen = new SplashScreen(screenParams);
  blendFunc = NULL;
  strokeAlpha = 1;
  fillAlpha = 1;
  lineWidth = 0;
  lineCap = splashLineCapButt;
  lineJoin = splashLineJoinMiter;
  miterLimit = 10;
  flatness = 1;
  lineDash = NULL;
  lineDashLength = 0;
  lineDashPhase = 0;
  strokeAdjust = false;
  clip = new OPVPSplashClip(0, 0, width - 0.001,
               height - 0.001, vectorAntialias);
  softMask = NULL;
  deleteSoftMask = false;
  inNonIsolatedGroup = false;
  next = NULL;
}

OPVPSplashState::OPVPSplashState(int width, int height, bool vectorAntialias,
			 SplashScreen *screenA) {
  SplashColor color;

  matrix[0] = 1;  matrix[1] = 0;
  matrix[2] = 0;  matrix[3] = 1;
  matrix[4] = 0;  matrix[5] = 0;
  memset(&color, 0, sizeof(SplashColor));
  strokePattern = new SplashSolidColor(color);
  fillPattern = new SplashSolidColor(color);
  screen = screenA->copy();
  blendFunc = NULL;
  strokeAlpha = 1;
  fillAlpha = 1;
  lineWidth = 0;
  lineCap = splashLineCapButt;
  lineJoin = splashLineJoinMiter;
  miterLimit = 10;
  flatness = 1;
  lineDash = NULL;
  lineDashLength = 0;
  lineDashPhase = 0;
  strokeAdjust = false;
  clip = new OPVPSplashClip(0, 0, width - 0.001,
               height - 0.001, vectorAntialias);
  softMask = NULL;
  deleteSoftMask = false;
  inNonIsolatedGroup = false;
  next = NULL;
}

OPVPSplashState::OPVPSplashState(OPVPSplashState *state) {
  memcpy(matrix, state->matrix, 6 * sizeof(SplashCoord));
  strokePattern = state->strokePattern->copy();
  fillPattern = state->fillPattern->copy();
  screen = state->screen->copy();
  blendFunc = state->blendFunc;
  strokeAlpha = state->strokeAlpha;
  fillAlpha = state->fillAlpha;
  lineWidth = state->lineWidth;
  lineCap = state->lineCap;
  lineJoin = state->lineJoin;
  miterLimit = state->miterLimit;
  flatness = state->flatness;
  if (state->lineDash) {
    lineDashLength = state->lineDashLength;
    lineDash = (SplashCoord *)gmallocn(lineDashLength, sizeof(SplashCoord));
    memcpy(lineDash, state->lineDash, lineDashLength * sizeof(SplashCoord));
  } else {
    lineDash = NULL;
    lineDashLength = 0;
  }
  lineDashPhase = state->lineDashPhase;
  strokeAdjust = state->strokeAdjust;
  clip = state->clip->copy();
  softMask = state->softMask;
  deleteSoftMask = false;
  inNonIsolatedGroup = state->inNonIsolatedGroup;
  next = NULL;
}

OPVPSplashState::~OPVPSplashState() {
  delete strokePattern;
  delete fillPattern;
  delete screen;
  gfree(lineDash);
  delete clip;
  if (deleteSoftMask && softMask) {
    delete softMask;
  }
}

#if POPPLER_VERSION_MAJOR > 0 || POPPLER_VERSION_MINOR >= 19
void OPVPSplashState::setState(Splash *osplash) {
  osplash->setMatrix(matrix);
  osplash->setFlatness(flatness);
  osplash->setLineDash(lineDash,lineDashLength,lineDashPhase);
  osplash->setLineCap(lineCap);
  osplash->setStrokeAdjust(strokeAdjust);
  osplash->setMiterLimit(miterLimit);
  osplash->setLineJoin(lineJoin);
}
#endif

void OPVPSplashState::setStrokePattern(SplashPattern *strokePatternA) {
  delete strokePattern;
  strokePattern = strokePatternA;
}

void OPVPSplashState::setFillPattern(SplashPattern *fillPatternA) {
  delete fillPattern;
  fillPattern = fillPatternA;
}

void OPVPSplashState::setScreen(SplashScreen *screenA) {
  delete screen;
  screen = screenA;
}

void OPVPSplashState::setLineDash(SplashCoord *lineDashA, int lineDashLengthA,
			      SplashCoord lineDashPhaseA) {
  gfree(lineDash);
  lineDashLength = lineDashLengthA;
  if (lineDashLength > 0) {
    lineDash = (SplashCoord *)gmallocn(lineDashLength, sizeof(SplashCoord));
    memcpy(lineDash, lineDashA, lineDashLength * sizeof(SplashCoord));
  } else {
    lineDash = NULL;
  }
  lineDashPhase = lineDashPhaseA;
}

void OPVPSplashState::setSoftMask(SplashBitmap *softMaskA) {
  if (deleteSoftMask) {
    delete softMask;
  }
  softMask = softMaskA;
  deleteSoftMask = true;
}
