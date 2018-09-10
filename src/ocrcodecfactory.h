#ifndef OCRCODECFACTORY_H
#define OCRCODECFACTORY_H

#include "ocrcodec.h"
#include "ocrcodec_baidu.h"

enum OCRCodecEngine
{
    OCRCodecEngine_eBaidu
};

class OCRCodecFactory
{
public:
    static OCRCodec * instance(OCRCodecEngine engine = OCRCodecEngine_eBaidu)
    {
        if(engine == OCRCodecEngine_eBaidu){
            static OCRCodec_Baidu codec;
            return &codec;
        }else{
            return Q_NULLPTR;
        }
    }

    static void destroy()
    {
        //do nothing
    }
};


#endif // OCRCODECFACTORY_H
