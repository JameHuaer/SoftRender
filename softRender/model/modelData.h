#ifndef MODELDATA_H
#define MODELDATA_H
#include <./softRender/math/MathUtil.h>
#include <./softRender/math/Matrix.h>
#include <./softRender/math/Vector.h>
#include "FrameImage.h"
enum ModelFillMode { kWireFrame, kSolide };
class ModelData {
   public:
   private:
   FrameImage * frame_image;//帧图片
   ModelFillMode fill_mode;
   
};
#endif